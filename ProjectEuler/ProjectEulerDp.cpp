
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include <map>
#include <set>
#include <unordered_set>
#include <numeric>
#include "myalgo.h"
#include "helper.h"
using namespace std;

// Project Euler #15: Lattice paths 
class LatticePath
{
	static const int MOD = 1000000007;
	vector<vector<int>> grid;
	int result = 0;
public:
	LatticePath(int N, int M) : grid(N + 1, vector<int>(M + 1, 0))  // NxM lattice is 
	{
		grid[0][0] = 1;
		for (int i = 0; i <= N; i++) {
			for (int j = 0; j <= M; j++) { // dynamic programming
				if (i < N) {
					grid[i + 1][j] += grid[i][j];  // down
					grid[i + 1][j] %= MOD;
				}
				if (j < M) {
					grid[i][j + 1] += grid[i][j];  // right
					grid[i][j + 1] %= MOD;
				}
			}
		}
		result = grid[N][M];
	}
	int get() const {
		return result;
	}
};
TEST_CASE("Project Euler #15: Lattice paths", "[NEW]")
{
	LatticePath p(2, 2);
	CHECK(p.get() == 6);
}

// Project Euler #18: Maximum path sum I , move from top to adjacent number below
int MaxPathSum(vector<vector<int>>& triangle)  // bottom up dp
{
	for (int r = triangle.size() - 1; r > 0; r--) {
		for (int c = 0; c < (int)triangle[r].size() - 1; c++) {
			triangle[r - 1][c] += max(triangle[r][c], triangle[r][c + 1]);  // add max of 2 adjacent nodes to the parent node
		}
	}
	return triangle[0][0];
};
