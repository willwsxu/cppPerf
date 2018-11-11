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
// LCS, DP, longest common subsequence
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
	//for (const auto& v : dp) {
	//	copy(begin(v), end(v), ostream_iterator<int>(cout, " "));
	//	cout << "\n";
	//}
	return ans;
}

TEST_CASE("Hackerrank string DP Common Child", "[NEW]")
{
	CHECK(commonChild("michelangelo", "hieroglyphology") == 5);
}

template<typename RandIter, typename Inserter>
void longestCommonSubsequence(RandIter startA, RandIter endA, RandIter startB, RandIter endB, Inserter inserter) {
	int sizeA = distance(startA, endA);
	int sizeB = distance(startB, endB);
	vector<vector<int>> dp(sizeA+1, vector<int>(sizeB+1, 0));
	for (int i = sizeA - 1; i >= 0; i--) {
		--endA;
		auto b = endB;
		for (int j = sizeB - 1; j >= 0; j--) {
			if (*endA == *(--b))
				dp[i][j] = 1 + dp[i + 1][j + 1];
			else
				dp[i][j] = max(dp[i + 1][j], dp[i][j + 1]);
		}
	}
	int i = 0;
	int j = 0;
	while (i <sizeA && j < sizeB) {
		if (*(startA + i) == *(startB + j)) {
			*inserter++ = *(startA + i);
			i++;
			j++;
		}
		else if (dp[i + 1][j] > dp[i][j + 1])
			i++;
		else
			j++;
	}
}

vector<int> longestCommonSubsequence(vector<int> a, vector<int> b) {
	if (a.empty() || b.empty())
		return{};
	vector<int> ans;
	longestCommonSubsequence(begin(a),end(a),begin(b),end(b), back_inserter(ans));
	return ans;
}
TEST_CASE("Hackerrank DP longest Common subsequence", "[NEW]")
{
	string s1 = "michelangelo";
	string s2="hieroglyphology";
	string result;
	longestCommonSubsequence(begin(s1), end(s1), begin(s2), end(s2), back_inserter(result));
	CHECK( result== "iello");
	CHECK(longestCommonSubsequence(vector<int>{1,2,3,4,1}, vector<int>{1,3,4,1,2,1,3}) == vector<int>{1,3,4,1});
}

// compute sum of all the substrings function.
// 3842
// 2
// 4, 2, 42=4+40+prev(2)
// 8,4,2,84,42,842=8+80+800+4+2+4+42+42=8+80+800+prev(4+2+42)+prev-prevprev (42+4)
// 3+30+300+3000+prev+8+84+842=3+30+300+3000+prev*2-prevprev = 3(1+10+100+1000)+prev*2-prevprev
int substrings(string n) {
	const int MAX_SIZE = 200000; // n=[1,200000]
	const int MOD = 1000000007;
	vector<long long> pow_mod{ 1 };
	pow_mod.resize(MAX_SIZE);
	for (int i = 1; i < MAX_SIZE; i++)
		pow_mod[i] = pow_mod[i - 1] * 10 % MOD;
	for (int i = 1; i < MAX_SIZE; i++)  // compute prefix sum;
		pow_mod[i] = (pow_mod[i] + pow_mod[i - 1]) % MOD;

	auto digit = rbegin(n);
	vector<long long> dp(3, 0);
	if (!n.empty())
		dp[1] = *digit - '0';
	for (size_t i = 1; i < n.size(); i++) {
		dp[2] = (pow_mod[i] * (*(++digit) - '0') + dp[1] * 2 - dp[0]) % MOD;
		rotate(begin(dp), begin(dp) + 1, end(dp));
	}
	return static_cast<int>(dp[1]);
}

TEST_CASE("Hackerrank DP substring sum", "[NEW]")
{
	CHECK(substrings("0") == 0);
	CHECK(substrings("16") == 23);
	CHECK(substrings("123") == 164);
}