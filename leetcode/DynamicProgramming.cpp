#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

std::unique_ptr<int[]> dp;
int combinationSum4Dp(vector<int>& nums, int target) {
	if (dp[target] >= 0)
		return dp[target];
	int total = 0;
	for (auto val : nums) {
		if (target >= val)
			total += combinationSum4Dp(nums, target - val);
	}
	dp[target] = total;
	return total;
}
// 377. Combination Sum IV
int combinationSum4(vector<int>& nums, int target) {
	dp = make_unique<int[]>(target + 1);
	std::fill(dp.get(), dp.get() + target + 1, -1);
	//for (int i=0; i<=target; i++)
	//    dp[i]=-1;
	dp[0] = 1;
	return combinationSum4Dp(nums, target);
}


// 416. Partition Equal Subset Sum
bool canPartition(vector<int>& nums) {
	int sum = 0;
	for (int n : nums)
		sum += n;
	if ((sum & 1) > 0)  // odd sum
		return false;
	sum /= 2;
	vector<bool> dp(sum + 1, false);
	dp[0] = true;
	for (int n : nums) {
		if (sum >= n) {
			dp[sum] = dp[sum] || dp[sum - n];
			if (dp[sum])
				return true;
		}
		for (int j = sum - 1; j > 0; j--) {
			if (j >= n)
				dp[j] = dp[j] || dp[j - n];
		}
	}
	return dp[sum];
}


int findPaths(int m, int n, int N, int i, int j) {
	vector < vector<vector<int>>> dp3(2, vector<vector<int>>(m, vector<int>(n, 0)));
	const int MOD = 1000000007;
	int prev = 0;
	int current = 0;
	for (int x = 1; x <= N; x++)
	{
		current = 1 - prev;
		for (int r = 0; r < m; r++)
		{
			for (int c = 0; c < n; c++)
			{
				dp3[current][r][c] = (r == 0 ? 1 : dp3[prev][r - 1][c]) % MOD;  // up !ERROR don't use i-- etc
				dp3[current][r][c] += r == m - 1 ? 1 : dp3[prev][r + 1][c];  // down
				dp3[current][r][c] %= MOD;
				dp3[current][r][c] += c == 0 ? 1 : dp3[prev][r][c - 1];  // left
				dp3[current][r][c] %= MOD;
				dp3[current][r][c] += c == n - 1 ? 1 : dp3[prev][r][c + 1];  // left
				dp3[current][r][c] %= MOD;
			}
		}
		prev = current; // swap
	}
	return dp3[current][i][j];
}

void testEqualSumPartition()
{
	std::cout << canPartition(vector<int>{}) << endl;
	std::cout << canPartition(vector<int>{1, 5, 11, 5}) << endl;  // true
	std::cout << canPartition(vector<int>{1, 2, 3, 5}) << endl;   // false
}

// prevBuy, last postion of buy, -1 means no
int maxProfit(vector<int>& prices, int pos, int prevBuy, vector<vector<int>>& dp) {
	if (pos >= (int)prices.size()) {
		return 0;
	}
	if (dp[pos][prevBuy + 1] >= 0)
		return dp[pos][prevBuy + 1];
	int ans = maxProfit(prices, pos + 1, prevBuy, dp);  // no action
	if (prevBuy >= 0) {
		if (prices[pos] > prices[prevBuy])   // sell if there is profit
			ans = std::max(ans, (prices[pos] - prices[prevBuy]) + maxProfit(prices, pos + 2, -1, dp));
	}
	else
		ans = std::max(ans, maxProfit(prices, pos + 1, pos, dp));  // buy
	dp[pos][prevBuy + 1] = ans;
	return ans;
}
int maxProfit(vector<int>& prices) {
	int n = prices.size();
	vector<vector<int>> dp(n, vector<int>(n + 1, -1));
	return maxProfit(prices, 0, -1, dp);
}

void testStockBuySell()
{
	std::cout << (maxProfit(vector<int> { 1, 2, 3, 0, 2 }) == 3) << endl;
	std::cout << (maxProfit(vector<int> { 2, 1}) == 0) << endl;
}

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

class CoinChange
{
	int coinChange(vector<int>& coins, vector<int>& dp, int amount) {// beat 17%
		if (amount == 0)
			return 0;
		int count = INT_MAX / 2;
		if (amount < 0)
			return count;
		if (dp[amount] >= 0)
			return dp[amount];

		for (int i = 0; i < coins.size(); i++) {
			count = min(count, 1 + coinChange(coins, dp,  amount - coins[i]));
		}
		dp[amount] = count;
		return count;
	}

public:
	// 322. Coin Change, minimal coins
	int coinChange(vector<int>& coins, int amount) {
		vector<int> dp(amount + 1, -1);
		int ans = coinChange(coins, dp, amount);
		return ans >= INT_MAX / 2 ? -1 : ans;
	}
};


TEST_CASE("coin change minimal", "[NEW]")
{
	CHECK(CoinChange().coinChange(vector<int>{2, 5, 6}, 10) == 2);
	CHECK(CoinChange().coinChange(vector<int>{5, 6}, 13) == -1);
}