#pragma once
#include <cstdint>
#include <vector>

template <typename T>
class data_type_traits
{
	using data_type = T *;
	using const_data_type = const T *;
};

// improve hash elements locality
template <class Logger, class HASH_DATA, template<typename> class HashFun >
class HASH_TABLE_BUCKET
{
	using data_type = HASH_DATA *;
	using const_data_type = const HASH_DATA *;
	struct HASH_ELEMENT
	{
		uint32_t		hash=0;
		data_type		pData=nullptr;
		bool			bErased = false;
#ifdef _TIMING
		time_t		tLastAccess;
		time_t		tInsert;
#endif
	};

protected:
	Logger	logger;
	std::vector<HASH_ELEMENT>	pElements;
	const short				bucket_size = 7;
	size_t					overflow;
	size_t					buckets;
	const short				overflow_size = 300;

	uint32_t iEntries=0;
	uint32_t iCollisions;
	uint32_t iMaxDepth;
	uint32_t iCounts[20];
	uint32_t iNewItems;
	uint32_t iErased; // items erased but not deleted
	uint32_t iReusedCount;
	uint32_t iInsertErrors;

	bool HashInitialize(size_t iDesiredSize) {
		resetStats();
		buckets = CalcSize(iDesiredSize);
		overflow = buckets*bucket_size;
		if (pElements.size() < overflow + overflow_size)
			pElements.resize(overflow+ overflow_size);
		return true;
	}

	void resetStats()
	{
		iInsertErrors = 0;
		iEntries = 0;
		iCollisions = 0;
		iMaxDepth = 0;
		iNewItems = 0;
		iErased = 0;
		iReusedCount = 0;
		memset(iCounts, 0, sizeof(iCounts));
	}

	void RemoveElement(HASH_ELEMENT& pHashElt)
	{
		// only erase those not flagged as erased
		if (!pHashElt.bErased) {
			pHashElt.bErased = true;
			iErased++;
			iEntries--;
		}
	}
	class iterator;
public:
	iterator find(const HASH_DATA &pData)
	{
		if (pElements.empty())
			return end();

		// get the bucket.
		size_t hashVal = HashFun<HASH_DATA>()(pData);
		size_t slot = findSlot(&pData, hashVal % buckets * bucket_size, hashVal);
		if (slot == UINT_MAX) // check overflow area
			slot = findSlot(&pData, overflow, hashVal);
		if (slot == UINT_MAX || pElements[slot].bErased || !pElements[slot].pData)
			return end();
#ifdef _TIMING
		pHashElt->tLastAccess = time(0);
#endif
		return iterator(pElements.begin()+slot, pElements.end());
	}

	std::pair<iterator, bool> insert(const HASH_DATA &pData)  // was insert2
	{
		return insert(&pData, true);
	}

	std::pair<iterator, bool> insert(const_data_type pData)  // was insert2
	{
		auto ret = insert(pData, false);
		if (!ret.second)
			delete pData;
		return ret;
	}
	size_t size() const
	{
		return iEntries;
	}

	// return # of elements erased
	size_t erase(const HASH_DATA &pData)
	{
		if (pElements.empty() || size()<1)
			return 0;
		
		size_t hashVal = HashFun<HASH_DATA>()(pData);
		size_t slot = findSlot(&pData, hashVal % buckets * bucket_size, hashVal);
		if (slot == UINT_MAX) // check overflow area
			slot = findSlot(&pData, overflow, hashVal);
		if (slot == UINT_MAX || pElements[slot].bErased || !pElements[slot].pData)
			return 0;

		RemoveElement(pElements[slot]);
		return 1;
	}

