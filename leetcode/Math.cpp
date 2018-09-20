#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
#include "helper.h"
using namespace std;

class Math
{

public:
	// 453. Minimum Moves to Equal Array Elements
	// each move increment all n-1 elements by 1, find min to make all equal
	// sum+(n-1)*m = x * n, minNum should be incremented in all m moves, so final value x=minNum+m
	// sum - m = n* minNUm, m=sum - n*minNum
	int minMoves(vector<int>& nums) {  // borrow ideas, beat 97%
		auto minNum = min_element(nums.begin(), nums.end());
		auto sum = accumulate(nums.begin(), nums.end(), 0);
		return sum - nums.size()*(*minNum);
	}

	// 462. Minimum Moves to Equal Array Elements II
	// find the minimum number of moves required to make all array elements equal, where a move is incrementing or decrementing a selected element by 1
	// borrowed idea target number is median
	// 2 2 2 3 6
	// 2 2 3 6
	int minMoves2(vector<int>& nums) { // beat 98%
		if (nums.size() < 2)
			return 0;
		nth_element(nums.begin(), nums.begin() + nums.size() / 2, nums.end());
		int median = *(nums.begin() + nums.size() / 2);
		return accumulate(nums.begin(), nums.end(), 0, [median](size_t a, int n) {
			return a + abs(n - median);
		});
	}

	// 789. Escape The Ghosts, you start from (0,0) and try to reach target. There are some ghosts on board
	// You and ghosts can move 1 step a time, in 4 directions (N S E W)
	// You lose if the ghost reaches you.
	// Borrowed Math idea: You should find the shortest path to target
	// ghost is better to do the same instead of trying to intercept you
	bool escapeGhosts(vector<vector<int>>& ghosts, vector<int>& target) { // beat 97%
		int steps = abs(target[0]) + abs(target[1]);
		for (const auto& g : ghosts) {
			if (abs(g[0] - target[0]) + abs(g[1] - target[1]) <= steps)
				return false;
		}
		return true;
	}

	// 319. Bulb Switcher. n bulbs that are initially off. You first turn on all the bulbs
	// Then, you turn off every second bulb
	// On the third round, you toggle every third bulb (turning on if it's off or turning off if it's on).
	// For the i-th round, you toggle every i bulb
	// For the n-th round, you only toggle the last bulb. Find how many bulbs are on after n rounds
	// Idea: it translates to toggle every ith bulb on ith round. that means ith bulb will toggled m times, m is the total factors of i
	// it is on if it is toggled odd # of times. perfect square number has odd number of factors
	int bulbSwitch(int n) {  // beat 100%
		int count = 1;
		while (count*count <= n)
			count++;
		return count - 1;
	}

	// 672. Bulb Switcher II. Given n room and 4 buttons, after m operations, how many status of n lights
	// operations: Flip all the lights, Flip even lights, Flip odd lights, Flip lights with (3k + 1) numbers
	int flipLights(int n, int m) {  // borrowed fomula, no idea!
		n = min(n, 3);
		return min(1 << n, 1 + m*n);
	}

protected:
	bool isPerfectSquare(int num, int low, int hi) {// watch out for tricky overflow issues
		if (low > hi)
			return false;
		int64_t mid = ((int64_t)low + hi) / 2;
		int64_t sq = mid*mid;
		if (sq == num)
			return true;
		if (sq > num)
			return isPerfectSquare(num, low, (int)mid - 1);
		return isPerfectSquare(num, (int)mid + 1, hi);
	}
public:
	bool isPerfectSquare(int num) {  // binary search, beat 100%
		return isPerfectSquare(num, 1, min((num + 1) / 2, 50000));
	}
	bool judgeSquareSum_old(int c) { // a few gotcha cases, number can be 0, same, watch for overflow, beat 18%
		int sq = 0;
		int ub = min((c + 1) / 2, 50000);
		for (int i = 0; (sq = i*i) <= c && sq >= 0; i++) {
			if (c == sq || isPerfectSquare(c - sq, i, ub))
				return true;
		}
		return false;
	}

	bool judgeSquareSum(int c) { // use hash set to store all perfect square, why it is slower?
		unordered_set<int> squares;
		int sq = 0;
		for (int i = 0; (sq = i*i) <= c && sq >= 0; i++) {
			squares.insert(sq);
			if (squares.count(c - sq))
				return true;
		}
		return false;
	}

