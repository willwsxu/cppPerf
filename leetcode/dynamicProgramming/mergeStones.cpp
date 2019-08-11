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
    int n = (int)stones.size();
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

// brute force: try all possilbe K piles merges in each step
// another top down, but think reverse order
// last move is from K piles to 1, each pile is formed recursively backwards
int mergeStones_topdown(vector<int>& stones, int K, int piles2merge,  int first, int last, vector<vector<vector<int>>>& memo)
{
    int len = last - first + 1;
    int total = stones[last] - (first > 0 ? stones[first - 1] : 0);
    if (len == 1) {
        if (piles2merge == 1)
            return 0 + total; //  add value as it is part of K piles
        if (K == piles2merge)
            return 0;
    }        
    if (len < piles2merge)  // not enough piles, invalid
        return INT32_MAX;
    if (len == piles2merge)  // exactly # piles to merge
        return total;  // add value as it is part of K piles
    if (memo[first][last][piles2merge - 1] < 0) {
        int ans = INT32_MAX;
        if (piles2merge == 1) {  // len> 1, K piles to 1 pile
            int pile = mergeStones_topdown(stones, K, K, first, last, memo);
            ans  = pile == INT32_MAX ? pile : pile + total; // add value as it is 1 of K piles
        }
        else {
            for (int k = first; k <= last - piles2merge + 1; k++) {  // separate into 2 groups
                int pile1 = mergeStones_topdown(stones, K, 1, first, k, memo);  // 1 pile
                if (pile1 == INT32_MAX)
                    continue;
                int pile2 = mergeStones_topdown(stones, K, piles2merge - 1, k + 1, last, memo);  // piles2merge - 1 piles
                if (pile2 == INT32_MAX)
                    continue;
                ans = min(ans, pile1 + pile2);  // don't add value as we add only when piles2merge==1
            }
        }
        memo[first][last][piles2merge-1] = ans;
    }
    return memo[first][last][piles2merge-1];
}
int mergeStones_topdown(vector<int>& stones, int K)
{
    partial_sum(begin(stones), end(stones), begin(stones));
    vector<vector<vector<int>>> memo(stones.size(), vector<vector<int>>(stones.size(), vector<int>(K,-1)));
    int ans = mergeStones_topdown(stones, K, K, 0, (int)stones.size() - 1, memo);
    return ans == INT32_MAX ? -1 : ans;
}

// for K>2, it can be expressed as pile(i,j, K)=min[pile(1,i,x)+pile(K-1,x+1,j)]
// base case j-i<=k-1: pile(k,i,j)=j-i==k-1?0:-1
// to save space, reduce K by 1. base case j-i<=k:  pile(k,i,j)=j-i==k?0:-1
int mergeStones(vector<int>& stones, int K) {
    int n = (int)stones.size();
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


int stoneGameII(vector<int>& piles, const int start, const int M, vector<vector<int>>& memo) {
    int N = (int)piles.size();
    if (start >= N)
        return 0;
    int total = piles[N - 1] - (start > 0 ? piles[start - 1] : 0);
    if (start + M >= N) {  // take all piles
        return total;
    }
    if (memo[start][M] == 0) {
        int opponent = INT32_MAX;
        for (int x = 0; x < 2 * M && start + x < piles.size(); x++) {
            opponent = min(opponent, stoneGameII(piles, start + x + 1, max(x + 1, M), memo));
        }
        // assume A1, B2, A3,B4,A5,B6 are one scenario stones are collected
        // B6=0 (none left)
        // A5 = pile 5
        // B4 = total from 4 - A5 = pile 4
        // A3 = total from 3 - B4 = A3+A5
        // B2 = total from 2 - A3 = B2+B4
        // A1 = total from 1 - B2 = A1+A3+A5
        memo[start][M] = total - opponent;
    }
    return memo[start][M];
}
// two player game. each take turn to collect next x piles, 1<=x<=M, 
int stoneGameII(vector<int>& piles) {
    partial_sum(begin(piles), end(piles), begin(piles));
    vector<vector<int>> memo(piles.size(), vector<int>(piles.size(), 0));
    return stoneGameII(piles, 0, 1, memo);
}

#include "catch.hpp"
TEST_CASE("1140. Stone Game II", "[DP]")
{
    CHECK(stoneGameII(vector<int>{2, 7, 9, 4, 4}) == 10);
}

TEST_CASE("1000. Minimum Cost to Merge Stones", "[DP]")
{
    CHECK(mergeStones2(vector<int>{4,1,3,2}, 2) == 20);
    CHECK(mergeStones_topdown(vector<int>{1}, 2) == 0);
    CHECK(mergeStones_topdown(vector<int>{4, 1, 3, 2}, 2) == 20);
    CHECK(mergeStones_topdown(vector<int>{4, 1, 3, 2}, 3) == -1);
    CHECK(mergeStones_topdown(vector<int>{3, 5, 1, 2, 6}, 3) == 25);
}
// 3 5 1 2 6 K=3
// 3 - 5 1 2 6 = 3 + 22 = 25, 3 5 - 1 2 6 X, 3 5 1 - 2 6 = 9 + 9 + 8 = 26  K = 3
// 5 - 1 2 6 = 23, 5 1 - 2 6X, 5 2 1 - 6 = 22   K = 2
// 1 2 6 K = 3
// 5 2 1 K = 3