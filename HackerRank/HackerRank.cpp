#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

static vector<int> dp(1000001, 0);
static int compute(int n)
{
	if (dp[n]>0 || n==0)
		return dp[n];
	if (n < 560000) {
		for (int i = 1; i <= n; i++) {
			dp[i] = dp[i - 1]+1;
			for (int k = 2; k*k <= i; k++) {
				if (i%k == 0)  // find factor k*b=
					dp[i] = min(dp[i], dp[i / k] + 1);
			}
		}
		return dp[n];
	}

	int ans = INT32_MAX;
	for (int k = 2; k*k <= n; k++)
		if (n%k == 0)  // find factor k*b=
			ans = min(ans, compute(n / k)+1);
	if (ans == INT32_MAX || n<560000)
		ans = min(ans, compute(n - 1)+1);  // assume this step is the worst choice
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