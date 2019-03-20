#pragma once

#include <unordered_set>

template <class Logger, typename HASH_DATA, template<typename> class HashFun >
class HASH_TABLE_Mock
{
	std::unordered_set<HASH_DATA, HashFun<HASH_DATA>>  table;  // mock implementation using STL
protected:
	Logger	logger;

	bool HashInitialize(size_t iDesiredSize)
	{
		return true;
	}
public:
	auto begin() {
		return table.begin();
	}
	auto end() {
		return table.end();
	}
	bool insert(const HASH_DATA& item) {
		return table.insert(item).second;
	}
	const HASH_DATA *find(const HASH_DATA &pData)
	{
		auto found = table.find(pData);
		if (found == table.end())
			return nullptr;
		return &(*found);
	}
	size_t size() const
	{
		return table.size();
	}
};