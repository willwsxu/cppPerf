#pragma once
#include <cstdint>

inline uint32_t defaultHashFunction(const char *key, size_t size, int prime)
{
	register uint32_t h = 0;
	for (size_t i = 0; i<size; i++)
		h = prime * h + key[i];
	return h;
}
template<typename T>
struct HashFun
{
	size_t operator()(const T& t) {
		return std::hash<T>{}(t);
	}
};

static size_t CalcSize(size_t iDesiredSize)
{
	if (iDesiredSize < 8000)
		return 16001;
	else if (iDesiredSize < 16000)
		return 32003;
	else if (iDesiredSize < 32000)
		return 64007;
	else if (iDesiredSize < 50000)
		return 101513;
	else if (iDesiredSize < 100000)
		return 200909;
	else if (iDesiredSize < 200000)
		return 403649;
	else if (iDesiredSize < 400000)
		return 806807;
	else if (iDesiredSize < 1000000)
		return 2000003;
	else if (iDesiredSize < 2000000)
		return 4000037;
	else
		return 8000009;
}

// improve hash elements locality
template <class Logger, class HASH_DATA, template<typename> class HashFun >
class HASH_TABLE_BUCKET
{
	struct HASH_ELEMENT
	{
		uint32_t		hash=0;
		HASH_DATA *		pData=nullptr;
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
		InitValues();
		buckets = CalcSize(iDesiredSize);
		overflow = buckets*bucket_size;
		if (pElements.size() < overflow + overflow_size)
			pElements.resize(overflow+ overflow_size);
		return true;
	}

	void InitValues()
	{
		reset();
	}
	void reset()
	{
		//		iErrors = 0;  reading
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
		if (!pHashElt->bErased) {
			pHashElt->bErased = true;
			iErased++;
			iEntries--;
		}
	}

	virtual void PostInsert(HASH_DATA *pData)
	{
		// Derived class will implement this method to do
		// process with the hash data structure after Insert
	}

	virtual bool ProcessSpecialLine(char* szLine, int iLine)
	{
		// Derived class will implement this function to handle
		// the lines with special processing 
		// these lines will beging with ;# format
		// Default behavior is to ignore this line
		return false;
	}
public:/*
	HASH_DATA *find(const HASH_DATA &pData)
	{
		if (pElements.empty())
			return nullptr;

		// get the bucket.
		size_t hashVal = HashFun<HASH_DATA>()(pData);
		HASH_ELEMENT *pHashElt = &pElements[hashVal % pElements.size()];

		// spin through the linked list looking for the correct entry.
		while (pHashElt && (pHashElt->bErased || *pHashElt->pData != pData))
			pHashElt = pHashElt->pNextElement;

		// if we found the correct entry, update the timestamp, then return the data
		if (pHashElt) {
#ifdef _TIMING
			pHashElt->tLastAccess = time(0);
#endif
			return pHashElt->pData;
		}

		// bad news,  entry not found.
		return nullptr;
	}
	*/
	std::pair<const HASH_DATA *, bool> insert(const HASH_DATA &pData)  // was insert2
	{
		return insert(&pData, true);
	}
	/*
	std::pair<HASH_DATA *, bool> insert(const HASH_DATA *pData)  // was insert2
	{
		auto ret = insert(pData, false);
		if (!ret.second)
			delete pData;
		return ret;
	}

	bool erase(HASH_DATA *pData)
	{
		if (!pElements || !pData || !iSize)
			return false;

		size_t hashIndex = HashFun<HASH_DATA>{}(*pData) % iSize;
		HASH_ELEMENT * pHashElt = pElements[hashIndex];

		// More than one element case
		//HASH_ELEMENT * pPrev = pHashElt;
		//bool bFirstNode = true;
		while (pHashElt && pHashElt->pData)
		{
			// Check to see if the element matches
			if (*pHashElt->pData == *pData)
			{
				RemoveElement(pHashElt);
				return true;
			}
			//bFirstNode = false;
			//pPrev = pHashElt;
			pHashElt = pHashElt->pNextElement;
		}
		return false;
	}

	void clear()
	{
		int deleted = 0;
		if (pElements)
		{
			UINT i;

			traceHashCollision(iMaxDepth, iCounts, iEntries);
			for (i = 0; i < iSize; i++)
			{
				HASH_ELEMENT *pNext;
				HASH_ELEMENT *pStart = pElements[i];
				HASH_ELEMENT *pFirst = pStart;

				if (pStart)
				{
					// Delete only the data portion of the first element
					if (pStart->pData)
					{
						delete(pStart->pData);
						deleted++;
					}

					pStart = pStart->pNextElement;

					// Delete both the data portion and the element itself starting 
					// from the second entry of the row
					while (pStart)
					{
						pNext = pStart->pNextElement;
						delete(pStart->pData);
						deleted++;
						free(pStart);
						pStart = pNext;
					}
					free(pFirst);
					pElements[i] = NULL;
				}
			}
		}
		logger(LOG_INFO, "Clear FH_HASH_TABLE %d items. newed %d, erased %d, entries %d, insert errors %d ", deleted, iNewItems, iErased, iEntries, iInsertErrors);
		reset();
	}
	*/
protected:
	// return int_max if no slot, or return empty slot, or return existing item
	size_t findSlot(const HASH_DATA *pData, size_t home, uint32_t hash)
	{
		size_t mySlot = UINT32_MAX;
		for (size_t slot = home; slot < home + bucket_size; slot++) {
			if (!pElements[slot].pData)  // empty
				return slot;
			if (pElements[slot].bErased) {  // save the erased slot
				if (mySlot == UINT32_MAX)
					mySlot = slot;
			}
			if (pElements[slot].hash == hash && *pElements[slot].pData == *pData)// find existing item
				return slot; 
		}
		return mySlot;
	}
	
