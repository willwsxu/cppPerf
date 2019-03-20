#include "stdafx.h"
#include <cassert>
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

// All values will be in the range of [0, 1000000].
// The number of operations will be in the range of[1, 10000].
class MyHashSet {
	const int bucket_size = 20003;
	const int PROBE_LEN = 140;
	struct HASH_ELEMENT {
		size_t	hash = UINT32_MAX;
		int		key = 0;
		bool	tomb = false;
	};
	std::vector<HASH_ELEMENT>	pElements;
	HASH_ELEMENT* find(int key)
	{
		auto hasVal = hasher(key);
		for (int i = 0; i < PROBE_LEN; i++) {
			auto &elem = pElements[(hasVal + i*i) % bucket_size];
			if (elem.hash == UINT32_MAX && !elem.tomb)
				break;
			else if (!elem.tomb && elem.key == key)
				return &elem;
		}
		return nullptr;
	}
	hash<int> hasher;

public:
	/** Initialize your data structure here. */
	MyHashSet() :pElements(bucket_size) {  // beat 17%
	}

	void add(int key) {
		auto hasVal = hasher(key);
		for (int i = 0; i < PROBE_LEN; i++) {
			auto &elem = pElements[(hasVal + i*i) % bucket_size];
			if (elem.hash == UINT32_MAX && !elem.tomb) {
				elem.key = key;
				elem.hash = hasVal;
				return;
			}
			else if (!elem.tomb && elem.key == key)
				return;
		}
		assert(false);  // insert failed
	}

	void remove(int key) {
		auto found = find(key);
		if (found) {
			found->tomb = true;
			found->hash = UINT32_MAX;
		}
	}

	/** Returns true if this set contains the specified element */
	bool contains(int key) {
		return find(key) != nullptr;
	}
};
TEST_CASE("705. Design HashSet", "[HASH]")
{
	MyHashSet my_set;
	my_set.add(1);
	my_set.add(2);
	CHECK(my_set.contains(1));
	CHECK(my_set.contains(3) == false);
	my_set.add(2);
	CHECK(my_set.contains(2));
	my_set.remove(2);
	CHECK(my_set.contains(2) == false);
}

// 706. Design HashMap
class MyHashMap {  // beat 96%
	const int bucket_size = 20003;
	const int PROBE_LEN = 140;
	struct HASH_ELEMENT {
		size_t	hash = UINT32_MAX;
		int		key = 0;
		int		data = 0;
		bool	empty = true;
	};
	std::vector<HASH_ELEMENT>	pElements;
	HASH_ELEMENT* find(int key)
	{
		auto hasVal = hasher(key);
		for (int i = 0; i < PROBE_LEN; i++) {
			auto &elem = pElements[(hasVal + i*i) % bucket_size];
			if (elem.empty)
				break;
			else if (elem.hash == hasVal && elem.key == key)
				return &elem;
		}
		return nullptr;
	}
	hash<int> hasher;
public:
	/** Initialize your data structure here. */
	MyHashMap() :pElements(bucket_size) {
	}

	/** value will always be non-negative. */
	void put(int key, int value) {
		auto hasVal = hasher(key);
		for (int i = 0; i < PROBE_LEN; i++) {
			auto &elem = pElements[(hasVal + i*i) % bucket_size];
			if (elem.empty) {
				elem.key = key;
				elem.hash = hasVal;
				elem.data = value;
				elem.empty = false;
				return;
			}
			else if (elem.hash == hasVal && elem.key == key) {
				elem.data = value;
				return;
			}
		}
		assert(false);  // insert failed
	}

	/** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
	int get(int key) {
		auto found = find(key);
		if (found)
			return found->data;
		return -1;
	}

	/** Removes the mapping of the specified value key if this map contains a mapping for the key */
	void remove(int key) {
		auto found = find(key);
		if (found) {
			found->hash = UINT32_MAX;
			found->key = 0;
		}
	}
};
