
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include <map>
#include <set>
#include <unordered_set>
#include <numeric>
#include "myalgo.h"
#include "helper.h"
#include "large_int_basic.h"
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
// Project Euler #31: Coin sums
class WaysMakeChange
{
	static const int MOD = 1000000007;
	vector<int> ways;
public:
	WaysMakeChange(int money, vector<int>&& coin_types): ways(money+1, 0)
	{
		ways[0] = 1;
		for (int c : coin_types) {  // fill one coint at a time
			for (int m = c; m <= money; m++) {
				ways[m] = (ways[m] + ways[m - c]) % MOD;
			}
		}
	}
	int get(int m) const
	{
		return ways[m];
	}
};

TEST_CASE("Project Euler #31: Coin sums ", "[OLD]")
{
	WaysMakeChange make_changes(100000, vector<int>{1,2,5,10,20,50,100,200});
	CHECK(make_changes.get(20) == 41);
}

class FibonacciDigits
{
	map<int, int>  digits_term;  // digits of first
public:
	FibonacciDigits(int N) {
		vector<int> f1{ 1 }, f2{ 1 };
		f1.reserve(N);
		f2.reserve(N);
		int term = 3;
		unsigned int digits = 1;
		while ((int)f1.size() < N) {
			sum(f1, f2);  // add f2 to f1
			if (f1.size() > digits) {
				digits = f1.size();
				digits_term[digits] = term;
			}
			swap(f1, f2);
			term++;
		}
	}
	int terms(int digits)
	{
		auto found = digits_term.lower_bound(digits);
		return found->second;
	}
};
TEST_CASE("Project Euler #25: N-digit Fibonacci number", "[SLOW]")
{
	FibonacciDigits digitFib(5000);
	CHECK(digitFib.terms(3) == 12);
	CHECK(digitFib.terms(4) == 17);
}