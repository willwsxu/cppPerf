#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// Problem Desc: merge K stones into a pile, repeatedly until piles < K
// return -1 if it cannot be merged into 1 pile, else return sum of each merge, value of merging pile =total stones of piles merged
// special case, K=2
// consider last merge, there exist an optimal x such that pile[0,n-1]=pile[0,x]+pile[x+1,n-1]+total[0,n-1]
// in general, pile(i,j) = min[pile(i,x)+pile(x+1,j)]+total(i,j)
// base case: pile(i,i)=0
int mergeStones2(vector<int>& stones, int K) {
    int n = stones.size();
    partial_sum(begin(stones), end(stones), begin(stones));
    vector<vector<int>> memo(n,vector<int>(n,0));  // merge result between [i, j]
    // special case merge stone i,i, result 0
    for (int len = 1; len < n; len++) {  // len=j-i
        for (int i = 0; i < n - len; i++) {
            int j = i + len;
            int min_val = INT32_MAX;
            for (int x = i; x < j; x++)
                min_val = min(min_val, memo[i][x] + memo[x + 1][j]);
            int total_ij = i > 0 ? stones[j] - stones[i - 1] : stones[j];
            memo[i][j] = total_ij + min_val;
        }
    }
    return memo[0][n - 1];
}
// for K>2, it can be expressed as pile(K,i,j)=min[pile(1,i,x)+pile(K-1,x+1,j)]
// base case j-i<=k-1: pile(k,i,j)=j-i==k-1?0:-1
// to save space, reduce K by 1. base case j-i<=k:  pile(k,i,j)=j-i==k?0:-1
int mergeStones(vector<int>& stones, int K) {
    int n = stones.size();
    partial_sum(begin(stones), end(stones), begin(stones));
    vector<vector<vector<int>>> memo(K,vector<vector<int>>(n, vector<int>(n, 0)));  // merge result between [i, j]
    // special case merge stone i,i, result 0
    for (int k = 1; k < K; k++) {  // k==1 means merge 2 piles
        for (int len = k; len < n; len++) {  // len=j-i
            for (int i = 0; i < n - len; i++) {
                int j = i + len;
                int min_val = INT32_MAX;
                for (int x = i; x < j; x++)
                    min_val = min(min_val, memo[1][i][x] + memo[k-1][x + 1][j]);
                int total_ij = i > 0 ? stones[j] - stones[i - 1] : stones[j];
                memo[k][i][j] = total_ij + min_val;
            }
        }
    }
    return memo[K-1][0][n - 1];
}

#include "catch.hpp"
TEST_CASE("mergeStones2", "[DP]")
{
    CHECK(mergeStones2(vector<int>{4,1,3,2}, 2) == 20);
    CHECK(mergeStones(vector<int>{4, 1, 3, 2}, 2) == 20);
}
// K=4, k=1,2,3; len = j-i
// 3 4 2 6 9 1 5
// k=1, len=[1,6] at least 2 piles, i=[0,5],[0,4],..., [0,0]
// k=2, len=[2,6] at least 3 piles, i=[0,4],...,[0,0]
// k=3, len=[3,6] at least 4 piles, i=[0,3], ..., [0,0]