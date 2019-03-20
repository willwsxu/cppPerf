#pragma once
#include "large_int_basic.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <cassert>

using std::vector;
using std::transform;

//template<typename DigitType=char, int scale=10>
class LargeInt
{
public:
	using DigitType = int;
	static const int scale = 1000000000;
	friend LargeInt operator+(LargeInt lhs, LargeInt rhs)
	{
		lhs += rhs;
		return lhs;
	}
	LargeInt& operator+=(LargeInt& rhs)  // or overload LargeInt& operator+=(LargeInt rhs)  
	{
		sum(this->li, rhs.li, scale);
		return *this;
	}
	LargeInt& operator+=(LargeInt&& rhs)
	{
		sum(this->li, rhs.li, scale);
		return *this;
	}

	LargeInt& operator-=(const LargeInt& rhs)  // this - rh2
	{
		assert(li.size() >= rhs.li.size());
		int borrow = 0;
		transform(begin(rhs.li), end(rhs.li), begin(li), begin(li), [&borrow](DigitType c1, DigitType c2) {
			int sum = c2 - c1 - borrow;
			if (sum < 0) {
				borrow = 1;
				sum += scale;
			}
			else
				borrow = 0;
			return static_cast<DigitType>(sum);
		});
		size_t short_size = rhs.li.size();
		if (borrow && li.size() > short_size) {
			auto start = begin(li) + short_size;
			transform(start, end(li), start, [&borrow](DigitType c1) {
				int sum = c1 - borrow;
				if (sum < 0) {
					borrow = 1;
					sum += scale;
				}
				else
					borrow = 0;
				return static_cast<DigitType>(sum);
			});
		}
		assert(borrow == 0);
		return *this;
	}

	friend LargeInt operator*(const LargeInt& lhs, const LargeInt& rhs);
	
	LargeInt(int n, size_t capacity=5)
	{
		li.reserve(capacity);
		large_int_fill(li, n, scale);
	}
	std::string get() {
		if (li.empty())
			return std::string();
		auto get_str=[](auto start, auto last) {
			std::string str;
			while (start != last && *start == 0)  // trim leading 0
				++start;
			if (start == last)
				return std::string("0");
			str.reserve(distance(start, last) + 1);
			std::transform(start, last, back_inserter(str), [](char c) { return c + '0'; });
			return str;
		};
		if (scale == 10)
			return get_str(rbegin(li), rend(li));
		vector<char> char_scale = scale_down(li, 9);
		return get_str(rbegin(char_scale), rend(char_scale));
	}
	LargeInt(LargeInt&& rhs):li(move(rhs.li))
	{
	}
	LargeInt(const LargeInt& rhs) :li(rhs.li)
	{
	}
	LargeInt& operator=(LargeInt&& rhs) {
		li = move(rhs.li);
		return *this;
	}
	void swap(LargeInt& rhs)
	{
		std::swap(li, rhs.li);
	}
	void trim0()
	{
		auto trailing0 = rbegin(li);
		while (trailing0 != rend(li) && *trailing0 == 0)
			++trailing0;
		auto last = trailing0.base();
		if (last == begin(li))
			++last;
		if (last != end(li)) {
			li.erase(last, end(li));
		}
	}
private:
	std::vector<DigitType> li;  // least significant to most significant digit, little endian
	friend void multiply(LargeInt& ans, LargeInt& temp_copy, const LargeInt& lhs, const LargeInt& rhs)
	{
		multiply(ans, temp_copy, begin(lhs.li), end(lhs.li), begin(rhs.li), end(rhs.li));
	}

	template<typename RandIter>
	friend void multiply(LargeInt& ans, LargeInt& temp_copy, RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e)
	{
		ans.li.assign(1, 0);
		int power = 0;
		for (auto y = rhs_s; y != rhs_e; ++y, power++) {
			if (*y > 0) {
				temp_copy.li.assign(power, 0); // add some zero before copy from lhs
				std::copy(lhs_s, lhs_e, back_inserter(temp_copy.li));
				large_int_multiply(temp_copy.li, *y, LargeInt::scale);
				if (ans.li.size() < temp_copy.li.size())
					std::swap(ans.li, temp_copy.li);
				ans += temp_copy;
			}
		}
	}
	template<typename RandIter>
	friend LargeInt multiply1(RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e);

