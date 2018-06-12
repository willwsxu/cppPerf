#pragma once
#pragma once
#pragma warning( disable : 4996 )

// winerror.h

#define S_OK                               0L//    ((HRESULT)0L)
#define S_FALSE                            1L//    ((HRESULT)1L)

#include <vector>
#include <string>

static char *GetToken(char **ppCur, char delim, bool bIsXCL)
{
	bool bEndQuotes = false;
	char *pStart = *ppCur;

	if (bIsXCL)
		while (*pStart == ' ' || *pStart == '\t')
			++pStart; // Trim leading whitespace in order to pick up quotes after whitespace correctly

	char *pCur = pStart;

	if (*pStart == '"')
	{
		bEndQuotes = true;
	}

	while (*pCur)
	{
		if (!bEndQuotes)
		{
			if (*pCur == delim)
			{
				*pCur++ = 0;
				*ppCur = pCur;
				return pStart;
			}
		}
		else
		{
			if (*pCur == '"' && (pCur + 1) && *(pCur + 1) == delim)
			{
				if (bIsXCL)
				{ // trim quotes
					*pCur = 0;
					pStart = pStart + 1;
					*ppCur = pCur + 2;
				}
				else
				{
					*++pCur = 0;
					*ppCur = pCur + 1;
				}
				return pStart;
			}
		}
		pCur++;
	}

	if (bIsXCL)
	{
		while (*(pCur - 1) == ' ' || *(pCur - 1) == '\t') // eat trailing whitespace
			*(--pCur) = '\0';

		if (bEndQuotes) {
			++pStart;
			--pCur;
			*pCur = 0; // move start one forward and replace trailing '"' with null
		}
	}

	*ppCur = pCur;
	return pStart;
}


inline void TrimTrailingWhitespace(char * str)
{
	char * pEnd = strrchr(str, '\0');
	while (pEnd && pEnd >= str && (*pEnd == ' ' || *pEnd == '\t' || *pEnd == '\0' || *pEnd == '\n')) { // trim trailing whitespace
		*pEnd = '\0';
		pEnd--;
	}
}

template<typename HASH_DATA>
std::vector<HASH_DATA*> construct(int argc, char *argv[])
{
	std::vector<HASH_DATA*> ans;
	return ans;
}

