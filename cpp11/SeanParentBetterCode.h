#pragma once
#include <random>

// 5 Goals
// no Incomplete types
// no raw loops
// No Incidental Data Structures
// no raw synchronization primitives
// no inheritance (no raw pointers)

template <typename In, typename Pred>
In stable_partition(In first, In last, Pred p)
{
	auto n = last - first;
	if (n == 0)
		return first;
	if (n == 1)
		return first + p(*first);
	auto m = first + n / 2;
	return rotate(stable_partition(first, m, p), m,
		stable_partition(m, last, p));
}


template <typename In, typename Pred>
In stable_partition_position(In first, In last, Pred p)
{
	auto n = last - first;
	if (n == 0)
		return first;
	if (n == 1)
		return first + p(first);
	auto m = first + n / 2;
	return rotate(stable_partition_position(first, m, p), m,
		stable_partition_position(m, last, p));
}

template <typename In>
void sort_subrange(In first, In last, In sf, In sl)
{
	if (sf == sl)
		return;
	if (sf != first) {
		nth_element(first, sf, last);
		++sf; // sf is already sorted
	}
	partial_sort(sf, sl, last);
}

template <typename RandI> // I models RandomAccessIterator
void shuffle(RandI f, RandI l) {
	//thread_local std::default_random_engine engine;    
	std::random_device rd;
	thread_local std::mt19937 engine(rd());
	shuffle(f, l, engine);
}
template<typename Container>
void shuffle(Container& cont)
{
	shuffle(begin(cont), end(cont));
}

template<typename RandI>
std::pair<RandI, RandI> slide(RandI first, RandI last, RandI to)
{
	if (to < first) return{ to, rotate(to, first, last) }; // slide up
	if (to > last) return{ rotate(first, last, to), to }; // slide down
	return{ first, last };
}

template<typename BiDirI, typename UPred>
auto gather(BiDirI first, BiDirI last, BiDirI to, UPred p)->std::pair<BiDirI, BiDirI>
{
	BiDirI a = std::stable_partition(first, to, not1(p));
	BiDirI b = std::stable_partition(to, last, p);
	return{ a, b };
}


template<typename BiDirI, typename UPred>
auto gather2(BiDirI first, BiDirI last, BiDirI to, UPred p)->std::pair<BiDirI, BiDirI>
{
	using value_type = iterator_traits<BiDirI>::value_type;
	BiDirI a = std::stable_partition(first, to, [&p](const value_type& x) {return !p(x); });
	BiDirI b = std::stable_partition(to, last, p);
	return{ a, b };
}
