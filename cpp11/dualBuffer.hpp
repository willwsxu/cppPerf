#include <sstream>
#include <iostream>
//#include <iomanip>
#include <string>

#include "HpCounter.h"
#include "ScopedGuard.h"
#include "FHXCLParser.h"

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include "seh.h"
#pragma once
/*
Phase 1
Add operation
No lock to append update to data block
if it is full or time up, lock and unload it to queue ( transfer out memory )

Send update operation
Lock the queue,
Get one item
Unlock
process item

phase 2
recycle memory if necessary
*/

// problem: last record not sent

static const int		MEM_BLOCK_SIZE = 8192;
static const char * const szTAL4Format = "TAL4";

struct QDATA
{
	void *		pData;
	UINT32		iSize;
	UINT32		iRec;
	const char *format;

	QDATA(void *d, UINT32 s, UINT32 r, const char *f = szTAL4Format) :pData(d), iSize(s), iRec(r), format(f)
	{
	}
};

using std::list;
//#define MEM_CHECK
// TODO queued records, bytes, latency (when and where),
class DataBatchingBuffer
{
	SQF_HANDLE		currSqf;		// batching updates
									//	deque<QDATA>	dataQueue;
	list<QDATA>		dataQueue;
	CriticalSection	lock_;
	DWORD			iLastTickCount;
	DWORD			dwUpdateInterval;

	// check for error
	DWORD			dwDrop;		// dropped records
	DWORD			dwSleep;
	DWORD			dwBad1;
	DWORD			dwBad2;

	DWORD			dwMaxAllowedQSize;
	volatile DWORD	dwQSize;
	DWORD			dwMaxQSize;
	DWORD			dwMaxQRec;
	volatile DWORD	dwTotalIn;
	volatile DWORD	dwTotalOut;
	volatile DWORD	dwTotalInRec;
	volatile DWORD	dwTotalOutRec;

	char			*sqfMem;

#ifdef TEST_SIMPLE
	UINT32			dwLen;
	UINT32			dwNumRec;
#endif

public:
	DataBatchingBuffer()
	{
		iLastTickCount = XTimer::getInstance().currTickTime();
		dwUpdateInterval = 10;
		dwMaxAllowedQSize = 20000;  // number of records
		if (XCLIsValid())
		{
			dwUpdateInterval = (DWORD)XCLGetLongVal("UpdateInterval", dwUpdateInterval);
			dwMaxAllowedQSize = (DWORD)XCLGetLongVal("ConvDualBufMaxQueue", dwMaxAllowedQSize);
		}
		static int count = 0;
		if (++count == 1)
			tb_msg("UpdateInterval=%d, ConvDualBufMaxQueue=%d", dwUpdateInterval, dwMaxAllowedQSize);

		sqfMem = nullptr;
		(void)allocMemSQF();
		ScopedGuard sg((LPCRITICAL_SECTION)&lock_);  // to fix coverity lock missing
		clearStats1();
		clearStats2();
	}
	~DataBatchingBuffer()
	{
		if (sqfMem)
			delete[]sqfMem;
	}

	inline char * allocMemChunk(UINT size)
	{
		char *pRaw = nullptr;
#ifdef MEM_CHECK
		size = MEM_BLOCK_SIZE + 2;  // fixed size for debugging
#endif
		try {
			/*			if ( gc.size()>0 )
			{
			ScopedGuard sg((LPCRITICAL_SECTION)&lock_);
			if ( gc.size()>0 )
			{
			pRaw = (char *)gc.back();
			gc.pop_back();
			}
			}*/
			//			if ( pRaw==nullptr )
			pRaw = new char[size];
		}
		catch (std::bad_alloc())
		{
			dwBad1++;
			return pRaw;
		}
		catch (...)
		{
			dwBad1++;
			return pRaw;
		}
#ifdef MEM_CHECK
		*pRaw = '<';
		pRaw[size - 1] = '>';
		pRaw++;
#endif
		return pRaw;
	}

