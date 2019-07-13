#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>
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
            if (str1[i] == str2[j]) {
                const string& ans = shortestSuperseq_dp(str1, str2, i + 1, j + 1, memo);
                memo[i][j].reserve(ans.size() + 1);
                memo[i][j].append(1, str1[i]).append(begin(ans), end(ans));
            }
            else {
                const string& ans1 = shortestSuperseq_dp(str1, str2, i + 1, j, memo);
                const string& ans2 = shortestSuperseq_dp(str1, str2, i, j + 1, memo);
                memo[i][j].reserve(min(ans1.size(), ans2.size()));
                if (ans1.size() < ans2.size())
                    memo[i][j].append(1, str1[i]).append(begin(ans1), end(ans1));
                else
                    memo[i][j].append(1, str2[j]).append(begin(ans2), end(ans2));
            }
        }
    }
    return memo[i][j];
}

string shortestCommonSupersequence_topdown(string str1, string str2) {
    vector<vector<string>> memo(str1.size()+1, vector<string>(str2.size()+1));
    return shortestSuperseq_dp(str1, str2, 0, 0, memo);
}

string lcs_str_bottomup(const string& str1, const string& str2) {
    size_t n = str1.size(), m = str2.size();
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
    size_t n = str1.size(), m = str2.size();
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

template<typename RandIter>
vector<vector<int>> lcs_bottomup(RandIter start1, RandIter start2, size_t n, size_t m) {
    vector < vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) { // compute common sequence up to i,j, store in dp[i+1][j+1]
            if (*(start1+i) == *(start2+j))
                dp[i + 1][j + 1] = dp[i][j] + 1;
            else
                dp[i + 1][j + 1] = max(dp[i + 1][j], dp[i][j + 1]);
        }
    }
    return dp;
}
template<typename RandIter>
vector<vector<int>> edit_distance_bottomup(RandIter start1, RandIter start2, size_t n, size_t m) {
    vector < vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int j = 1; j <= m; j++)
        dp[0][j] = j;  // base case, delete all string 2 to match empty string 1
    for (int i = 0; i < n; i++) {
        dp[i + 1][0] = i + 1; // base case, delete all string 1 to match empty string 2
        for (int j = 0; j < m; j++) { // compute common sequence up to i,j, store in dp[i+1][j+1]
            if (*(start1 + i) == *(start2 + j))
                dp[i + 1][j + 1] = dp[i][j];  // no edit
            else
                dp[i + 1][j + 1] = 1+ dp[i][j]; // replace
            dp[i + 1][j + 1] = min(dp[i + 1][j + 1], 1+min(dp[i + 1][j], dp[i][j + 1]));  // delete, insert
        }
    }
    return dp;
}
vector<vector<int>> lcs_matrix(const string& str1, const string& str2) {
    size_t n = str1.size(), m = str2.size();
    vector < vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) { // compute common sequence up to i,j, store in dp[i+1][j+1]
            if (str1[i] == str2[j])
                dp[i + 1][j + 1] = dp[i][j] + 1;
            else
                dp[i + 1][j + 1] = max(dp[i + 1][j], dp[i][j + 1]);
        }
    }
    return dp;
}
string shortestCommonSupersequence(string str1, string str2) {
    auto lcs = lcs_matrix(str1, str2);
    for (const auto& r : lcs) { copy(cbegin(r), cend(r), ostream_iterator<int>(cout, " ")); cout << endl;}
    int r = static_cast<int>(lcs.size() - 1);
    int c = static_cast<int>(lcs[0].size()-1);
    string res;
    res.reserve(str1.size() + str2.size() - lcs[r][c]);  // optimize memory alloc
    while (r > 0 && c > 0) {
        if (lcs[r][c] == lcs[r][c - 1]) { // move left, no change of lcs
            res.append(1, str2[--c]);
        }
        else if (lcs[r][c] == lcs[r - 1][c]) { // move up, no change of lcs
            res.append(1, str1[--r]);
        }
        else {  // move diagonal,  reduce lcs by 1
            res.append(1, str2[--c]);
            --r;
        }
    }
    res.resize(res.size() + max(r, c));
    reverse(begin(res), end(res));
    if (r > 0)
        copy(begin(str1), begin(str1)+r, begin(res));
    if (c>0)
        copy(begin(str2), begin(str2) + c, begin(res));
    return res;
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

int maxUncrossedLines(vector<int>& A, vector<int>& B, int a, int b, vector<vector<int>>& memo) {
    if (a == A.size() || b == B.size())
        return 0;
    if (memo[a][b] < 0) {
        memo[a][b] = maxUncrossedLines(A, B, a, b + 1, memo);
        memo[a][b] = max(memo[a][b], maxUncrossedLines(A, B, a + 1, b, memo));
        if (A[a] == B[b])
            memo[a][b] = max(memo[a][b], 1 + maxUncrossedLines(A, B, a + 1, b + 1, memo));
    }
    return memo[a][b];
}
int maxUncrossedLines(vector<int>& A, vector<int>& B) {
    //vector<vector<int>> memo(A.size(), vector<int>(B.size(), -1));
    //return maxUncrossedLines(A, B, 0, 0, memo);
    auto memo = lcs_bottomup(begin(A), begin(B), A.size(), B.size());  // 3x faster than top down
    return memo[A.size()][B.size()];
}

int minDistance(string word1, string word2) {
    auto memo = edit_distance_bottomup(begin(word1), begin(word2), word1.size(), word2.size());
    return memo[word1.size()][word2.size()];
}

class StringDp
{

    vector<vector<int>> dp;
    int minDistance(const string& word1, const string& word2, int pos1, int pos2) {
        if (pos1 == word1.size())
            return word2.size() - pos2;
        if (pos2 == word2.size())
            return word1.size() - pos1;
        if (dp[pos1][pos2] >= 0)
            return dp[pos1][pos2];
        if (word1[pos1] == word2[pos2])
            dp[pos1][pos2] = minDistance(word1, word2, pos1 + 1, pos2 + 1);
        else
            dp[pos1][pos2] = min(1 + minDistance(word1, word2, pos1 + 1, pos2), 1 + minDistance(word1, word2, pos1, pos2 + 1));
        return dp[pos1][pos2];
    }
public:
    // 583. Delete Operation for Two Strings, minimum number of steps required to make word1 and word2 the same
    int minDistance(string word1, string word2) { // beat 99%
        dp = vector<vector<int>>(word1.size(), vector<int>(word2.size(), int(-1)));
        return minDistance(word1, word2, 0, 0);
    }
};

#include "catch.hpp"
TEST_CASE("delete char to make string same", "[DP]")
{
    StringDp s;
    CHECK(s.minDistance("sea", "eat") == 2);
}

TEST_CASE("72. Edit Distance", "[DP]")
{
    CHECK(minDistance("intention", "execution") ==5);
    CHECK(minDistance("", "executions") == 10);
    CHECK(minDistance("intention", "") == 9);
}
TEST_CASE("1035. Uncrossed Lines", "[DP]")
{
    CHECK(maxUncrossedLines(vector<int>{2, 5, 1, 2, 5}, vector<int>{10, 5, 2, 1, 5, 2}) == 3);
    CHECK(maxUncrossedLines(vector<int>{1, 3, 7, 1, 7, 5}, vector<int>{1, 9, 2, 5, 1}) == 2);
}

TEST_CASE("1092. Shortest Common Supersequence", "[DP]")
{
    CHECK(shortestCommonSupersequence("bbbaaaba", "bbababbb") == "bbbaaababbb"); //"bbabaaababb"
    CHECK(shortestCommonSupersequence("abac", "cab") == "cabac");
    CHECK(lcs_str_bottomup("abac", "cab") == "ab");
    CHECK(shortestCommonSupersequence_LCS("abac", "cab") == "cabac");
    CHECK(shortestCommonSupersequence_topdown("abac", "cab") == "cabac");  // MLE
}

TEST_CASE("960. Delete Columns to Make Sorted III", "[DP]")
{
    CHECK(minDeletionSize(vector<string>{"abbbaa"}) == 2);
    CHECK(minDeletionSize(vector<string>{"abbba"}) == 1);
    CHECK(minDeletionSize(vector<string>{"babca", "bbazb"}) == 3);
    CHECK(minDeletionSize(vector<string>{"edcba"}) == 4);
    CHECK(minDeletionSize(vector<string>{"ghi", "def", "abc"}) == 0);
}