#pragma once
#include <functional>

typedef unsigned int UINT;

inline UINT defaultHashFunction(const char *key, size_t size, int prime)
{
	register UINT h = 0;
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