	inline bool allocMemSQF()
	{
		sqfMem = allocMemChunk(MEM_BLOCK_SIZE);
		if (sqfMem == nullptr)
			return false;

		currSqf.attach(sqfMem, MEM_BLOCK_SIZE);
		return true;
	}

	inline void clearCurrent()
	{
#ifdef TEST_SIMPLE
		dwLen = 0;
		dwNumRec = 0;
#endif
	}

	// add currSqf to queue
	inline void add2Queue()
	{
		ScopedGuard sg((LPCRITICAL_SECTION)&lock_);
		if (dwTotalInRec - dwTotalOutRec > dwMaxAllowedQSize)
		{
			dwDrop += currSqf.num_messages();
			currSqf.clear();
#ifdef TEST_SIMPLE
			clearCurrent();
#endif
			return;
		}
		UINT datasize = currSqf.size();
#ifdef TEST_SIMPLE
		UINT datasize = dwLen;
#endif
		if (datasize < 5 || datasize> MEM_BLOCK_SIZE)
		{
			dwBad2++;
			currSqf.clear();
#ifdef TEST_SIMPLE
			clearCurrent();
#endif
			return;
		}
		dwTotalIn++;
		dwTotalInRec += currSqf.num_messages();
		dwQSize++;
		dataQueue.push_back(QDATA(currSqf.transfer(), datasize, currSqf.num_messages()));
#ifdef TEST_SIMPLE
		dataQueue.push_back(QDATA(sqfMem, datasize, dwNumRec));
		clearCurrent();
#endif
#ifdef MEM_CHECK
		(void) memoryCheck(sqfMem - 1, datasize);
#endif
		sqfMem = nullptr;

		if (dwQSize != dataQueue.size())
		{
			dwBad2++;
		}
	}

	static int memoryCheck(const char *pRaw, int len)
	{
		if (len <= 0 || len > MEM_BLOCK_SIZE)
		{
			tb_msg("DUALBUF memory corruption, bad len %lu", len);
			return 0;
		}
		if (*pRaw != '<' || pRaw[MEM_BLOCK_SIZE + 1] != '>')
		{
			tb_msg("DUALBUF memory corruption, beg marker %c size %lu end marker %c", *pRaw, len, pRaw[MEM_BLOCK_SIZE + 1]);
			return 0;
		}
		return 1;
	}

	inline BOOL bSend(DWORD currSize, DWORD newsize, DWORD now)
	{
		if (currSize + newsize > MEM_BLOCK_SIZE || (now - iLastTickCount > dwUpdateInterval) && currSize > 4)
			return TRUE;
		return FALSE;
	}

	// flush out last block
	inline BOOL bSend(DWORD currSize)
	{
		if (currSize > 4)
		{
			DWORD now = XTimer::getInstance().currTickTime();
			if (now - iLastTickCount > dwUpdateInterval)
				return TRUE;
		}
		return FALSE;
	}

	// flush out last block, call from same thread as addData
	void heartbeat()
	{
		if (bSend(currSqf.size()))
			add2Queue();
	}

	bool queueWait()
	{
		for (int i = 0; i<10; i++)
		{
			Sleep(1);
			dwSleep++;
			if (dwTotalInRec - dwTotalOutRec < dwMaxAllowedQSize)
			{
				return true;
			}
		}
		return false;
	}

	void addTal4NoBatch(PTAL4_HDR pTal4)
	{
		if (dwTotalInRec - dwTotalOutRec > dwMaxAllowedQSize)
		{
			if (!queueWait())
			{
				ScopedGuard sg((LPCRITICAL_SECTION)&lock_);
				dwDrop += pTal4->wNumRecords;
				return;
			}
		}
		//		PTAL4_HDR copy = TAL4_DupBlock(pTal4);
		char * pTal4New = allocMemChunk(pTal4->dwSize + 1);
		if (pTal4New == nullptr)
			return;
		memcpy(pTal4New, pTal4, pTal4->dwSize);
		//		shared_ptr<char> pData( pTal4New );

		ScopedGuard sg((LPCRITICAL_SECTION)&lock_);
		dwTotalIn++;
		dwTotalInRec += pTal4->wNumRecords;
		dwQSize++;
		dataQueue.push_back(QDATA(pTal4New, pTal4->dwSize, pTal4->wNumRecords, szTAL4Format));
	}