template <class Logger, class HASH_DATA, template<typename> class HashFun, template<typename, typename, template<typename> class> class HashTableImpl>
class HASH_TABLE_FILE_READER_MOCK : public HashTableImpl<Logger, HASH_DATA, HashFun>
//template <class HASH_DATA, template<typename> typename HashTable>
//class FH_HASH_TABLE_FILE_READER : public HashTable<HASH_DATA>
{
	std::string filepath;
	std::string	lastSymUpdTime;		// file read time
	char delim;
	size_t iErrors;
	time_t	tLastModified;		// file time is changed
	bool bIsXCL;
protected:
	void SetLocalTimeString()
	{
		time_t now = time(NULL);
		struct tm *_tm = localtime(&now);
		std::ostringstream oss;
		oss << std::right << std::setw(2) << std::setfill('0') << _tm->tm_mon + 1 << "/" <<
			std::setw(2) << std::setfill('0') << _tm->tm_mday << "/" <<
			std::setw(4) << std::setfill('0') << (1900 + _tm->tm_year) << " at " <<
			std::setw(2) << std::setfill('0') << _tm->tm_hour << ":" <<
			std::right << std::setw(2) << std::setfill('0') << _tm->tm_min << ":" <<
			std::right << std::setw(2) << std::setfill('0') << _tm->tm_sec;
		lastSymUpdTime = oss.str();
	}

	void ReadtoHashTable(const char *szPath, const char *h="no header")  // = nullptr
	{
		filepath = szPath;
		FILE * pfTransform = nullptr;
		auto err = fopen_s(&pfTransform, szPath, "r");
		if (err)
		{
			//tb_sev_msg(SEV_WARN, "FH_HASH_TABLE::ReadtoHashTable : %s could not be loaded: %s", szPath, strerror(errno));
			return;
		}
		else
		{
			struct _stat FileStat;

			if (_stat(szPath, &FileStat) == S_OK) {
				tLastModified = FileStat.st_mtime;
				//tb_sev_msg(SEV_INFO, "FH_HASH_TABLE::ReadtoHashTable : %s file timestamp on initial read is %I64d", szPath, tLastModified);
			}
			//else
			//tb_sev_msg(SEV_WARN, "FH_HASH_TABLE::ReadtoHashTable : %s could not be loaded: %s", szPath, strerror(errno));
		}
		ReadtoHashTable(pfTransform);
		fclose(pfTransform);
	}

	void ReadtoHashTable(FILE *pfTransform, bool bLog = false)
	{
		// stLine is going to be the dynamic buffer to build complete lines read from the files.
		std::string stLine;
		int iLine = 0;
		int iProcessLine = 0;
		char szLine[2048]; // arbitrary size read buffer.  200 bytes should be enough to read most lines.

		int iOldSize = iEntries;

		iErrors = 0;
		while (!feof(pfTransform))
		{
			// clear the buffers
			stLine.clear();
			memset(szLine, 0, sizeof(szLine));

			// read until a complete line is read.
			while (fgets(szLine, sizeof(szLine), pfTransform) != nullptr) {
				// a complete line is one that is less than the size of the buffer or if the buffer ends with the new line character.
				size_t iLen = strlen(szLine);
				if (iLen < sizeof(szLine) - 1 || szLine[iLen - 1] == '\n') {
					TrimTrailingWhitespace(szLine);

					// join stripped line onto current line
					try {
						stLine.append(szLine);
					}
					catch (std::exception e) {
						//tb_sev_msg(SEV_CRIT, "Error building hashtable string: %s", e.what());
						continue;
					}

					// if the file is an .XCL file, check to see if there is a logical break at the end of the line.
					// If there is, the next line will be appended to the current line.
					/*if (bIsXCL) {
					iLen = stLine.length();
					if (iLen > 2 && stLine[0] != ';' && stLine[iLen - 1] == '-' && stLine[iLen - 2] == '-')
					{
					if (stLine[iLen - 3] != delim) { // line doesn't have delimiter before break
					//tb_sev_msg(SEV_WARN, "ERROR - New line symbol '--' must be preceeded by a field delimiter '%c' on line [%d]", delim, iLine);
					break;
					}
					// remove the "--" from the end of the line
					stLine.erase(iLen - 2, 2);
					iLine++;
					}
					else {
					break;
					}
					}
					else*/
					break;
				}
				else {
					// join szLine line onto current line.  This line is reached if the line read from the
					// file did not have the '\n' character at the end.
					try {
						stLine.append(szLine);
					}
					catch (std::exception e) {
						//tb_sev_msg(SEV_CRIT, "Error building hashtable string: %s", e.what());
						continue;
					}
				}
			}

			// count the line so logged messsages will indicate the correct line.
			iLine++;

			// skip blank lines
			if (stLine.length() == 0)
				continue;

			// Second line.  check to see if the delimiter should be changed.
			if (iProcessLine == 0 && stLine[0] == ';')
			{
				std::string::size_type pos = stLine.find("Delimiter");
				if (pos == std::string::npos) {
					pos = stLine.find("delimiter");
				}
				if (pos != std::string::npos) {
					// Delimiter will be the first non-space character after the keyword.
					// Some files have the keyword with no delimiter.  In that case, use the default.
					for (pos += 9; pos < stLine.length() && stLine[pos] == ' '; ++pos);
					if (stLine[pos] != 0 && stLine[pos] != ' ' && pos < stLine.length())
						delim = stLine[pos];
				}
			}

			if (stLine[0] != ';')
			{
				char *szElem[2048];
				memset(szElem, 0, sizeof(szElem));
				unsigned long linelength = (unsigned long)stLine.length() + 10; // add 10 characters to leave room for null char
				if (linelength < 4096)
					linelength = 4096; // minimum buffer size.  Set to default
									   /*CDynBuffer copy(linelength);  // initialize the dynamic buffer to be large enough to hold the line.
									   char *pCur = copy.GetWritePtr();
									   copy.AppendData(stLine.c_str());
									   *copy.GetWritePtr() = 0;*/
				char *copy = new char[linelength];
				char *pCur = copy;

				int i = 0;

				iProcessLine++;

				// parse line
				szElem[i] = GetToken(&pCur, delim, bIsXCL);
				if (!szElem[i++])
				{
					//tb_msg("error key in %s line %d", szPath, iLine);
					continue;
				}
				while (*pCur) {
					if ((UINT)i >= (sizeof(szElem) / sizeof(szElem[0]))) {
						//tb_sev_msg(SEV_WARN, "FH_HASH_TABLE::ReadtoHashTable exceeded maximum number of allowed values [%d] for key [%s]", sizeof(szElem), szElem[0]);
						break;
					}
					szElem[i++] = GetToken(&pCur, delim, bIsXCL);
				}
				vector<HASH_DATA *> hd = construct<HASH_DATA>(i, szElem);
				if (hd.size() == 0)
					iErrors++;

				for (i = 0; i<(int)hd.size(); i++) {
					if (insert(hd[i]).first != nullptr) {
						PostInsert(hd[i]);
						if (bLog) {
							//tb_sev_msg(SEV_INFO, "Adding %s to %s hash table.", hd[i]->GetKey(), sName.c_str());
						}
					}
				}
				hd.clear();
			}
			if (stLine.length() > 1 && stLine[1] == '#')
			{
				// Need to do special processing here.
				// We need to make a copy of the full string (stLine) as a char *
				// so the string can be passed to the ProcessSpecialLine function.
				char *SpecialLine = new char[stLine.length() + 1];
				if (SpecialLine != nullptr) {
					strncpy_s(SpecialLine, stLine.length() + 1, stLine.c_str(), _TRUNCATE);
					if (ProcessSpecialLine(SpecialLine, iLine))
						iProcessLine++;
					delete[]SpecialLine;
				}
			}
		}
		/*
		if (iErrors > 0 && iEntries - iOldSize == 0)
		tb_sev_msg(SEV_CRIT, "FileReadError. Reading %s (%d entries). Num Errors %d", szPath, iEntries - iOldSize, iErrors);
		else
		tb_msg("Reading %s (%d entries). Num Errors %d", szPath, iEntries - iOldSize, iErrors);

		tb_msg("(Entries=%d Collisions=%d Max depth=%d)", iEntries, iCollisions, iMaxDepth);*/
		traceHashCollision(iMaxDepth, iCounts, iEntries);
		SetLocalTimeString();
	}

public:
	void WriteHashTable() {
		std::fstream s((filepath + ".out").c_str(), ios_base::out);
		if (!s.is_open()) {
			logger(LOG_WARN, "%s", "open file to write failed");
			return;
		}
		copy(begin(), end(), ostream_iterator<HASH_DATA>(s, "\n"));
	}
};
