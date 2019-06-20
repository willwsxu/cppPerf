#include <vector>
#include <string>
using namespace std;

string shortestSuperseq_dp(string& str1, string& str2, int i, int j, vector<vector<string>>& memo)
{
    if (i == str1.size() && j == str2.size())
        return "";
    else if (i == str1.size())
        return str2.substr(j, str2.size() - j);
    else if (j == str2.size())
        return str1.substr(i, str1.size() - i);
    if (memo[i][j].empty()) {
        if (str1[i] == str2[j])
            memo[i][j] = str1[i] + shortestSuperseq_dp(str1, str2, i + 1, j + 1, memo);
        else {
            string ans1 = str1[i] + shortestSuperseq_dp(str1, str2, i + 1, j, memo);
            string ans2 = str2[j] + shortestSuperseq_dp(str1, str2, i, j + 1, memo);
            memo[i][j] = ans1.size() < ans2.size() ? ans1 : ans2;
        }
    }
    return memo[i][j];
}
string shortestCommonSupersequence(string str1, string str2) {
    vector<vector<string>> memo(str1.size(), vector<string>(str2.size()));
    return shortestSuperseq_dp(str1, str2, 0, 0, memo);
}

#include "catch.hpp"
TEST_CASE("1092. Shortest Common Supersequence", "[DP]")
{
    CHECK(shortestCommonSupersequence("abac", "cab") == "cabac");
}