	// did not had the desired performance improvement. Don't use.
	BOOL addTal4AsIs(PTAL4_HDR pTal4)
	{
		if (dwTotalInRec - dwTotalOutRec > dwMaxAllowedQSize)
		{
			if (!queueWait())
			{
				ScopedGuard sg((LPCRITICAL_SECTION)&lock_);
				dwDrop += pTal4->wNumRecords;
				return FALSE;
			}
		}
		ScopedGuard sg((LPCRITICAL_SECTION)&lock_);
		dwTotalIn++;
		dwTotalInRec += pTal4->wNumRecords;
		dwQSize++;
		dataQueue.push_back(QDATA(pTal4, pTal4->dwSize, pTal4->wNumRecords, szTAL4Format));
		return TRUE;
	}

	// add to current buffer, if it is full, push it to queue, and create a new buffer
#ifdef TEST_SIMPLE
	static const int recSize = 40;//8000;
#endif
	void addDataSqf(void *pData)
	{
#ifdef TEST_SIMPLE
		static char testData[100] = { 'A' };
#endif
		//		if ( dwTotalIn %100000==1 )
		//			tb_msg("DUALBUF %p add to queue count %lu", this, dwTotalIn);

		DWORD now = XTimer::getInstance().currTickTime();

		SQF_BLOCK_HEADER *sqf = (SQF_BLOCK_HEADER *)pData;
		//		tbAssert(sqf->block_size_<100);
		//		tbAssert(sqf->block_size_ > 4);
		if (bSend(currSqf.size(), sqf->block_size_, now))
#ifdef TEST_SIMPLE
			if (bSend(dwLen, recSize, now))
#endif
				add2Queue();
		if (currSqf.size() <= 4)
#ifdef TEST_SIMPLE
			if (dwLen < 1)
#endif
				iLastTickCount = now;

		if (sqfMem == nullptr)
		{
			if (!allocMemSQF())
			{
				return;
			}
		}
		currSqf.append(sqf);
#ifdef TEST_SIMPLE
		memcpy(sqfMem + dwLen, testData, recSize);
		dwLen += recSize;
		dwNumRec++;
#endif

		//		if ( sqf->block_size_+m_sqf.size() > MEM_BLOCK_SIZE )
		//			add2Queue();
	}

	void addDataTal4(PTAL4_HDR pTal4)
	{
	}

	QDATA processData()
	{
		if (dwTotalOut == dwTotalIn || dwQSize == 0)
		{
			return QDATA(nullptr, 0, 0);
		}
		ScopedGuard sg((LPCRITICAL_SECTION)&lock_);
		if (dwQSize == 0 || dwQSize != dwTotalIn - dwTotalOut)
		{
			(void)tbAssert(dwQSize>0);
			(void)tbAssert(dwQSize == dwTotalIn - dwTotalOut);
		}
		if (dwMaxQSize < dwQSize)
		{
			dwMaxQSize = dwQSize;
			DWORD QR = dwTotalInRec - dwTotalOutRec;
			if (dwMaxQRec < QR)
				dwMaxQRec = QR;
		}
		if (dataQueue.empty())
		{
			/*			ostringstream ostr;
			if ( getStats(ostr) )
			tb_msg("DUALBUF %p queue empty error, %s", this, ostr.str().c_str());*/
			return QDATA(nullptr, 0, 0);
		}
		dwTotalOut++;
		QDATA qData = dataQueue.front();
		dataQueue.pop_front();
		dwQSize--;
		dwTotalOutRec += qData.iRec;

#ifdef MEM_CHECK
		UINT s2 = dataQueue.size();

		if (!memoryCheck((const char *)qData.pData - 1, qData.iSize))
		{
			return QDATA(nullptr, 0, 0);
		}
#endif

		return qData;
	}

