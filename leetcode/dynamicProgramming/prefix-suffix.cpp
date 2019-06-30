#include <vector>
#include <algorithm>

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

#include <catch.hpp>
TEST_CASE("1105. Filling Bookcase Shelves", "[DP]")
{
    CHECK(minHeightShelves(vector<vector<int>>{ {1, 1}, { 2, 3 }, { 2, 3 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 2 }}, 4) == 6);
    CHECK(minHeightShelves_topdown(vector<vector<int>>{ {1, 1}, { 2, 3 }, { 2, 3 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 2 }}, 4) == 6);
}