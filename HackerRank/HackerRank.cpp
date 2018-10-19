#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

static vector<int> dp(1000001, 0);
static int compute(int n)
{
	if (dp[n]>0 || n==0)
		return dp[n];

	int ans = INT32_MAX;
	int factor = 0;
	for (int k = 2; k*k <= n; k++)
		if (n%k == 0) {  // find factor k*b=
			ans = min(ans, compute(n / k) + 1);
			factor++;
		}
	if (ans == INT32_MAX || factor<19)  // try n-1 if n has < 19 pairs of factors. Some tests fail without this
		ans = min(ans, compute(n - 1)+1); // too slow if n-1 is called always
	return dp[n]=ans;
}
/*
* Fast way to count n down to 0: two ways to count: count down 1 at a time, or factor n into a*b, count down to max(a,b)
*/
int downToZero(int n) { // 0<=N<=10^6
	return compute(n);
}

TEST_CASE("Hackerrank Down to Zero II", "[NEW]")
{
	CHECK(downToZero(554548) == 10);
	CHECK(downToZero(1321) == 7);
	CHECK(downToZero(999997) == 8);
	CHECK(downToZero(3) == 3);
	CHECK(downToZero(4) == 3);
	CHECK(downToZero(29) == 7);

	vector<int> testcase1{ 966514, 812849, 808707, 360422, 691410, 691343, 551065, 432560, 192658, 554548 };
	vector<int> answer1{ 8, 10, 8, 11, 9, 11, 9, 9, 10, 10 };
	for (int i = 0; i < testcase1.size(); i++) {
		cout << i << "\n";
		CHECK(downToZero(testcase1[i]) == answer1[i]);
	}
}