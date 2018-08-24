#pragma once

template <class Logger, class HASH_DATA, template<typename> class HashFun >
class FH_HASH_TABLE_BASE
{
	struct HASH_ELEMENT
	{
		HASH_DATA *pData;
		bool		bErased;
		time_t		tLastAccess;
		time_t		tInsert;
		HASH_ELEMENT *pNextElement;
	};
protected:
	Logger	logger;
	size_t iSize;
	HASH_ELEMENT **pElements;


	UINT iEntries;
	UINT iCollisions;
	UINT iMaxDepth;
	UINT iCounts[20];
	UINT iNewItems;
	UINT iErased; // items erased but not deleted
	UINT iReusedCount;
	UINT iInsertErrors;

	bool HashInitialize(size_t iDesiredSize) {
		if (pElements) {
			free(pElements);
		}
		InitValues();
		iSize = CalcSize(iDesiredSize);

		pElements = (HASH_ELEMENT **)malloc(sizeof(HASH_ELEMENT *) * iSize);
		if (!pElements)
		{
			logger(LOG_INFO, "Can't alloc hash table %d", iSize);
			pElements = nullptr;
			return false;
		}
		memset(pElements, 0, sizeof(HASH_ELEMENT *) * iSize);
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

	void RemoveElement(HASH_ELEMENT * pHashElt)
	{
		// only erase those not flagged as erased
		if (!pHashElt->bErased) {
			pHashElt->bErased = TRUE;
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
public:
	FH_HASH_TABLE_BASE() :pElements(nullptr), iSize(0)
	{}

	HASH_DATA *find(const HASH_DATA &pData)
	{
		if (!iSize || !pElements)
			return nullptr;

		// get the bucket.
		size_t hashVal = HashFun<HASH_DATA>()(pData);
		HASH_ELEMENT *pHashElt = pElements[hashVal % iSize];

		// spin through the linked list looking for the correct entry.
		while (pHashElt && (pHashElt->bErased || *pHashElt->pData != pData))
			pHashElt = pHashElt->pNextElement;

		// if we found the correct entry, update the timestamp, then return the data
		if (pHashElt) {
			pHashElt->tLastAccess = time(0);
			return pHashElt->pData;
		}

		// bad news,  entry not found.
		return nullptr;
	}

	std::pair<HASH_DATA *, bool> insert(const HASH_DATA &pData)  // was insert2
	{
		return insert(&pData, true);
	}
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

protected:

	// return value: first - item in the table, second - inserted
	std::pair<HASH_DATA *, bool> insert(const HASH_DATA *pData, bool bStackObj)  // was insert3
	{
		if (pData == nullptr || !pElements)
			return{ nullptr, false };

		UINT iDepth = 1;
		HASH_ELEMENT *pErased = nullptr;
		size_t iHashNumber = HashFun<HASH_DATA>()(*pData) % iSize;
		HASH_ELEMENT *pHashEltBase = pElements[iHashNumber];
		HASH_ELEMENT *pHashEltPrev = nullptr;
		for (HASH_ELEMENT * pHashElt = pHashEltBase; pHashElt != nullptr; pHashElt = pHashElt->pNextElement)
		{
			// if we find an element flagged as erased, save it.
			if (pHashElt->bErased) {
				if (pErased == nullptr)
					pErased = pHashElt;
			}
			else

				// if we find a match record, copy the new data
				if (*pHashElt->pData == *pData) {
					/*if (bIsXCL) {
					XCLEntryData *pOld = (XCLEntryData*)pHashElt->pData;
					string OldData = pOld->GetVectorValues();
					if (OldData.length() == 0)
					OldData = "<none>";
					XCLEntryData *pNew = (XCLEntryData*)pData;
					string NewData = pNew->GetVectorValues();
					if (NewData.length() == 0)
					NewData = "<none>";
					if (OldData.find(',') == OldData.npos) {
					(void)tb_sev_msg(SEV_INFO, "<XCL> Duplicate key [%s] detected. Overriding. Old Data: %s, New Data: %s", pData->GetKey(), OldData.c_str(), NewData.c_str());
					}
					else {
					(void)tb_sev_msg(SEV_INFO, "<XCL> Duplicate key [%s] detected. Overriding.", pData->GetKey());
					(void)tb_sev_msg(SEV_INFO, "\t Old Data: %s", OldData.c_str());
					(void)tb_sev_msg(SEV_INFO, "\t New Data: %s", NewData.c_str());
					}
					}*/

					*pHashElt->pData = *pData;
					return{ pHashElt->pData , false };
				}
			pHashEltPrev = pHashElt;
			iDepth++;
		}
		iEntries++;

		if (pErased) {
			*pErased->pData = *pData;

			iErased--;
			iReusedCount++;
			pErased->bErased = false;
			pErased->tLastAccess = time(0);
			pErased->tInsert = pErased->tLastAccess;
			return{ pErased->pData, false };
		}

		// existing code for adding an element to the end of the list
		if (pHashEltBase)
		{
			iCollisions++;
		}

		if (iDepth > iMaxDepth)
			iMaxDepth = iDepth;
		if (iDepth < sizeof(iCounts) / sizeof(UINT))
			iCounts[iDepth]++;

		// Allocate new one 
		HASH_ELEMENT *pNew = (HASH_ELEMENT*)malloc(sizeof(HASH_ELEMENT));
		if (!pNew)
		{
			logger(LOG_INFO, "Can't alloc HASH_ELEMENT");
			iInsertErrors++;
			iEntries--;
			return{ nullptr, false };
		}

		// Fill in data
		if (bStackObj) {
			pNew->pData = new HASH_DATA(*pData);//pData->constructClone();
			if (pNew->pData == nullptr) {
				free(pNew);
				iInsertErrors++;
				iEntries--;
				return{ nullptr, false };
			}
		}
		else {
			pNew->pData = const_cast<HASH_DATA*>(pData);  //cast away const
		}
		iNewItems++;
		pNew->tLastAccess = time(0);
		pNew->tInsert = pNew->tLastAccess;
		pNew->pNextElement = 0;
		pNew->bErased = false;

		// Link it in
		if (!pHashEltBase)
			pElements[iHashNumber] = pNew;
		else
			pHashEltPrev->pNextElement = pNew;
		return{ pNew->pData, true };
	}

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
	}
};
