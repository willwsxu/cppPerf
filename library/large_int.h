#pragma once
#include <vector>
#include <algorithm>
#include <iterator>

using std::vector;
using std::transform;

class LargeInt
{
public:
	friend LargeInt operator+(LargeInt lhs, LargeInt rhs)
	{
		if (lhs.li.size() < rhs.li.size()) {
			rhs += lhs;
			return rhs;
		}
		lhs += rhs;
		return lhs;
	}
	LargeInt& operator+=(LargeInt& rhs)  // rhs could be modified inside the method, assyme lhs is longer than rhs
	{
		vector<int> *pLong = &li;
		vector<int> *pShort = &rhs.li;
		int carry = 0;
		transform(pShort->begin(), pShort->end(), pLong->begin(), pLong->begin(), [&carry](int c1, int c2) {
			int sum = c1 + c2 + carry;
			carry = sum / 10;
			return sum % 10;
		});
		if (carry && pLong->size() > pShort->size()) {
			auto start = pLong->begin() + pShort->size();
			transform(start, pLong->end(), start, [&carry](int c1) {
				int sum = c1 + carry;
				carry = sum / 10;
				return sum % 10;
			});
		}
		if (carry)
			pLong->push_back(carry);
		return *this;
	}
	friend LargeInt operator*(const LargeInt& lhs, const LargeInt& rhs)
	{
		LargeInt ans(0);
		int shift = 0;
		LargeInt temp_copy(0);
		temp_copy.li.reserve(lhs.li.size()*rhs.li.size() + 5);
		ans.li.reserve(lhs.li.size()*rhs.li.size() + 5);
		for (int d : rhs.li) {
			temp_copy.li.assign(shift++, 0); // add some zero before copy from lhs
			std::copy(begin(lhs.li), end(lhs.li), back_inserter(temp_copy.li));
			temp_copy *= d;
			if (ans.li.size() < temp_copy.li.size())
				std::swap(ans.li, temp_copy.li);
			ans += temp_copy;
		}
		return ans;
	}

	LargeInt(int n)
	{
		if (n == 0) {
			li.push_back(0);
			return;
		}
		while (n > 0) {
			li.push_back(n % 10);
			n /= 10;
		}
	}
	vector<int> get() const {
		vector<int> x = li;
		std::reverse(begin(x), end(x));
		return x;
	}
	LargeInt(LargeInt&& rhs):li(move(rhs.li))
	{
	}
	LargeInt(const LargeInt& rhs) :li(rhs.li)
	{
	}
	LargeInt(const LargeInt& rhs, int append0):li(append0,0)
	{
		li.reserve(rhs.li.size() + append0);
		copy(begin(rhs.li), end(rhs.li), std::back_inserter(li));
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
		std::transform(begin(li), end(li), begin(li), [rhs, &carry](int n) {
			int multi = n*rhs + carry;
			carry = multi / 10;
			return multi % 10;
		});
		if (carry > 0)
			li.push_back(carry);
		return *this;
	}
	std::vector<int> li;  // least significant to most significant digit
};