	// faster multiply algorithm, needs test
	template<typename RandIter>
	friend LargeInt add(RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e);
public:
	// divide operands into half, left half (least siginicant) ceil(n/2), right half floor(n/2)
	template<typename RandIter>
	friend LargeInt multiply_fast(RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e);
	friend LargeInt multiply_fast(LargeInt& lhs, LargeInt& rhs);
};

template<typename RandIter>
LargeInt add(RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e)
{
	size_t left_size = distance(lhs_s, lhs_e);
	size_t right_size = distance(rhs_s, rhs_e);
	size_t size = max(left_size, right_size);
	LargeInt ans(0, size + 1);
	ans.li.clear();
	long long carry = 0;
	if (left_size < right_size) {
		copy(lhs_s, lhs_e, back_inserter(ans.li));
		ans.li.resize(right_size, 0);  // extend short one to equal
	}
	else {
		copy(rhs_s, rhs_e, back_inserter(ans.li));
		if (left_size > right_size)
			ans.li.resize(left_size, 0);
		carry = sum_int(lhs_s, lhs_e, begin(ans.li), begin(ans.li), 0, LargeInt::scale);
	}
	if (carry)
		large_int_fill(ans.li, carry, LargeInt::scale);
	return ans;
}

template<typename RandIter>
LargeInt multiply1(RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e)
{
	LargeInt ans(0);
	LargeInt temp_copy(0);
	const size_t out_size = distance(lhs_s, lhs_e) + distance(rhs_s, rhs_e) + 5;
	temp_copy.li.reserve(out_size);
	ans.li.reserve(out_size);
	multiply(ans, temp_copy, lhs_s, lhs_e, rhs_s, rhs_e);
	return ans;
}
inline LargeInt operator*(const LargeInt& lhs, const LargeInt& rhs)
{
	//std::cout << "lhs size" << lhs.li.size() << " ans size" << ans.li.size() << "\n";
	return multiply1(begin(lhs.li), end(lhs.li), begin(rhs.li), end(rhs.li));
}

// XY = 10^2ceil(n/2) XrYr + 10^ceil(n/2) * [(Xl + Xr)(Yl + Yr) - XlYl - XrYr] + XlYl
template<typename RandIter>
LargeInt multiply_fast(RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e)
{
	size_t size_l = distance(lhs_s, lhs_e);
	size_t size_r = distance(rhs_s, rhs_e);
	assert(size_l == size_r);
	if (size_l < 10) {
		return multiply1(lhs_s, lhs_e, rhs_s, rhs_e);
	}
	size_t right_half = size_r / 2;
	size_t left_half = size_r - right_half;  // left_half >= right_half
	auto XrYr = multiply_fast(lhs_s + left_half, lhs_e, rhs_s + left_half, rhs_e);  // first term in formula
	auto XlYl = multiply_fast(lhs_s, lhs_s + left_half, rhs_s, rhs_s + left_half);  // last term in formula

	LargeInt ans(0, (size_l + 1) << 1);
	ans.li.assign(left_half << 1, 0);
	copy(begin(XrYr.li), end(XrYr.li), back_inserter(ans.li));
	ans += XlYl;  // ans = 10^n XrYr+XlYl

	auto XlXr = add(lhs_s, lhs_s + left_half, lhs_s + left_half, lhs_e);
	auto YlYr = add(rhs_s, rhs_s + left_half, rhs_s + left_half, rhs_e);
	auto mid_part=multiply_fast(XlXr, YlYr);
	XlYl.trim0();
	XrYr.trim0();
	mid_part -= XlYl;
	mid_part -= XrYr;
	mid_part.trim0();

	XrYr.li.assign(left_half, 0);  // reuse XrYr as storage
	copy(begin(mid_part.li), end(mid_part.li), back_inserter(XrYr.li));
	ans += XrYr;
	return ans;
}

inline LargeInt multiply_fast(LargeInt& lhs, LargeInt& rhs)
{
	if (lhs.li.size() < rhs.li.size())
		fill_n(back_inserter(lhs.li), rhs.li.size() - lhs.li.size(), 0);
	else if (lhs.li.size() > rhs.li.size())
		fill_n(back_inserter(rhs.li), lhs.li.size() - rhs.li.size(), 0);
	return multiply_fast(begin(lhs.li), end(lhs.li), begin(rhs.li), end(rhs.li));
}
