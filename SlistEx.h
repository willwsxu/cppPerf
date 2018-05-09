
// SLIST with garbage collector
// Activity Diagram
// Create  MemoryRecycler, one per TCP
// Create 8x2 SListEx, each MemoryRecycler is assigned to 8 SListEx
// TCP src 0, Call MemoryRecycler[0].GetNewItem(), Call QParseCtf, call SListEx[0..7].PushItem
// TCP src 1, Call MemoryRecycler[1].GetNewItem(), Call QParseCtf, call SListEx[8..15].PushItem
//   16 parsing threads will call its SListEx::GetItems(), finish rest of parsing, call gc.RecycleItem
// This design is to restrict one producer/one consumer. We can furthur test performance if there are two producers

// 
#include <wtypes.h>  // required SLIST_ENTRY
#include <list>
#include <sstream>
#include <vector>
#include <memory>

using namespace std;

template <typename T>
struct SListItem
{
    SLIST_ENTRY entry;
	T			data;
};


// One producer to call PushItem. It can support multiple producers if sequence can be maintained.
// One consumer to call GetItems. Application should call reserve for the vector for perfornace reason
// When Recycler is depleted, App is recommended to wait for upto 100 msec (log wait count and total time)
// call clearStats  in  framework callback
// call getStats in  framework callback

// Two constructors allow flexibility to each take a shared GC or local ones.
template <class T, class GC>
class SListEx
{
protected:
	SLIST_HEADER	m_head;
	volatile unsigned long	m_NumItemsLife;		// total received
	volatile unsigned long	m_NumItemsOut;		// total items retrieved to application
	unsigned long	m_PeakItems;		// 
	shared_ptr<GC>	pGc;
	string			szPeakTime;

private:
	SListEx(const SListEx& noCopy)=delete;
	SListEx &operator=(const SListEx & right)=delete;
	SListEx(const SListEx&& noCopy) = delete;
	SListEx &operator=(const SListEx && right) = delete;

public:
	SListEx(shared_ptr<GC> _gc) : pGc(_gc)
	{
		::InitializeSListHead( &m_head );
		m_PeakItems=0;
		m_NumItemsOut=0;
		m_NumItemsLife=0;
		myGC=false;
	}

	SListEx()
	{
		::InitializeSListHead( &m_head );
		m_PeakItems=0;
		m_NumItemsLife=0;
		m_NumItemsOut=0;
		pGc = make_shared< GC>();
	}

	// Push item to slist
	void PushItem(SListItem<T> * item)
	{
		if (item != NULL)
		{
			/*PSLIST_ENTRY pE =*/ 
//			(void)InterlockedIncrement(&m_NumItems);
			(void)InterlockedIncrement((LONG*)&m_NumItemsLife);
	/*		if ( m_NumItemsLast=0 && m_NumItems>100 )
			{
				Sleep(0);// yield thread for consumer if it is waiting
				iSleepCount++;
			}*/
			(void)::InterlockedPushEntrySList(&m_head, &item->entry);
			//if (pE != item )	// How to prevent duplicate insert?
		}
	}

	void waiting()
	{
//		(void)InterlockedExchange(&m_NumItemsLast, 0);
		Sleep(0);
	}

	// Get a list of items in the reverse order, Read from BACK to get the right order (use reverse_iterator)
	// _list from caller should be a member data and should call reserve to have a good initial capacity
	// call gc.RecycleItem after each item is done
	// If same item is pushed, behaviour is undefined
//	__declspec(deprecated("GetItem(vector<> ...) is deprecated. Use GetItem(list<> ...)"))
	void GetItems(vector<SListItem<T> *>& _list)
	{
		if ( _list.size() > 0 )
		{
			printf("SListEx GetItems input vector is not empty %d\n", _list.size());
			_list.clear();
		}
		if ( m_NumItemsLife<=m_NumItemsOut )
		{
			if ( m_NumItemsLife< m_NumItemsOut )
				tb_sev_msg(SEV_WARN, "SListEx GetItems (vector) Items in %lu out %lu", m_NumItemsLife, m_NumItemsOut);
			return;
		}
		//if ( m_pRetrieveList==nullptr )
		PSLIST_ENTRY	m_pRetrieveList = ::InterlockedFlushSList( &m_head );
//		int maxNum = _list.capacity();
		while ( 0 != m_pRetrieveList ) //&& maxNum-- > 0 )
		{
			_list.push_back( reinterpret_cast<SListItem<T>*>(m_pRetrieveList) );
			m_pRetrieveList = m_pRetrieveList->Next;
		}

		long m_NumItems = m_NumItemsLife-m_NumItemsOut;
		if ( m_PeakItems < (unsigned long)m_NumItems )
		{
			m_PeakItems = m_NumItems;
			szPeakTime = szCurTime();
		}
		(void)InterlockedExchangeAdd(&m_NumItemsOut, static_cast<unsigned long>(_list.size()));
//		if ( (unsigned long)m_NumItems > _list.capacity() )
//			printf("m_NumItems %lu, _list.size() %lu, capacity %lu. vector realloc\n", m_NumItems, _list.size(), _list.capacity() );
//		(void)InterlockedExchangeAdd(&m_NumItems, -(LONG)_list.size());
	}