	// 67. Add Binary
	string addBinary(string a, string b) {  // beat 100%
		string *pA = &a, *pB = &b;
		if (a.size() < b.size()) {
			pA = &b;  // pA longer than pB
			pB = &a;
		}
		int carry = 0;
		transform(pB->rbegin(), pB->rend(), pA->rbegin(), pA->rbegin(), [&carry](char a, char b) {  // compute common part, from back
			int add = a - '0' + b - '0' + carry;
			carry = add / 2;
			return static_cast<char>(add % 2 + '0');
		});
		if (!carry)
			return *pA;
		if (pA->size() > pB->size()) {  // add carry to longer string
			auto begin = pA->rbegin() + pB->size();
			transform(begin, pA->rend(), begin, [&carry](char a) {
				if (carry == 0)  // just copy
					return a;
				int add = a - '0' + carry;
				carry = add / 2;
				return static_cast<char>(add % 2 + '0');
			});
		}
		if (carry)
			pA->insert(0, 1, '1');
		return *pA;
	}

	// 66. Plus One, Given a non-empty array of digits representing a non-negative integer, plus one to the integer
	vector<int> plusOne(vector<int>& digits) { // similar to #67 above, easier, beat 100%
		int carry = 1;
		vector<int> ans(digits);
		transform(ans.rbegin(), ans.rend(), ans.rbegin(), [&carry](int a) {
			if (carry == 0)  // just copy
				return a;
			int add = a + carry;
			carry = add / 10;
			return add % 10;
		});
		if (carry)
			ans.insert(ans.begin(), carry);
		return ans;
	}

	// 492. Construct the Rectangle, W*L=area, W<=L, W+L is minimal, area <=10^7
	vector<int> constructRectangle(int area) { // greedy method, beat 100%
		int MaxW = static_cast<int>(sqrt(area));
		for (int i = MaxW; i > 1; i--) {
			if (area%i == 0) {
				return{ area / i, i };
			}
		}
		return{ area, 1 };
	}
};


TEST_CASE("789. Escape The Ghosts", "[MATH]")
{
	CHECK(Math().escapeGhosts(vector<vector<int>>{ {1, 9}, { 2, -5 }, { 3, 8 }, { 9, 8 }, { -1, 3 }}, vector<int>{8, -10}) == false);
}

TEST_CASE("319. Bulb Switcher", "[MATH]")
{
	CHECK(Math().bulbSwitch(3) == 1);
	CHECK(Math().bulbSwitch(1001) == 31);
}

struct len_comp {
	bool operator()(int a, int b) const {
		return countDecDigit(a) < countDecDigit(b);
	}
};
long long digitNormalize(int n) {  // convert each digit to power of 10
	static int pow10[] = { 1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000 };
	long long ret = 0;
	while (n) {
		ret += pow10[n % 10];
		n /= 10;
	}
	return ret;
}
class MathPower {
public:
	using intset = std::multiset<int, len_comp>;
	MathPower()
	{
		if (pow2.empty()) {
			for (int i = 0, p = 1; p <= 1000000000; p = 1 << (++i))
				pow2.insert(p);
		}
	}
	// 869. Reordered Power of 2, 1 <= N <= 10^9
	// borrow clever idea: normalize a number to sum of power of 10 for each digit, sum is unique if digits are unique regardless of ordering
	bool reorderedPowerOf2(int N) {  // beat 100%
		auto p2 = pow2.equal_range(N);
		long long target = digitNormalize(N);
		return any_of(p2.first, p2.second, [target](int n) {return target == digitNormalize(n); });
	}
private:
	static intset pow2;
};
MathPower::intset MathPower::pow2;


TEST_CASE("869. Reordered Power of 2", "[POW]")
{
	MathPower p;
	CHECK(p.reorderedPowerOf2(46));
}

TEST_CASE("367. Valid Perfect Square", "[NEW]")
{
	CHECK(Math().isPerfectSquare(1));
	CHECK(Math().isPerfectSquare(808201));
}
TEST_CASE("633. Sum of Square Numbers", "[NEW]")
{
	CHECK(Math().judgeSquareSum(2147483646) == false);
	CHECK(Math().judgeSquareSum(0));
	CHECK(Math().judgeSquareSum(1));
	CHECK(Math().judgeSquareSum(808202));
	CHECK(Math().judgeSquareSum(3) == false);
}


class MathEasy
{
public:
	// 728. Self Dividing Numbers, divisible by all of its digits
	vector<int> selfDividingNumbers(int left, int right) {
		auto good = [](int n) {
			int orig = n;
			while (n > 0) {
				int digit = n % 10;
				if (digit == 0)
					return false;
				if (orig%digit != 0)
					return false;
				n /= 10;
			}
			return true;
		};
		vector<int> ans;
		ans.reserve(200);
		for (int i = left; i <= right; i++) {
			if (good(i))
				ans.push_back(i);
		}
		return ans;
	}
	// 507. Perfect Number, =sum of all its factors, except itself
	bool checkPerfectNumber(int num) {
		if (num <= 1)
			return false;
		int total = 1;
		for (int i = 2; i*i < num; i++) {
			if (num%i == 0) {
				total += (i + num / i);
			}
		}
		return total == num;
	}
};