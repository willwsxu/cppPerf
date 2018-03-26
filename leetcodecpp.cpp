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

void test()
{
	std::cout << canPartitionKSubsets(vector<int>{4, 3, 2, 3, 5, 2, 1}, 4) << endl;  // true
	std::cout << canPartitionKSubsets(vector<int>{-1,1,-1,1,-1,1, -1, 1, -1, 1}, 5) << endl;  // true
	std::cout << canPartitionKSubsets(vector<int>{-1, 1, -1, 1, -1, 1, -1, 1, -1, 1}, 6) << endl;  // false
	std::cout << canPartitionKSubsets(vector<int>{-1, 1, -1, 1, -1, 1, -1, 1, -1, 1}, 7) << endl;  // false
} 
