#pragma once

#include <cstdint>
#include <vector>
#include <cassert>

// closed hashing, or open addressing, using quadratic probing

template <class Logger, class HASH_DATA, template<typename> class HashFun >
class HASH_TABLE_PROBING
{
	const int bucket_size = 20003;
	const int PROBE_LEN = 140;
	struct HASH_ELEMENT {
		size_t	hash = UINT32_MAX;
		HASH_DATA		data{};
		bool	empty = true;
	};
	std::vector<HASH_ELEMENT>	pElements;
	size_t		elements = 0;
	int findSlot(const HASH_DATA& data)
	{
		auto hasVal = hasher(data);
		for (int i = 0; i < PROBE_LEN; i++) {
			size_t slot = (hasVal + i*i) % bucket_size;
			auto &elem = pElements[slot];
			if (elem.empty)  // keep probing until empty slot
				break;
			else if (elem.hash == hasVal && elem.data == data)
				return slot;
		}
		return -1;
	}
	HashFun<HASH_DATA> hasher{};

protected:
	bool HashInitialize(size_t iDesiredSize) {
		elements=0;
		auto buckets = CalcSize(iDesiredSize);
		if (pElements.size() < buckets)
			pElements.resize(buckets);
		return true;
	}
	class iterator;
public:
	// probe till end to find existing item
	// remember removed slot if happened
	// insert to removed slot, or the first slot
	std::pair<iterator, bool> insert(const HASH_DATA& value) {
		auto hasVal = hasher(value);
		for (int i = 0; i < PROBE_LEN; i++) {
			size_t slot = (hasVal + i*i) % bucket_size;
			auto &elem = pElements[slot];
			if (elem.empty) {
				elem.hash = hasVal;
				elem.data = value;
				elem.empty = false;
				elements++;
				return{ pElements.begin() + slot, true };
			}
			else if (elem.hash == hasVal && elem.data == value) {
				elem.data = value;
				return{ pElements.begin()+slot, false };
			}
		}
		return{ end(), false };
		assert(false);  // insert failed
	}
	iterator find(const HASH_DATA &data){
		auto slot = findSlot(data);
		if (slot<0)
			return end();
		return iterator(pElements.begin() + slot, pElements.end());
	}
	size_t erase(const HASH_DATA &data){
		auto slot = findSlot(data);
		if (slot<0)
			return 0;
		pElements[slot].hash = UINT32_MAX;
		elements--;
		return 1;
	}
	size_t size() const
	{
		return elements;
	}

	class iterator : public std::iterator<std::forward_iterator_tag, HASH_DATA> {
		using v_iter = typename std::vector<HASH_ELEMENT>::iterator;
		v_iter first;
		v_iter last;
		inline void next() {
			while (first != last) {
				if (!first->empty && first->hash!= UINT32_MAX)
					break;
				++first;
			}
		}
	public:
		iterator(v_iter begin, v_iter end, bool search = false) :first(begin), last(end) {
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
		HASH_DATA& operator*() { return first->data; }
		const HASH_DATA& operator*() const { return first->data; }
		HASH_DATA* operator->() const { return &first->pData; }
	};
	iterator begin() noexcept { return iterator(pElements.begin(), pElements.end(), true); }
	iterator end() noexcept { return iterator(pElements.end()); }
};