	// lmit to the capacity of _list
	void GetItems(std::list<SListItem<T> *>& _list)
	{
		if ( _list.size() > 0 )
		{
			fprintf(stdout, "SListEx GetItems input list is not empty %d\n", _list.size());
			_list.clear();
		}
		if ( m_NumItemsLife<=m_NumItemsOut )
		{
			static long logCount1=0;
			static long logCount2=0;
			if ( m_NumItemsLife< m_NumItemsOut )
			{
				(void)InterlockedIncrement(&logCount1);
				if ( m_NumItemsOut-m_NumItemsLife> 200 )
					(void)InterlockedIncrement(&logCount2);
				if ( logCount1%1000==0 )
					fprintf(stdout, "SListEx GetItems (list) Items in %lu < out %lu. Total count %lu, %lu difference > 200", m_NumItemsLife, m_NumItemsOut, logCount1, logCount2);
			}
			return;
		}
#ifdef DEBUG_TRACE
		DWORD start = XTimer::getInstance().currTickTime();
		DWORD start2 = GetTickCount();
#endif
		PSLIST_ENTRY pE = ::InterlockedFlushSList( &m_head );
#ifdef DEBUG_TRACE
		DWORD elapsed = XTimer::getInstance().currTickTime() - start;
		DWORD elapsed2 = GetTickCount() - start2;
		if (elapsed > 200 || elapsed2 > 200)
			tb_msg("SListEx::GetItems InterlockedFlushSList took %lu msec, vs GetTickCount %lu", elapsed, elapsed2);
		start = XTimer::getInstance().currTickTime();
		start2 = GetTickCount();
#endif
		while ( 0 != pE )
		{
			_list.push_back( reinterpret_cast<SListItem<T>*>(pE) );
			pE = pE->Next;
		}

		long m_NumItems = m_NumItemsLife-m_NumItemsOut;
		if ( m_PeakItems < (unsigned long)m_NumItems )
		{
			m_PeakItems = m_NumItems;
//			szPeakTime = szCurTime();  TODO
		}
		(void)InterlockedExchangeAdd(&m_NumItemsOut, static_cast<unsigned long>(_list.size()));
#ifdef DEBUG_TRACE
		elapsed = XTimer::getInstance().currTickTime() - start;
		elapsed2 = GetTickCount() - start2;
		if (elapsed > 200 || elapsed2 > 200)
			tb_msg("SListEx::GetItems ex-InterlockedFlushSList took %lu msec, vs GetTickCount %lu", elapsed, elapsed2);
#endif
	}

	unsigned long size() const
	{
		return m_NumItemsLife-m_NumItemsOut;
	}

	unsigned long getPeak() const
	{
		return m_PeakItems;
	}

	const char *getPeakTime() const
	{
		return szPeakTime.c_str();
	}

	unsigned long getItemsReceived() const
	{
		return m_NumItemsLife;
	}

	unsigned long getItemsOut() const
	{
		return m_NumItemsOut;
	}
/*	unsigned long getSleepCount() const
	{
		return iSleepCount;
	}*/
	unsigned short size2() const
	{
		return QueryDepthSList(( PSLIST_HEADER )&m_head);
	}

	GC& getGC() const
	{
		return *pGc;
	}

	void clearStats()
	{
		unsigned long subtract = m_NumItemsOut;
		if (subtract>m_NumItemsLife)
		{
			subtract = m_NumItemsLife;
			tb_sev_msg(SEV_WARN, "SListEx reset stats in %lu < out %lu, peak %lu", m_NumItemsLife, m_NumItemsOut, m_PeakItems);
		}
		else
			tb_msg("SListEx reset stats in %lu, out %lu, peak %lu", m_NumItemsLife, m_NumItemsOut, m_PeakItems);
		(void)InterlockedExchange(&m_PeakItems,0);
		(void)InterlockedExchangeSubtract(&m_NumItemsLife,subtract);
		(void)InterlockedExchangeSubtract(&m_NumItemsOut,subtract);
		tb_msg("AFTER SListEx reset stats in %lu, out %lu, peak %lu", m_NumItemsLife, m_NumItemsOut, m_PeakItems);
//		(void)InterlockedExchange(&iSleepCount,0);
	}

