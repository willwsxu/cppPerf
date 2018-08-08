#pragma once
#include <functional>

// inheritance hierarchy:
// HASH_TABLE_NEW -> FileRead Policy -> HashTable Impl
// FileRead policy class hierarchy: FileRead -> HASH_TABLE_FILE_READER policy
// FleRead policy = None
template <class Logger, class HASH_DATA, template<typename> class HashFun, template<typename, typename, template<typename> class> typename HashTableImpl>
class FileNone : public HashTableImpl<Logger, HASH_DATA, HashFun>
{
protected:
	template<typename...Args>
	void Init(Args...args)
	{
		size_t size;
		tie(size) = make_tuple(args...); //expect hash table size, no file read or write
		(void)HashInitialize(size);
	}
};

// Policy based design, all parts of hash table are customizable
template <class Logger, class HASH_DATA,
	template<typename, typename, template<typename> class> class HashTableImpl, //customized hash table implementation, sits on top of the class hierarchy
	template<typename, typename, template<typename> class, template<typename, typename, template<typename> class> class> typename FileRead,  // responsible for read and write to file, 4 template deep
	template<typename> class HashFun = std::hash >  // default to use STL hash
class HASH_TABLE_NEW : public FileRead<Logger, HASH_DATA, HashFun, HashTableImpl >
{
public:
	HASH_TABLE_NEW() {};
	template<typename...Args>
	HASH_TABLE_NEW(Args...args) {  // constructor calls Init of different FileRead policy with different parameters
		Init(forward<Args>(args)...);
	}
};