	// return value: first - item in the table, second - inserted
	std::pair<const HASH_DATA *, bool> insert(const HASH_DATA *pData, bool bStackObj)  // was insert3
	{
		if (pData == nullptr)
			return{ nullptr, false };

		size_t iDepth = 1;
		auto hash = HashFun<HASH_DATA>()(*pData);
		size_t home = hash % buckets * bucket_size;
		size_t slot = findSlot(pData, home, hash);
		if (slot == UINT_MAX) { // bucket is full, look for overflow area
			slot = findSlot(pData, overflow, hash);
			iDepth = bucket_size;
		} 
		else
			iDepth = slot - home;
		if (slot == UINT_MAX)
			return{ nullptr, false };
		if (slot!=home)
			iCollisions++;
		if (!pElements[slot].pData) {  // create new Item
			if (bStackObj) {
				pElements[slot].pData = new HASH_DATA(*pData);
				if (pElements[slot].pData == nullptr) {
					iInsertErrors++;
					return{ nullptr, false };
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

			// Fill in data
			return{ pData, true };
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
		return{ pElements[slot].pData, false };
	}
	/*
	void traceHashCollision(UINT iMaxDepth, UINT *iCounts, UINT iEntries)
	{
		UINT i;
		UINT iTotal = 0;
		double dAvg = 0.0;
		char szAvg[32] = "";
		if (iEntries < 1)
			return;
		for (i = 1; i <= iMaxDepth; i++)
		{
			iTotal += i * iCounts[i];
		}
		dAvg = (double)iTotal / (double)iEntries;
		//sprintf(szAvg, "%3.3f", dAvg);
		logger(LOG_INFO, "Collision(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d; avg = %s)",
			iCounts[1],
			iCounts[2],
			iCounts[3],
			iCounts[4],
			iCounts[5],
			iCounts[6],
			iCounts[7],
			iCounts[8],
			iCounts[9],
			iCounts[10],
			szAvg
		);
	}*/
};