	// Total Life items, Current items, Peak and peak time, recycler size
	__declspec(deprecated("getStats(char *buf ...) is deprecated. Use getStats(ostringstream ...)"))
		char *getStats(char *buf, int len, const char *padding, int which)
	{
		if ( len < 100 )
			return buf;  // not enough space
		int n = sprintf_s(buf, len, "%sSlist %d Total Items In %lu Out %lu, in Queue %lu, Peak %lu at %s, "
			"Garbage Queue size %lu. Queue Wait count %lu, Time %lu msec",
			padding, which, m_NumItemsLife, m_NumItemsOut, size(), getPeak(), getPeakTime(), 
			pGc->GetNewedItems(), pGc->GetMaxItemsExceededCount(), pGc->GetTotalSleepTime());
		return buf+n;
	}
	
	// padding is used to provide a meaning tag to a particular slist logging
	void getStats(std::ostringstream &ostr, const char *padding, int which, bool includeGC=true)
	{
		char buf[1000];
		(void)sprintf_s(buf, sizeof(buf), "%sSlist %d Total Items In %lu Out %lu, in Queue %lu, Peak %lu at %s, ",
			padding, which, m_NumItemsLife, m_NumItemsOut, size(), getPeak(), getPeakTime());
		ostr << buf;
		if ( includeGC )
			pGc->getStats(ostr, " ", which);
	}

	// Rarely needed. I used in unit test
	void reset()
	{
//		m_NumItems=0;
		m_PeakItems=0;
		m_NumItemsLife=0;
		m_NumItemsOut=0;
		PSLIST_ENTRY pE = ::InterlockedFlushSList( &m_head );
		while ( 0 != pE )
		{  
			PSLIST_ENTRY next = pE->Next;
			pGc->RecycleItem(reinterpret_cast<SListItem<T>*>(pE));
			pE = next;
		}
	}

};

// call RecycleItem from multiple threads to reuse memory
// call GetNewItem instead of dynamic memory allocation. Can be called from multiple thread.

// Next phase: Ensure this methods has minimal collision with RecycleItem

// memsize  preallocate items
template <typename T, size_t initmemsize, size_t maxmemsize>
class MemoryRecycler
{
	SLIST_HEADER	m_head;			// 
	PSLIST_ENTRY    currList;		// serve GetNewItem, not used for now
	volatile unsigned long		m_NewedItems;
	unsigned long				m_maxItems;
	volatile unsigned long		m_exceedMaxCount;
	volatile unsigned long		m_failedCount;
	volatile unsigned long		m_exceedMaxSleepTimeTotal;	
	unsigned long	m_NumItemsGet;			// total GetNewItem called
	unsigned long	m_NumItemsRecycled;		// total RecycleItem called

	int freeItems(PSLIST_ENTRY pE)
	{
		int count=0;
		while ( 0 != pE )
		{
			SListItem<T>* pItem = reinterpret_cast<SListItem<T>*>(pE);
			pE = pE->Next;
			delete pItem; // _aligned_free
			count++;
		}
		return count;
	}

private:
	MemoryRecycler(const MemoryRecycler& noCopy) = delete;
	MemoryRecycler &operator=(const MemoryRecycler & right)=delete;

public:
	MemoryRecycler()
	{
		currList = NULL;
		m_NewedItems=0;
		m_exceedMaxCount = 0;
		m_exceedMaxSleepTimeTotal = 0;
		m_failedCount=0;
		m_maxItems = maxmemsize;

		::InitializeSListHead( &m_head );

		for (size_t i=0; i<initmemsize; i++)
		{
			SListItem<T> *newItem = new SListItem<T>(); // _aligned_malloc
			(void)InterlockedIncrement( (volatile LONG *)&m_NewedItems );
			RecycleItem(newItem);
			newItem = nullptr;
		}
		m_NumItemsGet=0;
		m_NumItemsRecycled=0;
	}

	~MemoryRecycler()
	{
		int count=0;
		count += freeItems(currList);
		currList = ::InterlockedFlushSList( &m_head );
		count += freeItems(currList);
		fprintf(stderr, "Delete %d items\n", count);
	}


