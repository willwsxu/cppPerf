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

void testTrianglePath()
{
	TrianglePath test;
	test.test();
}