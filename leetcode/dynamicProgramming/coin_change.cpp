#include <vector>
#include <algorithm>
using namespace std;

class CoinChange
{
    int coinChange(vector<int>& coins, vector<int>& dp, int amount) {// beat 17%
        if (amount == 0)
            return 0;
        int count = INT_MAX / 2;
        if (amount < 0)
            return count;
        if (dp[amount] >= 0)
            return dp[amount];

        for (int c : coins)
            count = min(count, 1 + coinChange(coins, dp, amount - c));
        dp[amount] = count;
        return count;
    }

public:
    // 322. Coin Change, minimal coins
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, -1);
        int ans = coinChange(coins, dp, amount);
        return ans >= INT_MAX / 2 ? -1 : ans;
    }
    int coinChange2(vector<int>& coins, int amount) {// iterative dp, beat 89%
        const int MAX_AMOUNT = amount + 1;
        vector<int> dp(MAX_AMOUNT, 0);
        sort(coins.begin(), coins.end(), greater<>());
        for (int money = 1; money < MAX_AMOUNT; money++) { // find best ans from low to high
            int count = MAX_AMOUNT;
            for (int c : coins) { // for each coin, compute count using previous dp
                if (c <= money)
                    count = min(count, 1 + dp[money - c]);
            }
            dp[money] = count;
        }
        return dp[amount] > amount ? -1 : dp[amount];
    }

    // 518. Coin Change 2, ways to make change
    int change(int amount, vector<int>& coins, size_t coin, vector<vector<int>>& dp) {
        if (amount == 0)
            return 1;
        if (coin == coins.size())
            return 0;
        if (dp[amount][coin] >= 0)
            return dp[amount][coin];
        int count = 0;
        for (size_t c = coin; c < coins.size(); c++) { // recursively give change of coins, forward only
            if (amount < coins[c])  // coins are sorted so no need to try bigger ones
                break;
            count += change(amount - coins[c], coins, c, dp);
        }
        dp[amount][coin] = count;
        return count;
    }
    int change(int amount, vector<int>& coins) { // dp with backtracking, beat 13%
        sort(coins.begin(), coins.end());
        vector<vector<int>> dp(amount + 1, vector<int>(coins.size(), -1));
        return change(amount, coins, 0, dp);
    }

    int change2(int amount, vector<int>& coins) { // iterative borrowed idea! beat 64%
        const int MAX_AMOUNT = amount + 1;
        vector<int> dp(MAX_AMOUNT, 0);
        dp[0] = 1;
        for (int c : coins) { // for each coin, compute count using previous dp
            for (int money = c; money < MAX_AMOUNT; money++) { //from low to high
                dp[money] += dp[money - c];
            }
        }
        return dp[amount];
    }
};


#include "catch.hpp"
TEST_CASE("coin change minimal", "[COIN]")
{
    CHECK(CoinChange().coinChange(vector<int>{2, 5, 6}, 10) == 2);
    CHECK(CoinChange().coinChange(vector<int>{5, 6}, 13) == -1);
    CHECK(CoinChange().coinChange2(vector<int>{2, 5, 6}, 10) == 2);
    CHECK(CoinChange().coinChange2(vector<int>{5, 6}, 13) == -1);
}

TEST_CASE("ways to make coin change", "[COIN]")
{
    CHECK(CoinChange().change2(0, vector<int>{}) == 1);
    CHECK(CoinChange().change2(5, vector<int>{}) == 0);
    CHECK(CoinChange().change2(5, vector<int>{1, 2, 5}) == 4);
    CHECK(CoinChange().change2(10, vector<int>{2, 5, 6}) == 3);
    CHECK(CoinChange().change2(13, vector<int>{5, 6}) == 0);
    CHECK(CoinChange().change2(10, vector<int>{10}) == 1);
}
