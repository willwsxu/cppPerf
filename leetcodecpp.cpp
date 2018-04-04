#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int findMaxForm(vector<string>& strs, int m, int n) {
	int **memo = new int*[m + 1];
	for (int i = 0; i <= m; i++) {
		memo[i] = new int[n + 1];
		for (int j = 0; j <= n; j++) {
			memo[i][j] = 0;
		}
	}
	for (auto i = 0; i<strs.size(); i++) {
		int ones = 0;
		int zero = 0;
		for (auto j = 0; j<strs[i].size(); j++) {
			if (strs[i][j] == '1')
				ones++;
			else
				zero++;
		}
		for (int j = m; j >= zero; j--) {
			for (int k = n; k >= ones; k--)
				memo[j][k] = max(memo[j][k], 1 + memo[j - zero][k - ones]);
		}
	}
	int ans = memo[m][n];
	delete[] memo;
	return ans;
}

#include <memory>

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

int combinationSum4(vector<int>& nums, int target) {
	dp = make_unique<int[]>(target + 1);
	std::fill(dp.get(), dp.get() + target + 1, -1);
	//for (int i=0; i<=target; i++)
	//    dp[i]=-1;
	dp[0] = 1;
	return combinationSum4Dp(nums, target);
}

class TrianglePath //120. Triangle
{
	int minimumTotal(vector<vector<int>>& triangle, int level, int pos, vector<vector<int>>& memo) {
		if (level == triangle.size() - 1)
			return triangle[level][pos];
		if (memo[level][pos] != INT_MAX)
			return memo[level][pos];
		int left = minimumTotal(triangle, level + 1, pos, memo);
		int right = minimumTotal(triangle, level + 1, pos + 1, memo);
		memo[level][pos] = std::min(left, right) + triangle[level][pos];
		return memo[level][pos];
	}
public:
	int minimumTotal(vector<vector<int>>& triangle) {
		vector<vector<int>> memo(triangle.size(), vector<int>(triangle.size(), INT_MAX));
		return minimumTotal(triangle, 0, 0, memo);
	}
	void test()
	{
		vector<vector<int>> triangle;
		triangle.push_back(vector<int>{ 2 });
		triangle.push_back(vector<int>{ 3, 4 });
		triangle.push_back(vector<int>{ 6, 5, 7 });
		triangle.push_back(vector<int>{ 4, 1, 8, 3 });
		printf("%d", minimumTotal(triangle));
	}
};

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
			if (j>=n)
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
		for (int r = 0; r<m; r++)
		{
			for (int c = 0; c<n; c++)
			{
				dp3[current][r][ c] = (r == 0 ? 1 : dp3[prev][r - 1][c]) % MOD;  // up !ERROR don't use i-- etc
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

#include <iostream>
void testEqualSumPartition()
{
	std::cout << canPartition(vector<int>{}) << endl;
	std::cout << canPartition(vector<int>{1, 5, 11, 5}) << endl;  // true
	std::cout << canPartition(vector<int>{1, 2, 3, 5}) << endl;   // false
}

// support negative num[i], even though current requirement is 
// 1 <= k <= len(nums) <= 16.
// 0 < nums[i] < 10000
bool dfs(vector<int>& nums, int k, vector<bool>& visited, int subsetSum, int target, int setIdx, int numIdx) {
	if (k == 1 && target !=0 || k==0)
		return true;
	if (subsetSum == target && setIdx>0)  // setIdx>0 is used to support target=0
		return dfs(nums, k - 1, visited, 0, target, 0, 0);
	for (int i = numIdx; i < nums.size(); i++) {
		if (visited[i] || subsetSum + nums[i] > target)
			continue;
		visited[i] = true;
		if (dfs(nums, k, visited, subsetSum + nums[i], target, setIdx+1, i + 1))
			return true;
		visited[i] = false;
	}
	return false;
}
// 698. Partition to K Equal Sum Subsets
bool canPartitionKSubsets(vector<int>& nums, int k) {
	vector<bool> visited(nums.size(), false);
	int sum = 0;
	for (int n : nums)
		sum += n;
	if ((sum %k) > 0)  // not divisible
		return false;
	if (k < 1)
		return false;
	return dfs(nums, k, visited, 0, sum / k, 0, 0);
}

void testKpartition()
{
	std::cout << canPartitionKSubsets(vector<int>{4, 3, 2, 3, 5, 2, 1}, 4) << endl;  // true
	std::cout << canPartitionKSubsets(vector<int>{-1,1,-1,1,-1,1, -1, 1, -1, 1}, 5) << endl;  // true
	std::cout << canPartitionKSubsets(vector<int>{-1, 1, -1, 1, -1, 1, -1, 1, -1, 1}, 6) << endl;  // false
	std::cout << canPartitionKSubsets(vector<int>{-1, 1, -1, 1, -1, 1, -1, 1, -1, 1}, 7) << endl;  // false
} 

/* 650. 2 Keys Keyboard
Initially on a notepad only one character 'A' is present.You can perform two operations on this notepad for each step:
	Copy All : You can copy all the characters present on the notepad(partial copy is not allowed).
	Paste : You can paste the characters which are copied last time.
	The n will be in the range [1, 1000]
*/
// greedy methods, find the largest the factor, steps = s[factor]+n/factor
int minSteps(int n) {
	int s = 0;
	for (int factor = n / 2; factor >= 1; factor--) {
		if (n%factor > 0)
			continue;
		s += n / factor;
		n = factor;
		factor = factor / 2 + 1;
	}
	return s;
}


void testKey2()
{
	std::cout << ((minSteps(1)==0)?"true":"false") << endl;
	std::cout << (minSteps(2)==2) << endl;
	std::cout << (minSteps(3)==3) << endl;
	std::cout << (minSteps(4)==4) << endl;
	std::cout << (minSteps(8)==6) << endl;
	std::cout << (minSteps(9)==6) << endl;
	std::cout << (minSteps(12)==7) << endl;
}

// prevBuy, last postion of buy, -1 means no
int maxProfit(vector<int>& prices, int pos, int prevBuy, vector<vector<int>>& dp) {
	if (pos >= prices.size()) {
		return 0;
	}
	if (dp[pos][prevBuy + 1] >= 0)
		return dp[pos][prevBuy + 1];
	int ans = maxProfit(prices, pos + 1, prevBuy, dp);  // no action
	if (prevBuy >= 0) {
		if (prices[pos] > prices[prevBuy])   // sell if there is profit
			ans = std::max(ans, (prices[pos] - prices[prevBuy])+maxProfit(prices, pos+2, -1, dp));
	}
	else
		ans= std::max(ans, maxProfit(prices, pos + 1, pos, dp));  // buy
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
	std::cout << (maxProfit(vector<int> { 1, 2, 3, 0, 2 })==3) << endl;
	std::cout << (maxProfit(vector<int> { 2, 1})==0) << endl;
}

#include <queue>
class DirectGraph {
	vector<vector<vector<int>>>	adjList;
	vector<bool> visited;
	vector<int>			distTo;
	vector<int>			stopsTo;
	int dfsMin(int s, int dest, int K) {
		if (stopsTo[s] > K)
			return INT_MAX/2;
		if (stopsTo[s] == K) {
			if (s == dest)
				return distTo[s];
			return INT_MAX / 2;
		}
		if (visited[s])
			return distTo[s];
		int ans = INT_MAX / 2;
		for (auto item : adjList[s]) {
			int next = item[1];
			if (distTo[s] + item[2] < distTo[next]) {
				distTo[next] = distTo[s] + item[2];
				stopsTo[next] = stopsTo[s] + 1;
				dfsMin(next, dest, K);
			}
		}
		visited[s] = true;
		return ans;
	}

public:
	int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int K) {
		adjList.resize(n);
		visited.resize(n, false);
		for (auto item : flights) {
			adjList[item[0]].push_back(item);
		}
		distTo.resize(n, INT_MAX / 2);
		stopsTo.resize(n, 0);
		distTo[src] = 0;
		int ans= dfsMin(src, dst, K+1);
		//for (int d : distTo)
		//	cout << d << " ";
		//cout << endl;
		return distTo[dst] >= INT_MAX / 2 ? -1 : distTo[dst];
	}
};

void test()
{
	DirectGraph t[4];
	int ans = 0;
	ans= t[0].findCheapestPrice(3, vector < vector<int>>{ {0, 1, 100}, { 1,2,100 }, { 0,2,500 }}, 0, 2, 0);  // 500
	cout << ans << endl;

	ans = t[1].findCheapestPrice(3, vector < vector<int>>{ {0, 1, 100}, { 1,2,100 }, { 0,2,500 }}, 0, 2, 1); // 200
	cout << ans << endl;

	ans = t[2].findCheapestPrice(3, vector < vector<int>>{ {0, 1, 100}, { 1,2,100 }, { 0,2,500 }}, 1, 0, 1); // -1
	cout << ans << endl;
	
	ans = t[3].findCheapestPrice(4, vector < vector<int>>{ {0, 1, 100}, { 0,2,500 }}, 0, 3, 0); // -1
	cout << ans << endl;

	vector < vector<int>> flights{ { 3,4,4 },{ 2,5,6 },{ 4,7,10 },{ 9,6,5 },{ 7,4,4 },{ 6,2,10 },{ 6,8,6 },{ 7,9,4 },{ 1,5,4 },{ 1,0,4 },{ 9,7,3 },{ 7,0,5 },{ 6,5,8 },{ 1,7,6 },{ 4,0,9 },{ 5,9,1 },{ 8,7,3 },{ 1,2,6 },{ 4,1,5 },{ 5,2,4 },{ 1,9,1 },{ 7,8,10 },{ 0,4,2 },{ 7,2,8 } };
	ans = t[3].findCheapestPrice(10, flights, 6, 0, 7); // 14
	cout << ans << endl;
}