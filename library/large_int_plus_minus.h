#pragma once
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <cassert>

using std::vector;
using std::transform;

#include "large_int_basic.h"
#include "mathlib.h"

class LargeIntBasic
{
public:
	using DigitType = char;
	friend LargeIntBasic operator+(LargeIntBasic lhs, LargeIntBasic rhs)
	{
		sum(lhs.li, rhs.li);
		return lhs;
	}
	LargeIntBasic& operator+=(LargeIntBasic& rhs)
	{
		sum(this->li, rhs.li);
		return *this;
	}

	LargeIntBasic(int n, int capacity = 5)
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
		if (li.empty())
			return "";
		std::string str;
		str.reserve(li.size() + 1);
		auto start = rbegin(li);
		while (start != rend(li) && *start == 0)  // trim leading 0
			++start;
		if (start == rend(li))
			return "0";
		std::transform(start, rend(li), back_inserter(str), [](char c) { return c + '0'; });
		return str;
	}
	LargeIntBasic(LargeIntBasic&& rhs) :li(move(rhs.li))
	{
	}
	LargeIntBasic(const LargeIntBasic& rhs) :li(rhs.li)
	{
	}
	LargeIntBasic& operator=(LargeIntBasic&& rhs) {
		li = move(rhs.li);
		return *this;
	}
	void swap(LargeIntBasic& rhs)
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
	void reverse()
	{
		std::reverse(begin(li), end(li));
	}
	LargeIntBasic reverse_copy() const
	{
		LargeIntBasic copy(*this);
		std::reverse(begin(copy.li), end(copy.li));
		return copy;
	}
	bool is_palindrome() const {
		return isPalindrome(begin(li), end(li));
	}
	friend bool operator<(const LargeIntBasic& lhs, const LargeIntBasic& rhs) {
		if (lhs.li.size()<rhs.li.size())
			return true;
		if (lhs.li.size()>rhs.li.size())
			return false;
		return lhs.li<rhs.li;
	}
	friend bool operator==(const LargeIntBasic& lhs, const LargeIntBasic& rhs) {
		return lhs.li == rhs.li;
	}
private:
	std::vector<DigitType> li;  // least significant to most significant digit, little endian
};