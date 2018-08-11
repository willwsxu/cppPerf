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
