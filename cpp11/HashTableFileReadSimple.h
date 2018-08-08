#pragma once

// simple file reader, HASH_DATA is required to have constructor to take char * 
// Responsible to read hash table contents from file, and write back content to file after updated
template <class Logger, class HASH_DATA, template<typename> class HashFun, template<typename, typename, template<typename> class> class HashTableImpl>
class HASH_TABLE_FILE_READER : public HashTableImpl<Logger, HASH_DATA, HashFun>
{
	std::string filepath;
protected:
	void ReadtoHashTable(const char *szPath) {
		filepath = szPath;
		std::fstream s(szPath, ios_base::in);
		if (!s.is_open()) {
			logger(LOG_WARN, "%s", "open file to read failed");
			return;
		}
		char buf[1024];
		while (s.getline(buf, sizeof(buf))) {
			insert(HASH_DATA(buf));  // HASH_DATA must implement constructor taking char *
		}
		logger(LOG_INFO, "read %d items", size());
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
