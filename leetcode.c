// leetcode.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>

int findMaxForm(char** strs, int strsSize, int m, int n) {
	int **memo = (int**)malloc((m + 1)*sizeof(int*));
	for (int i = 0; i <= m; i++) {
		memo[i] = (int *)malloc((n + 1)*sizeof(int));
		for (int j = 0; j <= n; j++) {
			memo[i][j] = 0;
		}
	}
	for (int i = 0; i<strsSize; i++) {
		int ones = 0;
		int zero = 0;
		for (int j = 0; strs[i][j] != 0; j++) {
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
	free(memo);
	return ans;

}


int *dp=0;
int combinationSum4Dp(int* nums, int numsSize, int target) {
	if (dp[target] >= 0)
		return dp[target];
	int total = 0;
	for (int i = 0; i < numsSize; i++) {
		if (target >= nums[i])
			total += combinationSum4Dp(nums, numsSize, target - nums[i]);
	}
	dp[target] = total;
	return total;
}
int combinationSum4(int* nums, int numsSize, int target) {
	dp = (int*)malloc((target + 1)*sizeof(int));
	for (int i=0; i<=target; i++)
	    dp[i]=-1;
	dp[0] = 1;
	return combinationSum4Dp(nums, numsSize, target);
}

int c_main()
{
	char **strs = (char **)malloc(5 * sizeof(char*));
	strs[0] = "10";
	strs[1] = "0001";
	strs[2] = "111001";
	strs[3] = "1";
	strs[4] = "0";
	int ans = findMaxForm(strs, 5, 5, 3);
	printf("%d\n", ans);
    return 0;
}

