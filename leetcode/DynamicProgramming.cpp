#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h
#include "myalgo.h"
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
class BuyStock
{
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
public:
	int maxProfit(vector<int>& prices) {
		int n = prices.size();
		vector<vector<int>> dp(n, vector<int>(n + 1, -1));
		return maxProfit(prices, 0, -1, dp);
	}

	// 121. Best Time to Buy and Sell Stock, single transaction (1 buy, 1 sell)
	int maxProfit1(vector<int>& prices) { // beat 98%
		if (prices.size() < 2)
			return 0;
		for (int i = 0; i < (int)prices.size() - 1; i++)
			prices[i] = prices[i+1] - prices[i];
		return maxSubSum(prices.begin(), prices.end() - 1, 0);
	}
	// 122. Best Time to Buy and Sell Stock II, many transactions, but only one transaction at a time
	int maxProfit2(vector<int>& prices) { // easy, Gredy, sum up all positive diff prices, beat 98%
		int sum = 0;
		for (int i = 0; i < (int)prices.size() - 1; i++)
			sum += max(prices[i + 1] - prices[i], 0);
		return sum;
	}
};

TEST_CASE("309. Best Time to Buy and Sell Stock with Cooldown", "[NEW]")
{
	CHECK (BuyStock().maxProfit(vector<int> { 1, 2, 3, 0, 2 }) == 3);
	CHECK(BuyStock().maxProfit(vector<int> { 2, 1}) == 0);
	CHECK(BuyStock().maxProfit2(vector<int> { }) == 0);
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
	int climbStairs_topdown(int n, vector<int> dp) {  // beat 100%
		if (n <= 1)
			return 1;
		if (dp[n]<0)
			dp[n] = climbStairs_topdown(n - 1, dp) + climbStairs_topdown(n - 2, dp);
		return dp[n];
	}
public:
	// 70. Climbing Stairs, 1 or 2 steps
	int climbStairs(int n) {  // bottom up dp, beat 100%
		//return climbStairs_topdown(n, vector<int>(n + 1, -1));
		int prev = 1, prev_prev=1;
		for (int step = 2; step <= n; step++) {
			int curr = prev + prev_prev;
			prev_prev = prev;
			prev = curr;
		}
		return prev;
	}

	int minCostClimbingStairs(vector<int>& cost, int idx, vector<int>& dp)
	{
		if (idx < 0)
			return 0;
		int curr = cost[idx];
		if (dp[idx] < 0) {
			int c1 = curr + minCostClimbingStairs(cost, idx - 1, dp);  // on step to idx 
			if (idx > 0)
				c1 = min(c1, curr + minCostClimbingStairs(cost, idx - 2, dp));  //  two steps
			dp[idx] = c1;
		}
		return dp[idx];
	}
	int minCostClimbingStairs(vector<int>& cost) {  // top down dp, beat 40%
		cost.push_back(0);  // fake the end state
		return minCostClimbingStairs(cost, cost.size()-1, vector<int>((int)cost.size(), -1)); // from end state
	}
	// Once you pay the none negative cost, you can either climb one or two steps. 
	// find minimum cost to reach top of floor, and you can either start from step with index 0 or  1
	int minCostClimbingStairs_bottomup(vector<int>& cost) {  // bottom up dp, beat 100%
		int first = 0, second = 0;
		for (int c : cost) {
			c += min(first, second);
			first = second;
			second = c;
		}
		return min(first, second);  // may skip last step
	}
	// 788. Rotated Digits, rotate digit by 180 degree. only 2 and 5, 6 and 9 rotate to each other
	// 0,1,8 rotate to themselves, other digit rotate to invalid
	// given a positive number N, how many numbers X from 1 to N are good?>
	// i.e. a number is good if it consists of some 2,5,6,9 and don't consists of 3,4,7
	int rotatedDigits(int N) {  // beat 38% without dp
		const static set<int> must{ 2,5,6,9 };
		const static set<int> mustNot{ 3,4,7 };
		auto valid = [](int n) {
			bool good = false;
			while (n > 0) {
				int digit = n % 10;
				if (mustNot.count(digit))
					return false;
				if (must.count(digit))
					good = true;
				n /= 10;
			}
			return good;
		};
		int ans = 0;
		vector<char> dp(N + 1, 0); // try dp later!!!!!!!!!!!!!!!!
		for (int i = 1; i <= N; i++) {
			if (valid(i))
				ans++;
		}
		return ans;
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
	CHECK(ClimbStairs().minCostClimbingStairs(vector<int>{1, 0, 0, 0}) == 0);
	CHECK(ClimbStairs().minCostClimbingStairs(vector<int>{1, 100, 1, 1, 1, 100, 1, 1, 100, 1})==6);
	CHECK(ClimbStairs().minCostClimbingStairs(vector<int>{0, 0, 0, 1}) == 0);
}

class SpecialDp
{
public:
	int maxSubArray(vector<int>& nums) {  // beat 100%
		// refactor to a template, slowed 50%???
		return maxSubSum(nums.begin(), nums.end(), INT32_MIN); // in case sum is negative!
	}
};

TEST_CASE("53. Maximum Subarray", "[NEW]")
{
	CHECK(SpecialDp().maxSubArray(vector<int>{-1}) == -1);
}
class NumArray {
	vector<int> nums_;
public:
	NumArray(vector<int> nums):nums_(nums) {
		partial_sum(begin(nums_), end(nums_), begin(nums_));
	}

	int sumRange(int i, int j) {
		if (i>0)
			return nums_[j] - nums_[i-1];
		return nums_[j];
	}
};
TEST_CASE("303. Range Sum Query - Immutable", "[NEW]")
{
	NumArray t({ -2, 0, 3, -5, 2, -1 });
	CHECK(t.sumRange(0, 2) == 1);
	CHECK(t.sumRange(2, 5) == -1);
	CHECK(t.sumRange(0, 5) == -3);
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
	double new21Game(int N, int K, int W) {//TLE on large test
		vector<double> dp(K, -1);
		return new21Game(N, K, W, 0, dp);
	}
	double new21Game_bottomup(int N, int K, int W) {
		vector<double> dp(K+W);
		for (int i = 1; i <= N; i++) { // bottom up to calculate probability from N=1;

		}
		return 0;
	}
};


TEST_CASE("837. New 21 Game", "[NEW]")
{
	//CHECK(Game21().new21Game(9811, 8890, 7719) == Approx(1.0).epsilon(.00001));
	CHECK(Game21().new21Game(10, 1, 10) == Approx(1.0).epsilon(.00001));
	CHECK(Game21().new21Game(6, 1, 10) == Approx(0.6).epsilon(.00001));
	CHECK(Game21().new21Game(21, 17, 10) == Approx(0.73278).epsilon(.00001));
}