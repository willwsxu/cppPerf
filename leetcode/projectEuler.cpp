#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
#include "UnionFind.h"
using namespace std;

// Project Euler #186: Connectedness of a network.
// phone number generated via Lagged Fibonacci Generator, caller(n)=S2n-1, called(n)=S2n
// if caller(n)=celled(n), mis dial
// for each successful call, caller is a friend of celled, relation is transitive
// given one phone number, find how many successful calls need to reach p% friends of 1 million users
class FibGen
{
	vector<int> dp;
	const int MOD = 1000000;
public:
	FibGen(int K = 55) :dp(K, 0) {
		for (int k = 1; k <= K; k++) {
			dp[k - 1] = ((long long)300007 * k*k*k + 100003 - 200003 * k) % MOD;
		}
	}
	int get(int k) {
		if (k <= 55)
			return dp[k - 1];
		int result = (dp[31] + dp[0]) % MOD; // FibGen(k - 24) + FibGen(k - 55);
		rotate(begin(dp), begin(dp) + 1, end(dp));
		dp[54] = result;
		return result;
	}
};
int successfulCalls(int phone, int p)
{
	FibGen gen;
	UnionFind uf(1000000);
	int n = 1;
	int success = 0;
	int goal = p * 10000;
	while (true) {
		int caller = gen.get(n++);
		int called = gen.get(n++);
		if (called != caller) {
			success++;
			uf.union_(caller, called);
			if (uf.get_size(phone) >= goal) {
				return success;
			}
		}
	}
}
TEST_CASE("Project Euler #186: Connectedness of a network", "[NEW]")
{
	CHECK(successfulCalls(0, 1) == 622572);
	CHECK(successfulCalls(1, 99) == 3116963);
	CHECK(successfulCalls(524287, 99) == 2325629);
}

//Project Euler #39: Integer right triangles, perimeter N=[12, 5x10^6]
vector<int> rightTriangles(int N)
{
	// 3.4a <=5 x10^6
	// Euclid method: M^2-n^2, 2mn, m^2+n^2, p=2m(m+n)>=4n^2
	//vector<vector<int>> triangles;
	vector<int> perimeter;
	const int MAX_N = 1200;
	const int MAX_M = 1300;
	for (int n = 1; n < MAX_N; n++) {
		//int n_sq = n*n;
		for (int m = n + 1; m < MAX_M; m++) {
			//int m_sq = m*m;
			//triangles.push_back({ m_sq - n_sq, 2 * m*n, m_sq + n_sq });
			perimeter.push_back(2 * m*(m + n));
		}
	}
	//sort(begin(perimeter), end(perimeter));
	return perimeter;
}

TEST_CASE("Euler #39 rightTriangles", "[X]")
{
	auto x = rightTriangles(5000000);
	map<int, int> triangles_same_perimeter;
	for (int p : x)
		triangles_same_perimeter[p]++;
	int max_count = 0;
	int max_val = 0;  // value with max_count
	for (auto& m : triangles_same_perimeter) {
		if (m.second > max_count) {
			max_count = m.second;
			max_val = m.first;
		}
		m.second = max_val;  // change s
	}
	auto find_max_p = [&triangles_same_perimeter](int N) {
		auto found = triangles_same_perimeter.lower_bound(N);
		if (found->first > N)
			--found;
		return found->second;
	};
	CHECK(find_max_p(12) == 12);
	CHECK(find_max_p(80) == 60);
	CHECK(find_max_p(120) == 120);
}