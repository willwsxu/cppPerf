#include <vector>
#include <string>
#include <algorithm>
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


// 960. Delete Columns to Make Sorted III
int minDeletionSize(vector<string>& A) {
    int cols = A[0].size();
    vector<int> lcs(cols, 1);  // longest increasing sequence for all rows
    for (int c = 1; c < cols; c++) {  // compute LCS ends at c
        for (int i = 0; i < c; i++) {
            int increasing = 1;
            for (int r = 0; r < (int)A.size(); r++) {
                if (A[r][i] > A[r][c]) {
                    increasing = 0;
                    break;
                }
            }
            if (increasing) {
                lcs[c] = max(lcs[c], lcs[i] + increasing);
            }
        }
    }
    return cols - *max_element(begin(lcs), end(lcs));
}
#include "catch.hpp"
TEST_CASE("1092. Shortest Common Supersequence", "[DP]")
{
    CHECK(shortestCommonSupersequence("abac", "cab") == "cabac");
}

TEST_CASE("960. Delete Columns to Make Sorted III", "[DP]")
{
    CHECK(minDeletionSize(vector<string>{"abbbaa"}) == 2);
    CHECK(minDeletionSize(vector<string>{"abbba"}) == 1);
    CHECK(minDeletionSize(vector<string>{"babca", "bbazb"}) == 3);
    CHECK(minDeletionSize(vector<string>{"edcba"}) == 4);
    CHECK(minDeletionSize(vector<string>{"ghi", "def", "abc"}) == 0);
}