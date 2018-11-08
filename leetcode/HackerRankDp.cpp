#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h
#include "myalgo.h"
using namespace std;

// coin change
long long getWays(long money, vector<long>& coin_types) {
	if (money == 0)
		return 1;
	if (money<0)
		return -1;
	long long total = 0;
	for (int c_t : coin_types) {
		long long w = getWays(money - c_t, coin_types);
		if (w>0)
			total += w;
	}
	return total;
}


TEST_CASE("coin change ways", "[NEW]")
{
	//CHECK(getWays(4, vector<long>{1, 2, 3}) == 4);
}

// subproblem, suffix of 2 strings
// LCS, DP
int commonChild(const string& s1, const string& s2, int i, int j, vector<vector<int>>& dp) {
	if (i == s1.size() || j == s2.size())
		return 0;
	if (dp[i][j] >= 0)
		return dp[i][j];
	if (s1[i] == s2[j])
		dp[i][j] = 1 + commonChild(s1, s2, i + 1, j + 1, dp);
	else
		dp[i][j] = max(commonChild(s1, s2, i, j + 1, dp), commonChild(s1, s2, i + 1, j, dp));
	return dp[i][j];
}
// longest common child of s1 and s2
int commonChild(string s1, string s2) {
	vector<vector<int>> dp(s1.size(), vector<int>(s2.size(), -1));
	int ans = commonChild(s1, s2, 0, 0, dp);
	for (const auto& v : dp) {
		copy(begin(v), end(v), ostream_iterator<int>(cout, " "));
		cout << "\n";
	}
	return ans;
}

TEST_CASE("Hackerrank string DP Common Child", "[NEW]")
{
	CHECK(commonChild("michelangelo", "hieroglyphology") == 5);
}

