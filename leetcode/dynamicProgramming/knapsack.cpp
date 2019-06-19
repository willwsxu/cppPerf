
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <iostream>
using namespace std;

// given N stones, pick any two, smash them. if weight x==y, both destroyed
// if x<y, x is destroyed, y weight is y-x
// find the smallest weight of last stone
// N [1,30], weight [1,100]
using mvi=map<vector<int>, int>;
int lastStoneWeightII(vector<int>& stones, mvi& memo) { //*********
	sort(begin(stones), end(stones), greater<int>());
	while (stones.back() == 0)
		stones.erase(end(stones) - 1);
	if (stones.size() ==1) {
		return stones[0];
	}
	if (memo.find(stones) != end(memo))
		return memo[stones];
	int result = INT32_MAX;
	for (int i = 0; i < stones.size()-1; i++) {
		if (i > 0 && stones[i] == stones[i - 1])
			continue;
		for (int j = i + 1; j < stones.size(); j++) {
			if (j > i + 1 && stones[j] == stones[j - 1])
				continue;
			int saveI = stones[i];
			int saveJ = stones[j];
			vector<int> next(begin(stones), end(stones));
			if (saveI == saveJ) {
				next[i] = next[j] = 0;
			}
			else if (saveI > saveJ) {
				next[i] -= next[j];
				next[j] = 0;
			}
			else {
				next[j] -= next[i];
				next[i] = 0;
			}
			result = min(result, lastStoneWeightII(next, memo));
		}
	}
	memo[stones] = result;
	return result;
}
// transform problem into: devide stones into 2 groups, find the smallest difference
int lastStoneWeightII(vector<int>& stones) {
    int weights = accumulate(begin(stones), end(stones), 0);  // at most 30*100=3000
    // out of all available stones, which weight sum is achieveable
    vector<bool> sack_memo(weights / 2 + 1, 0);
    sack_memo[0] = true;  // base case
    for (int s : stones) {
        // coin change allow multiple use so order is reversed!!
        for (int w = sack_memo.size()-1; w >=s; w--)  // reverse order as each weight used once
            sack_memo[w] = sack_memo[w] | sack_memo[w-s];
    }
    for (int w = weights / 2; w >= 0; w--)
        if (sack_memo[w])
            return weights - w - w;
    return 0;
}

// clash 2 heaviest stones at each round
int lastStoneWeight(vector<int>& stones) {
	int size = stones.size();
	while (size > 1) {
		sort(begin(stones), end(stones), greater<int>());
		stones[0] -= stones[1];
		stones[1] = 0;
		size--;
		if (stones[0] == 0)
			size--;
	}
	sort(begin(stones), end(stones), greater<int>());
	return size > 0 ? stones[0] : 0;
}

class Knapsack
{
    int tallestBillboard(const vector<int>& rods, int idx, vector<vector<int>>& memo, int b1, int b2, int max_h) {
        if (b1 > max_h || b2 > max_h)
            return -1;  // 0 is valid memo as relative value is stored
        if (idx == rods.size())
            return b1 == b2 ? b1 : -1;
        int delta = abs(b1 - b2);
        if (memo[idx][delta] < -1) {
            int max_val = max({ tallestBillboard(rods, idx + 1, memo, b1 + rods[idx], b2, max_h),
                tallestBillboard(rods, idx + 1, memo, b1, b2 + rods[idx], max_h),
                tallestBillboard(rods, idx + 1, memo, b1, b2, max_h) });
            memo[idx][delta] = max(-1, max_val - min(b1, b2));  // adjust to store value onward
            return max_val;
        }
        else // different b1, b2, but same |b1-b2|, would add on same height
            return memo[idx][delta] >= 0 ? memo[idx][delta] + min(b1, b2) : -1;  // add back previous value
    }
public:
    // 956. Tallest Billboard, rods equal size
    // standard DP with twists: 2d memo state using delta, memo value is relative to current value!!
    int tallestBillboard(vector<int>& rods) {
        int total = accumulate(begin(rods), end(rods), 0);
        vector<vector<int>> memo(rods.size(), vector<int>(total / 2 + 1, INT32_MIN));
        return tallestBillboard(rods, 0, memo, 0, 0, total / 2);
    }
    // transform into knapsack, hint from: The sum of rods is at most 5000.

    int tallestBillboard_bottomup(vector<int>& rods) { // slower than tallestBillboard above
        map<int, int> dp;  // key=difference of 2 billboards, value= max height of the shorter
        dp[0] = 0;
        for (int r : rods) {
            map<int, int> old_dp=dp;
            for (const auto entry : old_dp) {
                int delta = entry.first;
                // add rod to the longer side
                dp[delta + r] = max(dp[delta + r], entry.second);
                // add rod to the shorter side
                int addition = min(delta, r);
                delta = abs(delta - r);
                dp[delta] = max(dp[delta], addition + entry.second);
            }
        }
        return dp[0];
    }
};
#include "catch.hpp"
TEST_CASE("956. Tallest Billboard", "[DP]")
{
    CHECK(Knapsack().tallestBillboard(vector<int>{1, 2}) == 0);
    CHECK(Knapsack().tallestBillboard(vector<int>{96, 112, 101, 100, 104, 93, 106, 99, 114, 81, 94}) == 503);
    CHECK(Knapsack().tallestBillboard(vector<int>{2, 4, 8, 16}) == 0);
    CHECK(Knapsack().tallestBillboard(vector<int>{1, 2, 3, 4, 5, 6}) == 10);
    CHECK(Knapsack().tallestBillboard(vector<int>{1, 2, 3, 6}) == 6);

    CHECK(Knapsack().tallestBillboard_bottomup(vector<int>{1, 2}) == 0);
    CHECK(Knapsack().tallestBillboard_bottomup(vector<int>{96, 112, 101, 100, 104, 93, 106, 99, 114, 81, 94}) == 503);
    CHECK(Knapsack().tallestBillboard_bottomup(vector<int>{2, 4, 8, 16}) == 0);
    CHECK(Knapsack().tallestBillboard_bottomup(vector<int>{1, 2, 3, 4, 5, 6}) == 10);
    CHECK(Knapsack().tallestBillboard_bottomup(vector<int>{1, 2, 3, 6}) == 6);
}
TEST_CASE("1046. Last Stone Weight", "[MATH]")
{
	CHECK(lastStoneWeight(vector<int>{2, 7, 4, 1, 8, 1}) == 1);
}
TEST_CASE("1049. Last Stone Weight II", "[DYN]")
{
    CHECK(lastStoneWeightII(vector<int>{31, 26, 33, 21, 40}) == 5);
	CHECK(lastStoneWeightII(vector<int>{2, 7, 4, 1, 8, 1}) == 1);
}
/*
int main(int argc, char* argv[])
{
	cout << argc << endl;
}
*/