#include "..\catch.hpp"  // don't put this file in stdafx.h

#include <map>
#include <set>
#include <unordered_set>
#include <numeric>
#include "myalgo.h"
#include "helper.h"
#include "UnionFind.h"
#include "large_int.h"
#include "digits-helper.h"
using namespace std;

// Project Euler #30: Digit Nth powers 
// number of k digits = sum of digit to nth power
class DigitPower
{
	vector<vector<int>> powers;  // pre compute powers of digit 2 to 9, rows
	set<int> ans_sum;
	void RecurseDigitPower(int N, int start, vector<int>& digits, int sum) {  // iterate all choices of 2 to 7 digit out of 10
		if (digits.size() > 1) { // at least 2 digit number
			int sum_copy = sum;
			vector<int> digits_of_sum{};
			while (sum_copy > 0) {
				digits_of_sum.push_back(sum_copy % 10);
				sum_copy /= 10;
			}
			if (digits_of_sum.size() == digits.size()) {
				vector<int> acopy(digits);
				sort(begin(acopy), end(acopy));
				sort(begin(digits_of_sum), end(digits_of_sum));
				if (digits_of_sum == acopy) {
					ans_sum.insert(sum);
					return;
				}
			}
			if (digits.size() > 7)  // 8*9^6=7 digits, so no need to go beyond 7 digit number
				return;
		}
		for (int digit = start; digit < 10; digit++) {
			digits.push_back(digit);
			RecurseDigitPower(N, digit, digits, sum + powers[digit][N]);
			digits.pop_back();
		}
	}
public:
	DigitPower(int p) : powers(10, vector<int>(p + 1, 1))  // p is max power
	{
		fill(begin(powers[0]), end(powers[0]), 0);
		for (int base = 2; base < (int)powers.size(); base++) {
			for (int j = 1; j <= p; j++) {
				powers[base][j] = powers[base][j - 1] * base;
			}
		}
	}
	int sum_digit_power(int N) {
		ans_sum.clear();
		vector<int> digits{};
		RecurseDigitPower(N, 0, digits, 0);
		return accumulate(begin(ans_sum), end(ans_sum), 0);
	}
};
TEST_CASE("Project Euler #30: Digit Nth powers", "[NEW]")
{
	DigitPower dpow(6);
	CHECK(dpow.sum_digit_power(5) == 553839);
	CHECK(dpow.sum_digit_power(3) == 1301);
	CHECK(dpow.sum_digit_power(4) == 19316);
	CHECK(dpow.sum_digit_power(6) == 548834);
}