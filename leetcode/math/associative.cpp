#include <map>
#include <algorithm>
#include <vector>

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

#include "catch.hpp"
TEST_CASE("1072. Flip Columns For Maximum Number of Equal Rows", "[MAP]")
{
	CHECK(maxEqualRowsAfterFlips(vector<vector<int>>{ {0,1}, {1,0}}) == 2);
}