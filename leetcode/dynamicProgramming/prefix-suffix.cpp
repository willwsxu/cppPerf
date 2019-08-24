#include <vector>
#include <algorithm>
#include <string>

using namespace std;
int minHeightShelves_dp(vector<vector<int>>& books, int shelf_width, int idx, vector<int>& memo) {
    if (idx == books.size())
        return 0;
    if (memo[idx])
        return memo[idx];
    int height = 0;
    int thickness = 0;
    int result = 1000000000;
    for (int i = idx; i < books.size(); i++) {
        height = max(books[i][1], height);
        thickness += books[i][0];
        if (thickness > shelf_width)
            break;
        result = min(result, height + minHeightShelves_dp(books, shelf_width, i + 1, memo));
    }
    memo[idx] = result;
    return result;
}
int minHeightShelves_topdown(vector<vector<int>>& books, int shelf_width) {
    vector<int> memo(books.size(), 0);
    return minHeightShelves_dp(books, shelf_width, 0, memo);
}


int minHeightShelves(vector<vector<int>>& books, int shelf_width) {
    vector<int> memo(books.size()+1, 0);
    for (int i = 0; i < books.size(); i++) {
        int height = 0;
        int thickness = 0;
        int result = 1000000000;
        for (int j = i; j >= 0; j--) {
            thickness += books[j][0];
            if (thickness > shelf_width)
                break;
            height = max(books[j][1], height);
            result = min(height + memo[j], result);
        }
        memo[i + 1] = result;
    }
    return memo[books.size()];
}

string lastSubstring(string s) {
    vector<vector<int>> memo(2, vector<int>(s.size(), 0));
    int max_count = 0;
    int max_index = 0;  // base case, string size==1
    int max_val = 0;
    for (size_t len = 1; len < s.size(); len++) {
        for (int start = 0; start <= s.size() - len; start++) {
            int last = start + len - 1;
            int current = len % 2;
            memo[current][start] = memo[1 - current][start] * 26 + s[last] - 'a';
            if (memo[current][start] > max_val) {
                max_val = memo[current][start];
                max_index = start;
                max_count = 0;
            }
            else if (memo[current][start] == max_val)
                max_count++;
        }
        if (max_count < 2)  // found unique largest substring
            break;
        max_val = 0;  // there is tie for largest substring
        max_index = 0;
        max_count = 0;
    }
    return s.substr(max_index, s.size() - max_index + 1);
}

#include <catch.hpp>
TEST_CASE("1105. Filling Bookcase Shelves", "[DP]")
{
    CHECK(minHeightShelves(vector<vector<int>>{ {1, 1}, { 2, 3 }, { 2, 3 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 2 }}, 4) == 6);
    CHECK(minHeightShelves_topdown(vector<vector<int>>{ {1, 1}, { 2, 3 }, { 2, 3 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 2 }}, 4) == 6);
}
TEST_CASE("1163. Last Substring in Lexicographical Order", "[DP]")
{
    CHECK(lastSubstring("abab") == "bab");
    CHECK(lastSubstring("leetcode") == "tcode");
    CHECK(lastSubstring("zazazazaza") == "zazazazaza");
}