	bool processDataAll(list<QDATA> &dList)
	{
		if (dwTotalOut == dwTotalIn || dwQSize == 0)
		{
			return false;
		}
		ScopedGuard sg((LPCRITICAL_SECTION)&lock_);
		if (dwQSize != dwTotalIn - dwTotalOut)
		{
			(void)tbAssert(dwQSize>0);
			(void)tbAssert(dwQSize == dwTotalIn - dwTotalOut);
			tb_sev_msg(SEV_WARN, "DataBatchingBuffer (%p) processDataAll queue size is %lu, In %lu, Out %lu", this, dwQSize, dwTotalIn, dwTotalOut);
		}
		if (dwQSize == 0)
			return false;
		if (dwMaxQSize < dwQSize)
		{
			dwMaxQSize = dwQSize;
			DWORD QR = dwTotalInRec - dwTotalOutRec;
			if (dwMaxQRec < QR)
				dwMaxQRec = QR;
		}
		auto item = begin(dataQueue);
		while (item != end(dataQueue))
		{
			dwTotalOut++;
			dwTotalOutRec += item->iRec;
			item++;
		}
		dwQSize = 0;
		dList.assign(begin(dataQueue), end(dataQueue));
		dataQueue.clear();
		return !dList.empty();
	}

	void bufDelete(QDATA &qData)
	{
		tal::seh::RegisterTranslator seh;
		try {

#ifdef MEM_CHECK
			(void)memoryCheck((const char *)qData.pData - 1, qData.iSize);
			delete[]((char *)qData.pData - 1);
#else
			delete[](char*)qData.pData;
#endif
		}
		catch (std::exception& excpt) {

			tb_sev_msg(SEV_WARN, "Exception of bufDelete %s", excpt.what());
			Sleep(10);
		}
	}

	void clear()
	{
		ScopedGuard sg((LPCRITICAL_SECTION)&lock_);
		int count = 0;
		while (!dataQueue.empty())
		{
			QDATA qData = dataQueue.front();
			dwDrop += qData.iRec;
			dataQueue.pop_front();
			count++;
		}
		if (count > 0)
		{
			std::ostringstream ostr;
			ostr << "DUALBUF cleared items " << count << " ";
			getStats(ostr);
			tb_msg("%s", ostr.str().c_str());
		}
		dwTotalOut = dwTotalIn;
		dwQSize = 0;
	}

	void clearStats1()
	{
		dwMaxQSize = 0;
		dwMaxQRec = 0;
		dwSleep = 0;
		dwDrop = 0;
		dwBad1 = 0;
		dwBad2 = 0;
#ifdef TEST_SIMPLE
		dwLen = 0;
		dwNumRec = 0;
#endif
	}
	void clearStats2()
	{
		dwQSize = 0;
		dwTotalIn = 0;
		dwTotalOut = 0;
		dwTotalInRec = 0;
		dwTotalOutRec = 0;
	}
	void clearStats()
	{
		ScopedGuard sg((LPCRITICAL_SECTION)&lock_);
		clearStats1();
		if (empty())
		{
			clearStats2();
		}
		else
		{
			tb_msg("DataBatchingBuffer::clearStats failed, queue is not empty %lu", dwQSize);
		}
	}

	bool empty()
	{
		return dwTotalIn == dwTotalOut && dwQSize == 0;
	}

	UINT32 getQRec() const
	{
		return (UINT32)(dwTotalInRec - dwTotalOutRec);  // queued rec
	}

	UINT32 getQSize() const
	{
		return dwQSize;
	}
	UINT32 getMaxQSize() const
	{
		return dwMaxQSize;
	}

	UINT32 getMaxQRec() const
	{
		return dwMaxQRec;
	}

	UINT32 getDropped() const
	{
		return dwDrop;
	}

	UINT32 getSleep() const
	{
		return dwSleep;
	}

