#include <vector>
#include <string>
#include <algorithm>
using namespace std;

const string& shortestSuperseq_dp(const string& str1, const string& str2, int i, int j, vector<vector<string>>& memo)
{ // top down MLE
    if (i == str1.size() && j == str2.size())
        return memo[i][j];
    if (memo[i][j].empty()) {
        if (i == str1.size())
            memo[i][j] = str2.substr(j, str2.size() - j);
        else if (j == str2.size())
            memo[i][j] = str1.substr(i, str1.size() - i);
        else {
            if (str1[i] == str2[j])
                memo[i][j] = str1[i] + shortestSuperseq_dp(str1, str2, i + 1, j + 1, memo);
            else {
                const string& ans1 = shortestSuperseq_dp(str1, str2, i + 1, j, memo);
                const string& ans2 = shortestSuperseq_dp(str1, str2, i, j + 1, memo);
                memo[i][j] = ans1.size() < ans2.size() ? str1[i] + move(ans1) : str2[j] + ans2;
            }
        }
    }
    return memo[i][j];
}

string lcs_str_bottomup(const string& str1, const string& str2) {
    int n = str1.size(), m = str2.size();
    vector<vector<string>> dp(n + 1, vector<string>(m + 1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) { // compute common sequence up to i,j, store in dp[i+1][j+1]
            if (str1[i] == str2[j])
                dp[i + 1][j + 1] = dp[i][j] + str1[i];
            else
                dp[i + 1][j + 1] = dp[i + 1][j].size()> dp[i][j + 1].size()? dp[i + 1][j]: dp[i][j + 1];
        }
    }
    return dp[n][m];
}

string shortestCommonSupersequence_LCS(string str1, string str2) {
    int n = str1.size(), m = str2.size();
    int i = 0, j = 0;
    string ans;
    for (char c : lcs_str_bottomup(str1, str2)) {
        while (str1[i] != c)
            ans.append(1, str1[i++]);
        while (str2[j] != c)
            ans.append(1, str2[j++]);
        ans.append(1, c);
        i++, j++;
    }
    if (i < n)
        ans.append(begin(str1) + i, end(str1));
    if (j < m)
        ans.append(begin(str2) + j, end(str2));
    return ans;
}

string shortestCommonSupersequence(string str1, string str2) {
    vector<vector<string>> memo(str1.size()+1, vector<string>(str2.size()+1));
    return shortestSuperseq_dp(str1, str2, 0, 0, memo);
}


// 960. Delete Columns to Make Sorted III
// give array of string of equal length, find minimum columns need to be deleted
// to make each sorted in lexicographic order
int minDeletionSize(vector<string>& A) {  // O(N^3)
    int cols = A[0].size();
    vector<int> lcs(cols, 1);  // longest increasing sequence for all rows
    // base case, c=0, LCS is 1
    for (int c = 1; c < cols; c++) {  // compute LCS ends at c
        // for each previous column i, if every cell on that column is <= cell 
        // on column c of same row, extend LCS[i] by 1
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
    CHECK(lcs_str_bottomup("abac", "cab") == "ab");
    CHECK(shortestCommonSupersequence_LCS("abac", "cab") == "cabac");
}

TEST_CASE("960. Delete Columns to Make Sorted III", "[DP]")
{
    CHECK(minDeletionSize(vector<string>{"abbbaa"}) == 2);
    CHECK(minDeletionSize(vector<string>{"abbba"}) == 1);
    CHECK(minDeletionSize(vector<string>{"babca", "bbazb"}) == 3);
    CHECK(minDeletionSize(vector<string>{"edcba"}) == 4);
    CHECK(minDeletionSize(vector<string>{"ghi", "def", "abc"}) == 0);
}