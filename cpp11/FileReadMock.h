#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>


// Responsible to figure out file full path
template <class Logger, class HASH_DATA, template<typename> class HashFun, template<typename, typename, template<typename> class> class HashTableImpl>
class FileReadMock : public HASH_TABLE_FILE_READER_MOCK<Logger, HASH_DATA, HashFun, HashTableImpl>
{
protected:
	template<typename...Args>
	void Init(Args...args)
	{
		size_t size;			// hash table initial size
		const char *filename;   // hash table contents, read back in on start
		tie(size, filename) = make_tuple(args...);
		if (HashInitialize(size))
			ReadtoHashTable(filename);
	}
};
