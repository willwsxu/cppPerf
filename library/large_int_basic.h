#pragma once
#include <vector>

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
template<typename RandRevIter>
int large_int_multiply(RandRevIter first1, RandRevIter last1, RandRevIter dest, int factor)
{
	int carry = 0;
	while (first1 != last1) {
		carry += *first1  * factor;
		*dest = carry % 10;
		carry /= 10;
		++first1;
		++dest;
	}
	return carry;
}
