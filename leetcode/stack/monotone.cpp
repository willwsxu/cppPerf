#include <stack>
#include <algorithm>
#include <vector>
#include <numeric>

using namespace std;

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
TEST_CASE("1124. Longest Well-Performing Interval", "[MAP]")
{
    CHECK(longestWPI(vector<int>{9, 9, 6, 0, 6, 6, 9}) == 3);
}