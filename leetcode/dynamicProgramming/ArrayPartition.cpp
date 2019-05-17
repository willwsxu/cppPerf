#include <vector>
#include <algorithm>
using namespace std;

// 1043 Given array of none-negative int, partition into sub array with length at most K
// After partition, each sub array max value become value of its element
// find largest sum
int maxSumAfterPartitioning(vector<int>& A, int K, int start, vector<int>& memo) {
	if (start == A.size())
		return 0;
	if (memo[start] < 0) {
		int max_subarray = 0;
		int result = 0;
		int idx = start;
		for (int i = 0; i < K&& idx < A.size();  i++, idx++) {
			max_subarray = max(max_subarray, A[idx]);
			result = max(result, max_subarray * (i + 1) + maxSumAfterPartitioning(A, K, idx + 1, memo));
		}
		memo[start] = result;
	}
	return memo[start];
}
int maxSumAfterPartitioning(vector<int>& A, int K) {
	//vector<int> memo(A.size(), -1);
	//return maxSumAfterPartitioning(A, K, 0, memo);  top down
	vector<int> memo(A.size()+1, 0);  // additional space for clean code later
	for (int i = (int)A.size() - 1; i >= 0; i--) {
		int idx = i;
		int max_subarray = 0;
		int result = 0;
		for (int j = 0; j < K && idx<A.size(); j++, idx++) {
			max_subarray = max(max_subarray, A[idx]);
			result = max(result, max_subarray * (j + 1) + memo[idx + 1]);
		}
		memo[i] = result;
	}
	return memo[0];
}

#include <catch.hpp>
TEST_CASE("1043 maxSumAfterPartitioning", "[DYN]")
{
	CHECK(maxSumAfterPartitioning(vector<int>{1, 15, 7, 9, 2, 5, 10}, 3) == 84);
}