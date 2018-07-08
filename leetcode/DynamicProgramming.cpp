#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

class StringDp
{

	vector<vector<int>> dp;
	int minDistance(const string& word1, const string& word2, int pos1, int pos2) {
		if (pos1 == word1.size())
			return word2.size() - pos2;
		if (pos2 == word2.size())
			return word1.size() - pos1;
		if (dp[pos1][pos2] >= 0)
			return dp[pos1][pos2];
		if (word1[pos1] == word2[pos2])
			dp[pos1][pos2] = minDistance(word1, word2, pos1 + 1, pos2 + 1);
		else
			dp[pos1][pos2] = min(1 + minDistance(word1, word2, pos1 + 1, pos2), 1 + minDistance(word1, word2, pos1, pos2 + 1));
		return dp[pos1][pos2];
	}
public:
	// 583. Delete Operation for Two Strings, minimum number of steps required to make word1 and word2 the same
	int minDistance(string word1, string word2) { // beat 99%
		dp = vector<vector<int>>(word1.size(), vector<int>(word2.size(), int(-1)));
		return minDistance(word1, word2, 0, 0);
	}
};


TEST_CASE("delete char to make string same", "[DP]")
{
	StringDp s;
	CHECK(s.minDistance("sea", "eat") == 2);
}