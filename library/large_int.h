#pragma once
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <cassert>

using std::vector;
using std::transform;

class LargeInt
{
public:
	using DigitType = char;
	friend LargeInt operator+(LargeInt lhs, LargeInt rhs)
	{
		if (lhs.li.size() < rhs.li.size()) {
			rhs += lhs;
			return rhs;
		}
		lhs += rhs;
		return lhs;
	}
	LargeInt& operator+=(const LargeInt& rhs)
	{
		return add(begin(rhs.li), end(rhs.li));
	}
	template<typename RandIter>
	LargeInt& add(RandIter rhs_s, RandIter rhs_e)
	{
		if (li.size() < (size_t)distance(rhs_s, rhs_e))
			assert(li.size() >= (size_t)distance(rhs_s, rhs_e));
		int carry = 0;
		transform(rhs_s, rhs_e, begin(li), begin(li), [&carry](DigitType c1, DigitType c2) {
			int sum = c1 + c2 + carry;
			carry = sum / 10;
			return static_cast<DigitType>(sum % 10);
		});
		size_t short_size = distance(rhs_s, rhs_e);
		if (carry && li.size() > short_size) {
			auto start = begin(li) + short_size;
			transform(start, end(li), start, [&carry](DigitType c1) {
				int sum = c1 + carry;
				carry = sum / 10;
				return static_cast<DigitType>(sum % 10);
			});
		}
		if (carry)
			li.push_back(static_cast<DigitType>(carry));
		return *this;
	}

	template<typename RandIter>
	friend LargeInt add(RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e);

	LargeInt& operator-=(const LargeInt& rhs)  // this - rh2
	{
		if (li.size() < rhs.li.size())
			assert(li.size() >= rhs.li.size());
		int borrow = 0;
		transform(begin(rhs.li), end(rhs.li), begin(li), begin(li), [&borrow](DigitType c1, DigitType c2) {
			int sum = c2 - c1 - borrow;
			if (sum < 0) {
				borrow = 1;
				sum += 10;
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
					sum += 10;
				}
				else
					borrow = 0;
				return static_cast<DigitType>(sum);
			});
		}
		if (borrow>0)
			assert(borrow == 0);
		return *this;
	}

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
				temp_copy *= (*y);
				if (ans.li.size() < temp_copy.li.size())
					std::swap(ans.li, temp_copy.li);
				ans += temp_copy;
			}
		}
	}
	template<typename RandIter>
	friend LargeInt multiply1(RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e);
	friend LargeInt operator*(const LargeInt& lhs, const LargeInt& rhs);

	// divide operands into half, left half (least siginicant) ceil(n/2), right half floor(n/2)
	template<typename RandIter>
	friend LargeInt multiply_fast(RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e);
	friend LargeInt multiply_fast(LargeInt& lhs, LargeInt& rhs);

	LargeInt(int n, int capacity=5)
	{
		li.reserve(capacity);
		if (n == 0) {
			li.push_back(static_cast<DigitType>(0));
			return;
		}
		while (n > 0) {
			li.push_back(static_cast<DigitType>(n % 10));
			n /= 10;
		}
	}
	std::string get() const {
		std::string str;
		str.reserve(li.size() + 1);
		auto start = rbegin(li);
		while (*start == 0)  // trim leading 0
			++start;
		std::transform(start, rend(li), back_inserter(str), [](char c) { return c + '0'; });
		return str;
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
private:
	LargeInt& operator*=(int rhs)
	{
		int carry = 0;
		std::transform(begin(li), end(li), begin(li), [rhs, &carry](DigitType n) {
			int multi = n*rhs + carry;
			carry = multi / 10;
			return static_cast<DigitType>(multi % 10);
		});
		if (carry > 0)
			li.push_back(static_cast<DigitType>(carry));
		return *this;
	}
	std::vector<DigitType> li;  // least significant to most significant digit
};

template<typename RandIter>
LargeInt multiply1(RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e)
{
	LargeInt ans(0);
	LargeInt temp_copy(0);
	const int out_size = distance(lhs_s, lhs_e) + distance(rhs_s, rhs_e) + 5;
	temp_copy.li.reserve(out_size);
	ans.li.reserve(out_size);
	multiply(ans, temp_copy, lhs_s, lhs_e, rhs_s, rhs_e);
	return ans;
}
LargeInt operator*(const LargeInt& lhs, const LargeInt& rhs)
{
	//std::cout << "lhs size" << lhs.li.size() << " ans size" << ans.li.size() << "\n";
	return multiply1(begin(lhs.li), end(lhs.li), begin(rhs.li), end(rhs.li));
}

// XY = 10^2ceil(n/2) XrYr + 10^ceil(n/2) * [(Xl + Xr)(Yl + Yr) - XlYl - XrYr] + XlYl
template<typename RandIter>
LargeInt multiply_fast(RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e)
{
	int size_l = distance(lhs_s, lhs_e);
	int size_r = distance(rhs_s, rhs_e);
	assert(size_l == size_r);
	if (size_l < 10) {
		return multiply1(lhs_s, lhs_e, rhs_s, rhs_e);
	}
	int right_half = size_r / 2;
	int left_half = size_r - right_half;
	auto XrYr = multiply_fast(lhs_s + left_half, lhs_e, rhs_s + left_half, rhs_e);  // first term in formula
	auto XlYl = multiply_fast(lhs_s, lhs_s + left_half, rhs_s, rhs_s + left_half);  // last term in formula

	LargeInt ans(0, (size_l + 1) << 1);
	ans.li.assign(left_half << 1, 0);
	copy(begin(XrYr.li), end(XrYr.li), back_inserter(ans.li));
	ans += XlYl;  // ans = 10^n XrYr+XlYl

	auto XlXr = add(lhs_s, lhs_s + left_half, lhs_s + left_half, lhs_e);
	auto YlYr = add(rhs_s, rhs_s + left_half, rhs_s + left_half, rhs_e);
	auto mid_part=multiply_fast(XlXr, YlYr);
	mid_part -= XlYl;
	mid_part -= XrYr;

	XrYr.li.assign(left_half, 0);  // reuse XrYr as storage
	copy(begin(mid_part.li), end(mid_part.li), back_inserter(XrYr.li));
	ans += XrYr;
	return ans;
}

LargeInt multiply_fast(LargeInt& lhs, LargeInt& rhs)
{
	if (lhs.li.size() < rhs.li.size())
		fill_n(back_inserter(lhs.li), rhs.li.size() - lhs.li.size(), 0);
	else if (lhs.li.size() > rhs.li.size())
		fill_n(back_inserter(rhs.li), lhs.li.size() - rhs.li.size(), 0);
	return multiply_fast(begin(lhs.li), end(lhs.li), begin(rhs.li), end(rhs.li));
}

template<typename RandIter>
LargeInt add(RandIter lhs_s, RandIter lhs_e, RandIter rhs_s, RandIter rhs_e)
{
	size_t left_size = distance(lhs_s, lhs_e);
	size_t right_size = distance(rhs_s, rhs_e);
	size_t size = max(left_size, right_size);
	LargeInt ans(0, size + 1);
	ans.li.clear();
	if (left_size < right_size) {
		copy(rhs_s, rhs_e, back_inserter(ans.li));
		ans.add(lhs_s, lhs_e);
	}
	else {
		copy(lhs_s, lhs_e, back_inserter(ans.li));
		ans.add(rhs_s, rhs_e);
	}
	return ans;
}