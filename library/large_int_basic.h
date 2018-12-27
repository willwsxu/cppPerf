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
template<typename RandRevIter, typename T, typename Result_Type=long long>
Result_Type large_int_multiply(RandRevIter first1, RandRevIter last1, RandRevIter dest, T factor, int scale=10)
{
	using Value_Type = std::remove_reference<decltype(*dest)>::type;
	Result_Type carry = 0;
	while (first1 != last1) {
		carry += Result_Type(*first1)  * factor;
		*dest = static_cast<Value_Type>(carry % scale);
		carry /= scale;
		++first1;
		++dest;
	}
	return carry;
}

template<typename T, typename Value_Type=char>
void large_int_fill(std::vector<Value_Type>& large_int, T init, int scale=10)
{
	if (init < 0) {  // allow positive only
		init = -init;
	}
	do {
		large_int.push_back(static_cast<Value_Type>(init % scale));
		init /= scale;
	} while (init > 0);
}
template<typename T>
T large_int_get(std::vector<char>& large_int)  // little endian
{    // compute from back
	return accumulate(rbegin(large_int), rend(large_int), T(), [](T t, char c) { return t * 10 + c; });
}

// each vector element can store one digit or many digits up to 9 (scale =1000000000)
template<typename T, typename ValueType=char>
void large_int_multiply(std::vector<ValueType>& product, T factor, int scale=10)
{
	auto carry = large_int_multiply(begin(product), end(product), begin(product), factor, scale);
	if (carry>0)
		large_int_fill(product, carry, scale);
}

template<typename Value_Type=int>
std::vector<char> scale_down(std::vector<Value_Type>& product, int scale_digits) {
	std::vector<char> dest;
	dest.reserve(product.size() * 8);
	for (Value_Type n : product) {
		if (!dest.empty()) {
			int digits = dest.size() % scale_digits;
			if (digits != 0)
				fill_n(back_inserter(dest), scale_digits - digits, 0);
		}
		large_int_fill(dest, n);
	}
	return dest;
}