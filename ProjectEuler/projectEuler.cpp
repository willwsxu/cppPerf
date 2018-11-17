////////////////////////////////////////////////
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "..\catch.hpp"  // don't put this file in stdafx.h

#include <map>
#include <set>
#include <numeric>
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
TEST_CASE("Project Euler #186: Connectedness of a network", "[EUL]")
{
	CHECK(successfulCalls(0, 1) == 622572);
	CHECK(successfulCalls(1, 99) == 3116963);
	CHECK(successfulCalls(524287, 99) == 2325629);
}

int gcd(int p, int q)
{
	return q == 0 ? p : gcd(q, p%q);
}
// http://www.friesian.com/pythag.htm
//Project Euler #39: Integer right triangles, perimeter N=[12, 5x10^6]
vector<vector<int>> rightTrianglesFundamental(int N)
{
	// Euclid method: M^2-n^2, 2mn, m^2+n^2, p=2m(m+n)>=4n^2, m>n
	vector<vector<int>> triangles;
	const int MAX_N = static_cast<int>(sqrt(N / 4)) + 1; // 4n^2 <= p
	const int MAX_M = static_cast<int>(sqrt(N / 2)) + 1;  // 2m^2 <= p
	cout << MAX_N << " " << MAX_M << "\n";
	for (int n = 1; n < MAX_N; n++) {
		int n_sq = n*n;
		for (int m = n + 1; m < MAX_M; m+=2) { // m and n must be odd parity
			if (gcd(m, n) == 1) {
				int m_sq = m*m;
				triangles.push_back({ m_sq - n_sq, 2 * m*n, m_sq + n_sq });
			}
		}
	}
	cout << "fundamental triangles " << triangles.size() << "\n";
	return triangles;
}

TEST_CASE("Euler #39 fundamental right Triangles", "[EUL]")
{
	auto x = rightTrianglesFundamental(5000000);
	set<vector<int>> uniq;
	for (auto& v : x) {
		sort(begin(v), end(v));
		uniq.insert(v);
	}
	CHECK(uniq.size() == x.size());
}
map<int,int> max_right_triangles_by_perimeter(int N)
{
	auto x = rightTrianglesFundamental(N);
	map<int, int> perimeter_map;
	for (const auto& v : x) {
		int perim = accumulate(cbegin(v), cend(v),0);
		for (int p = perim; p < N; p += perim)
			perimeter_map[p]++;  // step 1, count repeats of perimeter
	}
	cout << "perimeter_map triangles " << perimeter_map.size() << "\n";
	int max_count = 0;
	int max_val = 0;  // value with max_count
	for (auto& m : perimeter_map) {
		if (m.second > max_count) {
			max_count = m.second;
			max_val = m.first;
		}
		m.second = max_val;  // change map to store the last perimeter which has the max triangles
	}
	return perimeter_map;
}

TEST_CASE("Euler #39 rightTriangles", "[NEW]")
{
	map<int, int> triangles_same_perimeter = max_right_triangles_by_perimeter(5000000);
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