	// Create a new item if it can not find existing memory first
	SListItem<T> * GetNewItem()
	{
		if ( currList== NULL )
			currList = ::InterlockedFlushSList( &m_head );
		if ( currList== NULL )
		{
			if ( m_NewedItems >= m_maxItems )
			{
#ifndef NDEBUG
				printf("Items exceeded max allowed %lu. Is your consumer too slow\n", m_maxItems);
#endif
				(void)InterlockedIncrement( &m_exceedMaxCount );
				unsigned short sSleep = 0;
				while (sSleep < 100)
				{
					currList = ::InterlockedFlushSList( &m_head );
					if (currList == NULL)
					{
						Sleep(1);
						++sSleep;
					}
					else
					{
						SListItem<T> *newItem = reinterpret_cast<SListItem<T>*>(currList);// can not use static_cast
						(void)InterlockedExchangeAdd( &m_exceedMaxSleepTimeTotal, sSleep);
						currList = currList->Next;
						(void)InterlockedIncrement( &m_NumItemsGet );
						return newItem;
					}
				}
				(void)InterlockedExchangeAdd( &m_exceedMaxSleepTimeTotal, sSleep);
				(void)InterlockedIncrement( &m_failedCount );
				return NULL;
			}
			(void)InterlockedIncrement( (volatile LONG *)&m_NewedItems );
			(void)InterlockedIncrement( &m_NumItemsGet );
			return new SListItem<T>();
		}
		SListItem<T> *newItem = reinterpret_cast<SListItem<T>*>(currList);// can not use static_cast
		currList = currList->Next;
		(void)InterlockedIncrement( &m_NumItemsGet );
		return newItem;
	}
	
	// should be deprecated
	SListItem<T> * PopItem()
	{
		SLIST_ENTRY *pE = InterlockedPopEntrySList(&m_head);
		if ( pE ) {
			return reinterpret_cast<SListItem<T>*>(pE);
		}
		if ( m_NewedItems > m_maxItems )
		{
#ifndef NDEBUG
			printf("Items exceeded max allowed %lu. Is your consumer too slow\n", m_maxItems);
#endif
			return NULL;
		}
		(void)InterlockedIncrement( (volatile LONG *)&m_NewedItems );
		return new SListItem<T>();
	}
	
	// Recyle used item
	void RecycleItem(SListItem<T> * item)
	{
		(void)InterlockedIncrement( &m_NumItemsRecycled );
		(void)::InterlockedPushEntrySList(&m_head, &item->entry);
	}

	const unsigned long GetNewedItems() const
	{
		return m_NewedItems;
	}

	const unsigned long GetMaxItemsExceededCount() const
	{
		return m_exceedMaxCount;
	}

	const unsigned long GetTotalSleepTime() const
	{
		return m_exceedMaxSleepTimeTotal;
	}

	const unsigned long MaxItems() const
	{
		return m_maxItems;
	}

	void MaxItems(long maxItems)
	{
		(void)InterlockedExchange((long *)&m_maxItems, maxItems);
	}

	void clearStats()
	{
		(void)InterlockedExchange(&m_exceedMaxCount,0);
		(void)InterlockedExchange(&m_exceedMaxSleepTimeTotal,0);
		(void)InterlockedExchange(&m_failedCount,0);
		(void)InterlockedExchange(&m_NumItemsGet,0);
		(void)InterlockedExchange(&m_NumItemsRecycled,0);
	}

	void getStats(ostringstream &ostr, const char *padding, int which)
	{
		char buf[1000];
		(void)sprintf_s(buf, sizeof(buf), "%sMemory Recycler %d size %lu [Capacity %lu] Requested %lu Recycled %lu, available %lu, Wait count %lu, Sleep %lu msec, Failed Count %lu",
			padding, which, m_NewedItems, m_maxItems, m_NumItemsGet, m_NumItemsRecycled, m_NumItemsRecycled+m_NewedItems-m_NumItemsGet, m_exceedMaxCount, m_exceedMaxSleepTimeTotal, m_failedCount);
		ostr << buf;
	}

	const unsigned long size() const
	{
		// QueryDepthSList is limited to USHORT (65535) but slist has no limit
		unsigned long count=0;
		const SLIST_ENTRY *pE=currList;
		while ( 0 != pE )
		{
			pE = pE->Next;
			count++;
		}

		pE = RtlFirstEntrySList(&m_head);

		while ( 0 != pE )
		{
			pE = pE->Next;
			count++;
		}
		return count;
	}

	void reset()
	{
		SLIST_ENTRY *pE=currList;
		while ( 0 != pE )
		{
			currList = pE;
			pE = pE->Next;
			(void)::InterlockedPushEntrySList(&m_head, currList);
		}
		currList = NULL;
	}
};