	// return 0, no stats
	int getStats(std::ostringstream& ostr)
	{
		if (dwTotalIn < 1)
			return 0;
		ostr << "DUALBUF Total items In " << dwTotalIn << " (" << dwTotalInRec << " rec) " << " Out " << dwTotalOut << " (" << dwTotalOutRec << " rec) ";
		ostr << " MQ: " << dwMaxQSize << " MQR: " << dwMaxQRec << " Q: " << dataQueue.size() << " calc Q: " << dwQSize;

#ifdef TEST_SIMPLE
		if (dwNumRec>0)
			ostr << " leftover rec " << dwNumRec;
#endif
		if (currSqf.size()>4)
			ostr << " leftover rec: " << currSqf.num_messages() << " size: " << currSqf.size();
		if (dwDrop)
			ostr << " drop msg: " << dwDrop;
		if (dwBad1)
			ostr << " bad alloc: " << dwBad1;
		if (dwBad2)
			ostr << " bad2: " << dwBad2;
		if (dwSleep > 0)
			ostr << " Sleep: " << dwSleep;
		//		ostr << std::endl;
		return 1;
	}

};

class DataQManager
{
	DataBatchingBuffer	sqfOut[MAX_SOURCES];

public:
	DataQManager()
	{
	}

	inline DataBatchingBuffer& myBuffer(WORD iSource)
	{
		return sqfOut[iSource];
	}

	void clearStats()
	{
		for (int i = 0; i<MAX_SOURCES; i++)
			sqfOut[i].clearStats();
	}

	//		return sprintf(pBuf, "Q: %lu MQ: %lu (%lu KB) RCV %lu Sent %lu Latency %lu", //MQR %lu 
	//			qsize(), peakQueueSize, peakQueueBytes/1024, totalAdded, totalProcessed, peakQueueLatency);  //peakQueueRec
	unsigned int getStats(char * buf)
	{
		UINT32 qrec = 0, qsize = 0, mqsize = 0, drop = 0, mqr = 0, dwSleep = 0;
		for (int i = 0; i<MAX_SOURCES; i++)
		{
			qrec += sqfOut[i].getQRec();
			qsize += sqfOut[i].getQSize();
			mqsize += sqfOut[i].getMaxQSize();
			mqr += sqfOut[i].getMaxQRec();
			drop += sqfOut[i].getDropped();
			dwSleep += sqfOut[i].getSleep();
		}
		int len = sprintf(buf, "QR: %lu Q: %lu MQR: %lu MQ: %lu", qrec, qsize, mqr, mqsize);
		if (drop> 0)
			len += sprintf(buf + strlen(buf), " DROP: %lu", drop);
		if (dwSleep > 0)
			len += sprintf(buf + strlen(buf), " Sleep: %lu", dwSleep);
		return len;
	}
};

// multi sender, each to its own buffer
// one reader
#include "FID_VALS.H"
#include "tal4Hdl.h"
class DualBufferTest
{
public:
	DataBatchingBuffer myBuffer;
	DualBufferTest()
	{
		thread_ = nullptr;
		bExit = false;
		bExited = false;
		updatesToSend = 1;
		packetSize = 100;
	}
	~DualBufferTest()
	{
		if (thread_)
		{
			try {
				delete thread_;
			}
			catch (...)
			{
			}
		}
	}

	void DoUpdates()
	{
#ifdef TEST_SIMPLE
		char buffer[MEM_BLOCK_SIZE];
#endif
		UINT32 i;
		DWORD now = XTimer::getInstance().currTickTime();

		SQF_HANDLE testSQF(200);
		TALTIME ttTime;
		testSQF.begin_message(2, ttTime);
		testSQF.add_symbol_info("+AAPL\\9M4\\10");
		LPACK lp;
		lp.lVal = 1100;
		lp.wBase = 'm';
		testSQF.add_quote('A', lp, 100, '1', SQF_QUOTE_SEGMENT::BEST_ASK);
		testSQF.add_quote('B', lp, 100, '2', SQF_QUOTE_SEGMENT::BEST_BID);
		testSQF.end_message();

		tb_msg("BUFFERTEST: producer %p Start Send at %lu. Rec size %d", this, now, testSQF.size());
		for (i = 0; i<updatesToSend; i++)
		{
			//			myBuffer.heartbeat();
			myBuffer.addDataSqf((void *)testSQF.get());
			if (bExit)
			{
				i++;
				break;
			}
			//			if ( i%1000==0 )
			//				Sleep(0);
			if (myBuffer.getQSize() > 1000)
				Sleep(1);
		}
		Sleep(50);
		myBuffer.heartbeat();
		DWORD end = XTimer::getInstance().currTickTime();
		tb_msg("BUFFERTEST: producer %p updates %lu ends at %lu, took %lu msec, rate %lu", this, i, end, end - now, i / (end - now) * 1000);
		std::ostringstream ostr;
		myBuffer.getStats(ostr);
		tb_msg("BUFFERTEST: producer %p %s", this, ostr.str().c_str());
		bExited = true;
		//		delete thread_;
	}


