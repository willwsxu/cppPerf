#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

int getNext(int n)
{
	int ans = n - 1;
	for (int k = 2; k*k <= n; k++)
		if (n%k == 0)
			ans = n / k;
	return ans;
}

static int computed = 0;
static vector<int> dp(1000001, 0);
static void compute(int n)
{
	if (n <= computed)
		return;
	while (++computed <= n) {
		dp[computed] = dp[computed - 1] + 1;
		for (int k = 2; k*k <= computed; k++)
			if (n%k == 0)
				dp[computed] = min(dp[computed], dp[n / k]);
	}
}
/*
* Complete the downToZero function below.
*/
int downToZero(int n) { // 0<=N<=10^6
						/*
						* Write your code here.
						*/
	if (n<4)
		return n;
	int step = 0;
	while (n>3) {
		n = getNext(n);
		step++;
	}
	return step + n;
}

TEST_CASE("Hackerrank Down to Zero II", "[NEW]")
{
	CHECK(downToZero(3) == 3);
	CHECK(downToZero(4) == 3);
	CHECK(downToZero(999997) == 8);
}