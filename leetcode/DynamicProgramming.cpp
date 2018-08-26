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

		for (int c : coins)
			count = min(count, 1 + coinChange(coins, dp,  amount - c));
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
	int coinChange2(vector<int>& coins, int amount) {// iterative dp, beat 89%
		const int MAX_AMOUNT = amount + 1;
		vector<int> dp(MAX_AMOUNT, 0);
		sort(coins.begin(), coins.end(), greater<>());
		for (int money = 1; money < MAX_AMOUNT; money++) { // find best ans from low to high
			int count = MAX_AMOUNT;
			for (int c : coins) { // for each coin, compute count using previous dp
				if (c <= money)
					count = min(count, 1 + dp[money - c]);
			}
			dp[money] = count;
		}
		return dp[amount] > amount ? -1: dp[amount];
	}

	// 518. Coin Change 2, ways to make change
	int change(int amount, vector<int>& coins, int coin, vector<vector<int>>& dp) {
		if (amount == 0)
			return 1;
		if (coin==coins.size())
			return 0;
		if (dp[amount][coin] >= 0)
			return dp[amount][coin];
		int count = 0;
		for (size_t c = coin; c < coins.size(); c++) { // recursively give change of coins, forward only
			if (amount < coins[c])  // coins are sorted so no need to try bigger ones
				break;
			count += change(amount - coins[c], coins, c, dp);
		}
		dp[amount][coin] = count;
		return count;
	}
	int change(int amount, vector<int>& coins) { // dp with backtracking, beat 13%
		sort(coins.begin(), coins.end());
		vector<vector<int>> dp(amount + 1, vector<int>(coins.size(), -1));
		return change(amount, coins, 0, dp);
	}

	int change2(int amount, vector<int>& coins) { // iterative borrowed idea! beat 64%
		const int MAX_AMOUNT = amount + 1;
		vector<int> dp(MAX_AMOUNT, 0);
		dp[0] = 1;
		for (int c : coins) { // for each coin, compute count using previous dp
			for (int money = c; money < MAX_AMOUNT; money++) { //from low to high
				dp[money] += dp[money - c];
			}
		}
		return dp[amount];
	}
};


TEST_CASE("coin change minimal", "[COIN]")
{
	CHECK(CoinChange().coinChange(vector<int>{2, 5, 6}, 10) == 2);
	CHECK(CoinChange().coinChange(vector<int>{5, 6}, 13) == -1);
	CHECK(CoinChange().coinChange2(vector<int>{2, 5, 6}, 10) == 2);
	CHECK(CoinChange().coinChange2(vector<int>{5, 6}, 13) == -1);
}

TEST_CASE("ways to make coin change", "[COIN]")
{
	CHECK(CoinChange().change2(0, vector<int>{}) == 1);
	CHECK(CoinChange().change2(5, vector<int>{}) == 0);
	CHECK(CoinChange().change2(5, vector<int>{1,2, 5}) == 4);
	CHECK(CoinChange().change2(10, vector<int>{2, 5, 6}) == 3);
	CHECK(CoinChange().change2(13, vector<int>{5, 6}) == 0);
	CHECK(CoinChange().change2(10, vector<int>{10}) == 1);
}

class ClimbStairs
{
	vector<int> dp;
	int climbStairsDp(int n) {  // beat 100%
		if (n <= 1)
			return 1;
		if (dp[n]<0)
			dp[n] = climbStairsDp(n - 1) + climbStairsDp(n - 2);
		return dp[n];
	}
public:
	// 70. Climbing Stairs
	int climbStairs(int n) {
		dp = move(vector<int>(n + 1, -1));
		return climbStairsDp(n);
	}

	// Once you pay the none negative cost, you can either climb one or two steps. 
	// find minimum cost to reach top of floor, and you can either start from step with index 0 or  1
	int minCostClimbingStairs(vector<int>& cost) {

	}
};
TEST_CASE("70. Climbing Stairs", "[NEW]")
{
	CHECK(ClimbStairs().climbStairs(2) == 2);
	CHECK(ClimbStairs().climbStairs(3) == 3);
	CHECK(ClimbStairs().climbStairs(10) == 89);
}
TEST_CASE("746. Min Cost Climbing Stairs", "[NEW]")
{
}

class Game21
{
	double new21Game(int N, int K, int W, int points, vector<double>& dp) {
		if (points >= K) {
			return points <= N ? 1.0 : 0;
		}
		if (dp[points] >= 0)
			return dp[points];
		double ans = 0;
		for (int i = 1; i <= W; i++) {
			ans += 1.0 / W * new21Game(N, K, W, points + i, dp);
		}
		dp[points] = ans;
		return ans;
	}
public:
	// 837. New 21 Game, each card has points [1,W]
	// Alice stops drawing numbers when she gets K or more points.  What is the probability that she has N or less points?
	// 0 <= K <= N <= 10000, 1 <= W <= 10000
	double new21Game(int N, int K, int W) {
		vector<double> dp(K, -1);
		return new21Game(N, K, W, 0, dp);
	}
};


TEST_CASE("837. New 21 Game", "[NEW]")
{
	//CHECK(Game21().new21Game(9811, 8890, 7719) == Approx(1.0).epsilon(.00001));
	CHECK(Game21().new21Game(10, 1, 10) == Approx(1.0).epsilon(.00001));
	CHECK(Game21().new21Game(6, 1, 10) == Approx(0.6).epsilon(.00001));
	CHECK(Game21().new21Game(21, 17, 10) == Approx(0.73278).epsilon(.00001));
}