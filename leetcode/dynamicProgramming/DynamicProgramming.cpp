#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h
#include "myalgo.h"
#include "TreeNode.h"
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
		return WXU::maxSubSum(prices.begin(), prices.end() - 1, 0).first;
	}
	// 122. Best Time to Buy and Sell Stock II, many transactions, but only one transaction at a time
	int maxProfit2(vector<int>& prices) { // easy, Gredy, sum up all positive diff prices, beat 98%
		int sum = 0;
		for (int i = 0; i < (int)prices.size() - 1; i++)
			sum += max(prices[i + 1] - prices[i], 0);
		return sum;
	}
	// 123. Best Time to Buy and Sell Stock III, 2 transactions
	int maxProfit3(vector<int>& prices) { // beat 99%
		int n = prices.size();
		if (n < 2)
			return 0;
		// start from right, sell highest buy low
		vector<int> profitRight(n+1, 0);  // add extra slot for edge case later
		int sell = prices[n - 1];
		for (int i = n - 2; i >= 0; i--) {
			if (prices[i] > sell)// new selling point
				sell = prices[i];
			profitRight[i] = max(sell - prices[i], profitRight[i+1]);  // max profit from i till end
		}
		int buy = prices[0];
		int maxP = profitRight[0];
		for (int i = 0; i < n; i++) {
			if (prices[i] < buy)  // new low, new buy
				buy = prices[i];
			// transaction 1: prices[i] - buy, transaction 2: profitRight[i + 1]
			maxP = max(maxP, prices[i] - buy + profitRight[i + 1]);
		}
		return maxP;
	}

	//915. Partition Array into Disjoint Intervals, left side <= right side
	int partitionDisjoint(vector<int>& A) {
		vector<int> min_from_right(A.size()+1, INT32_MAX);
		for (int i = A.size() - 1; i >= 0; i--) {
			min_from_right[i] = min(A[i], min_from_right[i + 1]);
		}
		int max_from_left = INT32_MIN;
		for (size_t i = 0; i < A.size(); i++) {
			max_from_left = max(max_from_left, A[i]);
			if (max_from_left <= min_from_right[i + 1])
				return i + 1;
		}
		return 0; // not reachable
	}
};

TEST_CASE("123. Best Time to Buy and Sell Stock III", "[STOCK]")
{
	CHECK(BuyStock().maxProfit3(vector<int>{3, 2, 6, 5, 0, 3}) == 7);
}

