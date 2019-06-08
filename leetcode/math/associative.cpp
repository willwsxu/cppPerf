#include <map>
#include <algorithm>
#include <vector>
#include <numeric>
using namespace std;

// given a matrix of values 0 or 1, flip any number of column (flip all cells from 1 to 0 or 0 to 1)
// find max rows of same values
// idea: only flip all row with same pattern will produce result (same value on a row)
// if a raw has oppositie pattern, it will generate result of opposite value
// e.g row 10110 01001 can be flipped to 11111 00000
int maxEqualRowsAfterFlips(vector<vector<int>>& matrix) {
	map<vector<int>, int> row_pattern;
	for (vector<int>& r : matrix) {
		if (r[0]) {
			for (int& cell : r)
				cell = 1 - cell;  // flip row to opposite value as they are part of answer
		}
		row_pattern[r]++;
	}
	return max_element(begin(row_pattern), end(row_pattern), [](const auto & m1, const auto & m2) { return m1.second < m2.second; })->second;
}

// find submatrix sum 
// idea:for each pairs of columns, find all matrix that sharing bottom right corner, row by row
//  to speed up computing, calculate prefix sum of eash row
int numSubmatrixSumTarget(vector<vector<int>>& matrix, int target) {
	int m = matrix.size(), n = matrix[0].size();
	for (int i = 0; i < m; i++) {
		partial_sum(begin(matrix[i]), end(matrix[i]), begin(matrix[i]));
	}
	int res = 0;
	for (int c1 = 0; c1 < n; c1++) {
		for (int c2 = c1; c2 < n; c2++) {  // for each column pair c1, c2
			map<int, int> count{ {0,1} }; // count all matrix with c1 c2 as left and right sides
			int cumul = 0;
			for (int r = 0; r < m; r++) {
				int bottom_right = matrix[r][c2];
				if (c1>0)
					bottom_right -= matrix[r][c1-1];
				cumul += bottom_right;
				res += count[cumul-target];  // count of top left corners
				count[cumul]++; // add the left corner which can form desired matrix
			}
		}
	}
	return res;
}
//1074. Number of Submatrices That Sum to Target
#include "catch.hpp"
TEST_CASE("1072. Flip Columns For Maximum Number of Equal Rows", "[MAP]")
{
	CHECK(maxEqualRowsAfterFlips(vector<vector<int>>{ {0,1}, {1,0}}) == 2);
}

TEST_CASE("1074. Number of Submatrices That Sum to Target", "[MAP]")
{
	CHECK(numSubmatrixSumTarget(vector<vector<int>>{ {0, 1, 0, 0, 1}, { 0, 0, 1, 1, 1 }, { 1, 1, 1, 0, 1 }, { 1, 1, 0, 1, 1 }, { 0, 1, 1, 0, 0 }}, 1) == 47);
}