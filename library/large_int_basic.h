#pragma once
#include <vector>

// store digit in little endian format, from least to most significant
// add string of equal length, return carry
// digit can be int or char type
template<typename RandRevIter>
int sum_int(RandRevIter first1, RandRevIter last1, RandRevIter first2, RandRevIter dest, int offset = '0')
{
	int carry = 0;
	while (first1 != last1) {
		carry += *first1 - offset + *first2 - offset;
		*dest = carry % 10 + offset;
		carry /= 10;
		++first1;
		++first2;
		++dest;
	}
	return carry;
}

// add value v2 to v1
// least significant digit at left, little endian
template<typename T>
inline void sum(std::vector<T>& v1, std::vector<T>& v2)
{
	if (v1.size() < v2.size())
		fill_n(back_inserter(v1), v2.size() - v1.size(), 0);
	else if (v1.size() > v2.size())
		fill_n(back_inserter(v2), v1.size() - v2.size(), 0);
	int carry = sum_int(begin(v1), end(v1), begin(v2), begin(v1), 0);
	while (carry > 0) {
		v1.push_back(carry % 10);
		carry /= 10;
	}
}

// multiply large int with regular int, digit is int, not char type
// least significant digit at left, little endian
template<typename RandRevIter, typename T>
T large_int_multiply(RandRevIter first1, RandRevIter last1, RandRevIter dest, T factor)
{
	T carry = 0;
	while (first1 != last1) {
		carry += *first1  * factor;
		*dest = carry % 10;
		carry /= 10;
		++first1;
		++dest;
	}
	return carry;
}

template<typename T>
void larget_int_fill(std::vector<char>& large_int, T init)
{
	while (init>0) {
		large_int.push_back(init % 10);
		init /= 10;
	}
}
template<typename T>
T large_int_get(std::vector<char>& large_int)  // little endian
{    // compute from back
	return accumulate(rbegin(large_int), rend(large_int), T(), [](T t, char c) { return t * 10 + c; });
}

template<typename T>
void large_int_multiply(std::vector<char>& product, T factor)
{
	T carry = large_int_multiply(begin(product), end(product), begin(product), factor);
	larget_int_fill(product, carry);
}