TEST_CASE("309. Best Time to Buy and Sell Stock with Cooldown", "[DYN]")
{
	CHECK (BuyStock().maxProfit(vector<int> { 1, 2, 3, 0, 2 }) == 3);
	CHECK(BuyStock().maxProfit(vector<int> { 2, 1}) == 0);
	CHECK(BuyStock().maxProfit2(vector<int> { }) == 0);
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
		vector<char> dp(N + 1, 0);
		for (int i = 1; i <= N; i++) {
			if (valid(i))
				ans++;
		}
		return ans;
	}
	int rotatedDigits_dp(int N) {  // beat 100%
		int ans = 0;
		vector<char> dp(N + 1, 0);
		vector<char> first10{ 1,1,0,2,2,0,0,2,1,0 };//  0-success, 2 - invalid, 1 - valid but not good
		copy(begin(first10), end(first10), begin(dp));
		for (int i = 1; i <= N; i++) {  // each number can be computed from prev dp
			int p1 = i / 10;
			int p2 = i % 10;
			if (dp[p1] == 2 || dp[p2] == 2) // invalid in either part is invalid
				dp[i] = 2;
			else if (dp[p1] == 0 || dp[p2] == 0) { // success in either part is success
				ans++;
				dp[i] = 0;
			}
			else  // both part is valid but not good
				dp[i] = 1;
		}
		return ans;
	}
};
TEST_CASE("70. Climbing Stairs", "[DYN]")
{
	CHECK(ClimbStairs().climbStairs(2) == 2);
	CHECK(ClimbStairs().climbStairs(3) == 3);
	CHECK(ClimbStairs().climbStairs(10) == 89);
}
TEST_CASE("746. Min Cost Climbing Stairs", "[DYN]")
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
		return WXU::maxSubSum(nums.begin(), nums.end(), INT32_MIN).first; // in case sum is negative!
	}

	//918. Maximum Sum Circular Subarray
	int maxSubarraySumCircular(vector<int>& A) {
		int max_sum = INT32_MIN;
		int min_sum = INT32_MAX;
		int sum_pos = 0;
		int sum_neg = 0;
		for (int a : A) {
			sum_pos += a;
			sum_neg += a;
			max_sum = max(max_sum, sum_pos);
			min_sum = min(min_sum, sum_neg);
			if (sum_pos < 0)
				sum_pos = 0;
			if (sum_neg > 0)
				sum_neg = 0;
		}
		if (max_sum < 0 || min_sum >= 0) // number all negative, or none negative
			return max_sum;
		return max(max_sum, std::accumulate(begin(A), end(A), 0) - min_sum);
	}
	// 931. Minimum Falling Path Sum, from top to bottom, can move at most 1 to left or right
	int minFallingPathSum(vector<vector<int>>& A) {  // beat 99%
		if (A.empty())
			return 0;
		int N = A.size();
		for (int r = N - 2; r >= 0; r--) {  // from second to last row, compute min value to path down
			A[r][0] += min(A[r + 1][0], A[r + 1][1]);
			A[r][N-1] += min(A[r + 1][N-1], A[r + 1][N-2]);
			for (int c = 1; c < N-1; c++) {
				A[r][c] += min(min(A[r + 1][c - 1], A[r + 1][c]), A[r + 1][c + 1]);
			}
		}
		return *min_element(begin(A[0]), end(A[0]));
	}
	// 935. Knight Dialer, N-1 steps to dial N numbers
	// find total distinct numbers, mod 10^9+7
	int knightDialer(int N) {
		static const int MOD = 1000000007;
		vector<vector<int>> moves{ {4,6},{6,8},{7,9},{4,8},{0,3,9},{},{0,1,7},{2,6},{1,3},{2,4} };
		vector<vector<long long>> dp(2, vector<long long>(10, 1));
		int prev = 0;
		int curr = 0;
		for (int i = 1; i < N; i++) {
			curr = i % 2;
			prev = 1 - curr;
			for (int j = 0; j < 10; j++) {
				dp[curr][j] = 0;
				for (int m : moves[j])
					dp[curr][j] += dp[prev][m];
				dp[curr][j] %= MOD;
			}
		}
		return (int)(accumulate(begin(dp[curr]), end(dp[curr]), (long long)0)%MOD);
	}
	// 940. Distinct Subsequences II
	int distinctSubseqII(string S) {
		const int MOD = 1000000007;
		vector<long long> ends_with(26, 0);  // count sub sequence ends with each char
		for (char ch : S)  // append new char to all current sequences, plus 1
			ends_with[ch - 'a'] = (accumulate(begin(ends_with), end(ends_with), (long long)0) + 1) % MOD;
		return accumulate(begin(ends_with), end(ends_with), (long long)0) % MOD;
	}
	// 964. Least Operators to Express Number no parenthesis. 1<= target <= 2x10^8, x=[2,100]
	// x=100
	// building blocks: 1=100/100, 100, 100*100, etc. 100^x, x=0,1,2,...
	// 
	int cost(int exp) {
		if (exp == 0)
			return 2;
		return exp;
	}

	map<pair<int, int>, int> dp964; // result of intermediate target
	int leastOpsExpressTarget(int x, int target, int exp) {
		if (target <= 1)
			return cost(exp - 1)*target;
		if (dp964[{target, exp}] == 0) {
			int remain = target%x;
			target /= x;
			int c1 = cost(exp - 1)*remain;
			c1 += leastOpsExpressTarget(x, target, exp + 1);
			if (remain > 0) {  // two choices, e.g. 124=120+4 or 125-1
				int c2 = cost(exp - 1)*(x - remain);
				c2 += leastOpsExpressTarget(x, target + 1, exp + 1);
				c1 = min(c1, c2);
			}
			dp964[{target, exp}] = c1;
		}
		return dp964[{target, exp}];
	}
	// x=2, target=1023
	// exp=0, remain=1, new target=511, plus count (1022+2/2)=2, minus count(1024-2/2)=2
	// exp=1, remain=1, new target=255, plus count (2*510+2) =3, minus count( 2*512-2 ) = 3
	int leastOpsExpressTarget(int x, int target)  // borrow idea of bottom up, need to review again
	{
		//dp964.clear();    //top down
		//return leastOpsExpressTarget(x, target, 1)-1;
		int remain = target%x;
		int prev_plus_count = remain * 2;
		int prev_minus_count = remain > 0 ? (x - remain) * 2 : 0;
		int plus_count = prev_plus_count, minus_count = prev_minus_count;
		int exp = 1;
		while ((target /= x) > 0) {
			remain = target%x;
			plus_count = min(remain * exp + prev_plus_count, (remain + 1) * exp + prev_minus_count);
			minus_count = min((x - remain)*exp + prev_plus_count, (x - remain - 1)*exp + prev_minus_count);
			prev_minus_count = minus_count;
			prev_plus_count = plus_count;
			exp++;
		}
		return min(exp+minus_count, plus_count) - 1; // why k+minus_count?
	}
	// 790. Domino and Tromino Tiling
	// idea: build 2 types tile, even ended or not
	// case analysis: add one or two tiles to extend N one at a time
	int numTilings(int N) {
		if (N < 2)
			return N;
		int MOD = 1000000007;
		// base cases, tile len 1, 2
		vector<int> dp2{1, 2}; // domino
		vector<int> dp3{0, 2}; // tromino, sticking out at last position
		for (int i = 3; i <= N; i++) {
			int domino = dp2[0];// one way to add domino to prev prev tile, horizontal
			domino = (domino + dp2[1]) % MOD;// one way to get domino from prev domino, vertical
			domino = (domino + dp3[1])%MOD;   // add a tromino to uneven ended tile
			int tromino = (dp2[0] * 2) % MOD; // add tromino to even ended tile, 2 directions
			tromino = (tromino + dp3[1]) % MOD; // add a domino to extend from previous tromino
			swap(dp2[0], dp2[1]);
			swap(dp3[0], dp3[1]);
			dp2[1] = domino;
			dp3[1] = tromino;
		}
		return dp2[1];
	}
};
TEST_CASE("964. Least Operators to Express Number no parenthesis", "[DP]")
{
	CHECK(SpecialDp().leastOpsExpressTarget(2, 84500083) == 133);
	CHECK(SpecialDp().leastOpsExpressTarget(5, 126) == 4); // 5*5*5+5/5
	CHECK(SpecialDp().leastOpsExpressTarget(5, 3) == 4);   // 5-5/5-5/5 or 5/5+5/5+5/5
	CHECK(SpecialDp().leastOpsExpressTarget(5, 125) == 2);
	CHECK(SpecialDp().leastOpsExpressTarget(5, 124) == 4);
	CHECK(SpecialDp().leastOpsExpressTarget(5, 4) == 2);
	CHECK(SpecialDp().leastOpsExpressTarget(5, 2) == 3);
	CHECK(SpecialDp().leastOpsExpressTarget(5, 1) == 1);
}
TEST_CASE("935. Knight Dialer", "[DP]")
{
	CHECK(SpecialDp().knightDialer(1) == 10);
	CHECK(SpecialDp().knightDialer(2) == 20);
	CHECK(SpecialDp().knightDialer(3) == 46);
	CHECK(SpecialDp().knightDialer(5000) == 406880451);
}
TEST_CASE("918. Maximum Sum Circular Subarray", "[DP]")
{
	CHECK(SpecialDp().maxSubarraySumCircular(vector<int>{-2, -3, -1}) == -1);
	CHECK(SpecialDp().maxSubarraySumCircular(vector<int>{-1, -3, -2}) == -1);
	CHECK(SpecialDp().maxSubarraySumCircular(vector<int>{1,-2,3,-2}) == 3);
	CHECK(SpecialDp().maxSubarraySumCircular(vector<int>{5,-3, 5}) == 10);
	CHECK(SpecialDp().maxSubarraySumCircular(vector<int>{1, -2, 3, -2, 5}) == 7);
}
TEST_CASE("53. Maximum Subarray", "[DYN]")
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
TEST_CASE("303. Range Sum Query - Immutable", "[DYN]")
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


TEST_CASE("837. New 21 Game", "[DYN]")
{
	//CHECK(Game21().new21Game(9811, 8890, 7719) == Approx(1.0).epsilon(.00001));
	CHECK(Game21().new21Game(10, 1, 10) == Approx(1.0).epsilon(.00001));
	CHECK(Game21().new21Game(6, 1, 10) == Approx(0.6).epsilon(.00001));
	CHECK(Game21().new21Game(21, 17, 10) == Approx(0.73278).epsilon(.00001));
}
class BinaryTreeDp
{

};