	void DoUpdatesTal4()
	{
		UINT32 i;
		DWORD now = GetTickCount();

		Tal4Handle testTal4;

		tb_msg("BUFFERTEST: producer %p Start Send at %lu. TAL4 block size %lu, num rec %lu", this, now, testTal4.pTal4->dwSize, testTal4.size());
		for (i = 0; i<updatesToSend; i++)
		{
			while (testTal4.pTal4->dwSize < MEM_BLOCK_SIZE)
			{
				testTal4.addRecordNoLatency("TESTSYM", "TST", 1, 2);
				for (int i = 0; i<10; i++)
					testTal4.add(T_BIDSIZE, (long)200);
			}

			// addTal4AsIs is much slower. I speculate that becasue it alloc size much larger.
			// rate 18118059 6876155
			myBuffer.addTal4NoBatch(testTal4.pTal4);
			testTal4.reset();

			//			myBuffer.addTal4AsIs(testTal4.pTal4);
			//			testTal4.create(0x8000);
			if (bExit)
			{
				i++;
				break;
			}
			//			if ( i%1000==0 )
			//				Sleep(0);
			if (myBuffer.getQSize() > 1000)
				Sleep(1);
		}
		//		Sleep(50);
		//		myBuffer.heartbeat();
		DWORD end = GetTickCount();
		__int64 blkRate = (__int64)i * 1000 / (end - now);
		tb_msg("BUFFERTEST: TAL4 producer %p updates %lu (rec %I64u) ends at %lu, took %lu msec, block rate %lu, rec rate %I64u, byte rate %I64u",
			this, i, (__int64)i*testTal4.size(), end, end - now, (DWORD)blkRate, blkRate*testTal4.size(), blkRate*testTal4.pTal4->dwSize);
		std::ostringstream ostr;
		myBuffer.getStats(ostr);
		tb_msg("BUFFERTEST: TAL4 producer %p %s", this, ostr.str().c_str());
		bExited = true;
		//		delete thread_;
	}

	void Stop()
	{
		if (thread_ == nullptr)
			return;
		bExit = true;
		thread_->join();
		delete thread_;
	}

	boost::thread*	thread_;
	volatile bool	bExit;
	volatile bool	bExited;

	UINT32			updatesToSend;
	int				packetSize;

	void StartSender(UINT32 updates, int size, bool bSQF = true)
	{
		updatesToSend = updates;
		packetSize = size;
		if (thread_)
			Stop();
		bExit = false;
		bExited = false;
		if (bSQF)
			thread_ = new boost::thread(boost::bind(&DualBufferTest::DoUpdates, this));
		else
			thread_ = new boost::thread(boost::bind(&DualBufferTest::DoUpdatesTal4, this));
	}

	bool done()
	{
		return bExited;
	}
};

class DualBufferTestReceiver
{
	DualBufferTest	sender[100];
	UINT32			updateCount;
	__int64			updateRec;
	HpCounter		totUpdates;
	boost::thread*	thread_;
	int				totSenders;
	volatile bool	bExited;

public:
	DualBufferTestReceiver()
	{
		updateCount = 0;
		updateRec = 0;
		thread_ = nullptr;
		totSenders = 1;
		bExited = false;
		totUpdates.setHeading("DualBufferTest");
	}