	void clear(bool data_erase)
	{
		if (pElements.empty())
			return;
		int deleted = 0;
		//traceHashCollision(iMaxDepth, iCounts, iEntries);
		for (auto x= pElements.begin(); x!= pElements.end(); ++x)
		{
			if (!x->pData)
				continue;
			if (data_erase) {
				delete x->pData;
				x->pData = nullptr;
			}
			x->bErased = true;
			deleted++;
		}
		//logger(LOG_INFO, "Clear HASH_TABLE %d items. newed %d, erased %d, entries %d, insert errors %d ", deleted, iNewItems, iErased, iEntries, iInsertErrors);
		resetStats();
	}
	~HASH_TABLE_BUCKET()
	{
		clear(true);
	}
protected:
	// return int_max if no slot, or return empty slot, or return existing item
	size_t findSlot(const_data_type pData, size_t home, uint32_t hashVal)
	{
		size_t mySlot = UINT32_MAX;
		for (size_t slot = home; slot < home + bucket_size; slot++) {
			if (!pElements[slot].pData)  // empty
				return slot;
			if (pElements[slot].bErased) {  // save the erased slot
				if (mySlot == UINT32_MAX)
					mySlot = slot;
			}
			if (pElements[slot].hash == hashVal && *pElements[slot].pData == *pData)// find existing item
				return slot; 
		}
		return mySlot;
	}
	
	// return value: first - item in the table, second - inserted
	std::pair<iterator, bool> insert(const_data_type pData, bool bStackObj)  // was insert3
	{
		if (pData == nullptr)
			return{ end(), false };

		size_t iDepth = 1;
		auto hashVal = HashFun<HASH_DATA>()(*pData);
		size_t home = hashVal % buckets * bucket_size;
		size_t slot = findSlot(pData, home, hashVal);
		if (slot == UINT_MAX) { // bucket is full, look for overflow area
			slot = findSlot(pData, overflow, hashVal);
			iDepth = bucket_size;
		} 
		else
			iDepth = slot - home;
		if (slot == UINT_MAX)
			return{ end(), false };
		if (slot!=home)
			iCollisions++;
		pElements[slot].hash = hashVal;
		if (!pElements[slot].pData) {  // create new Item
			if (bStackObj) {
				pElements[slot].pData = new HASH_DATA(*pData);
				if (pElements[slot].pData == nullptr) {
					iInsertErrors++;
					return{ end(), false };
				}
			}
			else {
				pElements[slot].pData = const_cast<HASH_DATA*>(pData);  //cast away const
			}
			iEntries++;
			iNewItems++;
#ifdef _TIMING
			pElements[slot].tLastAccess = time(0);
			pElements[slot].tInsert = pElements[slot].tLastAccess;
#endif
			pElements[slot].bErased = false;
			if (iDepth > iMaxDepth)
				iMaxDepth = iDepth;
			if (iDepth < sizeof(iCounts) / sizeof(uint32_t))
				iCounts[iDepth]++;

			return{ { pElements.begin() + slot, pElements.end()}, true };
		}
		else if (pElements[slot].bErased) {
			iEntries++;
			iErased--;
			iReusedCount++;
			pElements[slot].bErased = false;
		}
		*pElements[slot].pData = *pData;// if we find a match record or an erased, copy the new data
#ifdef _TIMING
		pElements[slot].tLastAccess = time(0);
		pElements[slot].tInsert = pElements[slot].tLastAccess;
#endif
		return{ { pElements.begin() + slot, pElements.end() }, false };
	}
public:
	class iterator : public std::iterator<std::forward_iterator_tag, HASH_DATA> {
		using v_iter = typename std::vector<HASH_ELEMENT>::iterator;
		v_iter first;
		v_iter last;
		inline void next() {
			while (first != last) {
				if (first->pData && !first->bErased)
					break;
				++first;
			}
		}
	public:
		iterator(v_iter begin, v_iter end, bool search=false):first(begin),last(end) {
			if (search)
				next();
		}
		iterator(v_iter end) :first(end), last(end) {	}
		friend bool operator==(const iterator& it1, const iterator&it2) {
			return it1.first == it2.first;
		}
		friend bool operator!=(const iterator& it1, const iterator&it2) {
			return it1.first != it2.first;
		}
		iterator& operator++() { ++first; next();  return (*this); }  // prefix
		iterator operator++(int) { auto temp(*this); ++first; next();  return temp; }  // postfix
		HASH_DATA& operator*() { return *first->pData; }
		const HASH_DATA& operator*() const { return *first->pData; }
		HASH_DATA* operator->() const { return first->pData; }
	};
	iterator begin() noexcept { return iterator(pElements.begin(), pElements.end(), true); }
	iterator end() noexcept { return iterator(pElements.end()); }
};
