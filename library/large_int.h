#pragma once
#include <vector>
#include <algorithm>

using std::vector;
using std::transform;

class LargeInt
{
public:
	friend LargeInt operator*(LargeInt lhs, int rhs)
	{
		int carry = 0;
		std::transform(begin(lhs.li), end(lhs.li), begin(lhs.li), [rhs, &carry](int n) { 
			int multi = n*rhs + carry; 
			carry = multi / 10; 
			return multi % 10;
		});
		if (carry > 0)
			lhs.li.push_back(carry);
		return lhs;
	}
	friend LargeInt operator+(LargeInt lhs, LargeInt rhs)
	{
		vector<int> *pLong = &lhs.li;
		vector<int> *pShort = &rhs.li;
		if (pLong->size() < pShort->size())
			swap(pLong, pShort);
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
		return lhs.li.size()<rhs.li.size()? rhs:lhs;
	}
	friend LargeInt operator*(LargeInt lhs, LargeInt rhs)
	{
		LargeInt ans(0);
		int shift = 0;
		for (int d : rhs.li) {
			LargeInt m = lhs*d;
			if (shift)
				m.li.insert(begin(m.li), shift, 0);
			ans = ans + m;
			shift++;
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
	LargeInt& operator=(LargeInt&& rhs) {
		li = move(rhs.li);
		return *this;
	}
private:
	std::vector<int> li;  // least significant to most significant digit
};