#pragma once

#include <cstdint>
#include <vector>
#include <cassert>

// closed hashing, or open addressing, using quadratic probing

template <class Logger, class HASH_DATA, template<typename> class HashFun >
class HASH_TABLE_PROBING
{
	int bucket_size = 20003;
	int PROBE_LEN = 140;
	static const size_t	MASK_EMPTY = 0x80000000;  // empty - highest bit not set
	static const size_t MASK_TOMB =  0x40000000;  // tombstone - 2nd highest bit set
	static const size_t MASK_HASH =  0x3FFFFFFF;   // 30 bits hash
	static const size_t MASK_VALID = 0xC0000000;

	struct HASH_ELEMENT {
		size_t		preHash = MASK_EMPTY;  // 2 highest bits are reserved for emptiness and tombstone
		HASH_DATA	data{};
		bool	empty() const {
			return (preHash&MASK_EMPTY) != 0;
		}
		bool	tomb_stone() const {
			return (preHash&MASK_TOMB) != 0;
		}
		bool	valid() const {
			return (preHash&MASK_VALID) == 0;
		}
	};
	std::vector<HASH_ELEMENT>	pElements;
	size_t		elements = 0;
	int findSlot(const HASH_DATA& data)
	{
		auto hashVal = hasher(data)&MASK_HASH;
		for (int i = 0; i < PROBE_LEN; i++) {
			size_t slot = (hashVal + i*i) % bucket_size;
			auto &elem = pElements[slot];
			if (elem.empty())  // keep probing until empty slot
				break;
			else if (elem.preHash == hashVal && elem.data == data)
				return slot;
		}
		return -1;
	}
	HashFun<HASH_DATA> hasher{};

protected:
	bool HashInitialize(size_t iDesiredSize) {
		elements=0;
		bucket_size = CalcSize(iDesiredSize);
		if (pElements.size() < (size_t)bucket_size)
			pElements.resize(bucket_size);
		PROBE_LEN = static_cast<int>(sqrt(bucket_size));
		return true;
	}
	class iterator;
public:
	// probe till end to find existing item
	// remember removed slot if happened
	// insert to removed slot, or the first slot
	std::pair<iterator, bool> insert(const HASH_DATA& value) {
		auto hashVal = hasher(value)&MASK_HASH;
		size_t deleted = UINT32_MAX;
		for (int i = 0; i < PROBE_LEN; i++) {
			size_t slot = (hashVal + i*i) % bucket_size;
			auto &elem = pElements[slot];
			if (elem.empty()) {  // probing complete
				auto *elem2 = &elem;
				if (deleted != UINT32_MAX) {  // re-use deleted slot
					elem2 = &pElements[deleted];
					slot = deleted;
				}
				elem2->preHash = hashVal;
				elem2->data = value;
				elements++;
				return{ pElements.begin() + slot, true };
			}
			else if (elem.preHash == hashVal && elem.data == value) {
				elem.data = value;
				return{ pElements.begin()+slot, false };
			}
			else if (elem.tomb_stone())  // save first deleted slot
				deleted = slot;
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
		pElements[slot].preHash = MASK_TOMB;
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
				if (first->valid()) 
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