	void DoReceive()
	{
		bExited = false;
		DWORD start = 0;
		int retryOnDone[100] = { 0 };
		int doneCount[100] = { 0 };
		std::list<QDATA> dList;
		while (true)
		{
			int done = 0;
			for (int i = 0; i< totSenders; i++)
			{
				// receive only after send is done. test threading/lock issue
				/*				if ( !sender[i].done() )
				{
				Sleep(1);
				continue;
				}
				*/
				// processDataAll 149 million/sec vs processData 139 milllion/s (7 producer, 1 consumer, 8 core box)
				//				QDATA qData = sender[i].myBuffer.processData();
				//				if (qData.pData==nullptr)

				if (!sender[i].myBuffer.processDataAll(dList))
				{
					Sleep(0);
					if (sender[i].done())
					{
						if (sender[i].myBuffer.empty())
						{
							done++;
							doneCount[i]++;
							if (doneCount[i] == 1)
								tb_msg("BUFFERTEST read %d done", i + 1);
						}
						else
							retryOnDone[i]++;
					}
					if (retryOnDone[i] >= 10)
					{
						if (!sender[i].myBuffer.empty())
						{
							sender[i].myBuffer.clear();
							tb_msg("BUFFERTEST clear %d, %d", i + 1, retryOnDone[i]);
						}
						(void)tbAssert(sender[i].myBuffer.empty());
					}
					continue;
				}
				while (!dList.empty())
				{
					QDATA &qData = dList.front();

					(void)InterlockedIncrement((LONG *)&updateCount);
					if (updateCount == 1)
					{
						start = GetTickCount();
						tb_msg("BUFFERTEST: receive first msg at %lu on sender %d", start, i + 1);
					}
					//				if ( updateCount%100000==0 )
					//					tb_msg("BUFFERTEST: receive %lu, sender %d", updateCount, i+1);

					totUpdates.increment(qData.iRec, 0);
					updateRec += qData.iRec;
					sender[i].myBuffer.bufDelete(qData);

					dList.pop_front();
				}
			}
			if (done == totSenders)
				break;
		}
		DWORD end = GetTickCount();
		Sleep(30);  // allow the counter class to get the latest number
		tb_msg("BUFFERTEST: Receive done at %lu. Took %lu msec. block %lu rate %lu, rec %I64u Rate %I64u msg/sec. %I64u rec/blk counter rate %lu K, peak %lu K, total rec  %I64u",
			end, end - start, updateCount, updateCount / (end - start) * 1000, updateRec, updateRec * 1000 / (end - start), updateRec / updateCount,
			totUpdates.getRate(), totUpdates.getMaxRate(), totUpdates.getCount());

		for (unsigned int i = 0; i< (unsigned int)totSenders; i++)
		{
			std::ostringstream ostr;
			sender[i].myBuffer.getStats(ostr);
			tb_msg("BUFFERTEST: send %d %s", i + 1, ostr.str().c_str());
		}

		bExited = true;;
	}

	// Note: size is not used by the test 
	void StartTest(UINT32 updates, int size, int senders)
	{
		totSenders = senders;
		for (int i = 0; i< senders; i++)
		{
			sender[i].StartSender(updates, size);
		}
		if (thread_)
			delete thread_;
		thread_ = new boost::thread(boost::bind(&DualBufferTestReceiver::DoReceive, this));
		tb_msg("BUFFERTEST: send %lu updates, sends %d", updates, senders);
	}

	void StartTestTal4(UINT32 updates, int size, int senders)
	{
		totSenders = senders;
		for (int i = 0; i< senders; i++)
		{
			sender[i].StartSender(updates, size, false);
		}
		if (thread_)
			delete thread_;
		thread_ = new boost::thread(boost::bind(&DualBufferTestReceiver::DoReceive, this));
		tb_msg("BUFFERTEST: send batched TAL4 %lu updates, senderss = %d", updates, senders);
		Sleep(300);
		if (!bExited)
			thread_->join();
		delete thread_;
		thread_ = nullptr;
		tb_msg("BUFFERTEST: complete");
	}
};
