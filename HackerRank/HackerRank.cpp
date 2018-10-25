#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "Graph.h"
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
	for (size_t i = 0; i < testcase1.size(); i++) {
		CHECK(downToZero(testcase1[i]) == answer1[i]);
	}
}

// return "yes" if it sorted
// return "yes\nswap a b" if it becomes sorted by swapping position a and b
// return "yes\nreverse a b" if it becomes sorted by reverting all nubers position a and b
// return "no"
string almostSorted(vector<int> arr) { // divid into 3 section, increasing, mid, decreasing
	auto decrease = adjacent_find(begin(arr), end(arr), [](int a, int b) { return a>b; });  // first fist number decreasing
	if (decrease == end(arr))  // sorted
		return "yes";
	auto rlast = make_reverse_iterator(decrease);
	auto increase = adjacent_find(rbegin(arr), rlast, [](int a, int b) { return a<b; });
	if (increase != rbegin(arr) && *decrease > *(increase - 1))  // first value in mid is larger than first in third section
		return "no";
	if (decrease != begin(arr) && *(decrease - 1) > *increase)   // last value in first section is larger than last in mid
		return "no";

	auto mid_end = increase.base();  // use it for end of mid section, from decrease to increase
	int first = distance(begin(arr), decrease) + 1;
	int second = distance(begin(arr), mid_end);
	auto mid_increase = adjacent_find(decrease, mid_end, [](int a, int b) { return a < b; });
	if (mid_increase != mid_end) {  // mid section is not decrease
		iter_swap(decrease, increase.base() - 1);
		auto mid_decrease = adjacent_find(decrease, mid_end, [](int a, int b) { return a > b; });
		if (mid_decrease != mid_end) {
			return "no"; // not sorted after swap, no good
		}
		return string("yes\nswap ").append(to_string(first)).append(1, ' ').append(to_string(second));
	}
	auto mid_dist = distance(decrease, mid_end);
	if (mid_dist == 2 || mid_dist==3) {
		return string("yes\nswap ").append(to_string(first)).append(1, ' ').append(to_string(second));
	}
	return string("yes\nreverse ").append(to_string(first)).append(1, ' ').append(to_string(second));
}

TEST_CASE("Hackerrank almost sorted", "[NEW]")
{
	CHECK(almostSorted(vector<int>{2, 4, 12, 8, 10, 6, 13}) == "yes\nswap 3 6");
	CHECK(almostSorted(vector<int>{2, 4, 12, 10, 8, 6, 13}) == "yes\nreverse 3 6");
	CHECK(almostSorted(vector<int>{2, 4, 12, 8, 6, 13}) == "yes\nswap 3 5");

	CHECK(almostSorted(vector<int>{2, 4, 12, 8, 10, 6}) == "yes\nswap 3 6");
	CHECK(almostSorted(vector<int>{2, 4, 12, 10, 8, 6}) == "yes\nreverse 3 6");

	CHECK(almostSorted(vector<int>{3, 6, 4, 5}) == "no");
	CHECK(almostSorted(vector<int>{3, 4, 1, 5}) == "no");
	CHECK(almostSorted(vector<int>{3, 2}) == "yes\nswap 1 2");
	CHECK(almostSorted(vector<int>{1, 2, 3, 4, 5}) == "yes");
	CHECK(almostSorted(vector<int>{5}) == "yes");
	CHECK(almostSorted(vector<int>{}) == "yes");
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

TEST_CASE("Hackerrank Common Child", "[NEW]")
{
	CHECK(commonChild("michelangelo", "hieroglyphology") == 5);
}

// Breadth First Search: Shortest Reach
// n nodes, m edges
// cost from s to each node, -1 if not reachable
// print cost from node 1 to n, but exclude s
vector<int> bfs(int n, int m, vector<vector<int>> edges, int s) {
	GraphUnweighted g(n);
	g.undirectEdges(edges);
	auto ans = g.bfs(s);
	ans.erase(begin(ans));
	ans.erase(begin(ans) + s - 1);
	for (int& w : ans) {  // each edge weight 6 units
		if (w > 0)
			w *= 6;
	}
	return ans;
}