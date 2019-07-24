#include <stack>
#include <algorithm>
#include <vector>
#include <numeric>

using namespace std;

// idea: find longest sequence with more tiring days
// 1. transform value, tiring day=1, none-tiring day=-1
// 2. compute prefix sum over entire sequence
// for any 2 points i and j, if prefix_sum[j]-prefix_sum[j-1]>0, it is a valid sequence
// 3a. using map is slow
// iterate sequence once, store prefix_sum[i] in map only if it is smaller to maximize longest seq
// 3b using stack
//   keep track of position of smaller prefix sum value, when traverse from left to right
//     discard any larger values to the right as it forms shorter sequence only
//   traverse from right to left, to find if there is any valid sequence, for each j
//     find if there is i on top of stack so prefix[j]-prefix[i]>1
int longestWPI_map(vector<int>& hours) {
    if (hours.empty())
        return 0;
    transform(begin(hours), end(hours), begin(hours), [](int h) { return h > 8 ? 1 : -1; });
    partial_sum(begin(hours), end(hours), begin(hours));
    map<int, int> val_early_pos{ {0,-1} };  // earliest position of a value

    int longest = 0;
    for (int p = 0; p < (int)hours.size(); p++) {
        if (hours[p] < begin(val_early_pos)->first)
            val_early_pos.emplace(hours[p], p);
        auto start = val_early_pos.lower_bound(hours[p] - 1);
        if (start == end(val_early_pos))
            --start;
        else if (start->first > hours[p] - 1) {
            if (start == begin(val_early_pos))
                continue;
            else
                --start;
        }
        longest = max(longest, p - start->second);
    }
    return longest;
}

int longestWPI(vector<int>& hours) {
    transform(begin(hours), end(hours), begin(hours), [](int h) { return h > 8 ? 1 : -1; });
    partial_sum(begin(hours), end(hours), begin(hours));
    stack<int> decreasing_index;  // 
    for (int p = 0; p < static_cast<int>(hours.size()); p++) {
        if (hours[p] >= 0)
            continue;
        if (decreasing_index.empty() || hours[p] < hours[decreasing_index.top()])
            decreasing_index.push(p);
    }
    int ans = 0;
    for (int p = static_cast<int>(hours.size())-1; p > 0; p--) {
        while (!decreasing_index.empty() && hours[p] > hours[decreasing_index.top()]) {
            ans = max(ans, p - decreasing_index.top());
            decreasing_index.pop();
        }
        if (decreasing_index.empty() && hours[p]>0) {
            ans = max(ans, p+1);
            break;
        }
    }
    return max<int>(ans, hours[0]>0);
}; 

#include "catch.hpp"
TEST_CASE("1124. Longest Well-Performing Interval", "[STACK]")
{
    CHECK(longestWPI(vector<int>{9, 9, 6, 0, 6, 6, 9}) == 3);
}
TEST_CASE("1124. Longest Well-Performing Interval, map", "[MAP]")
{
    CHECK(longestWPI_map(vector<int>{9, 9, 6, 0, 6, 6, 9}) == 3);
}