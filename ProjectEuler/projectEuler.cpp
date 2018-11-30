////////////////////////////////////////////////
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "..\catch.hpp"  // don't put this file in stdafx.h

#include <map>
#include <set>
#include <unordered_set>
#include <numeric>
#include "myalgo.h"
#include "helper.h"
#include "UnionFind.h"
using namespace std;

// reference
// https://euler.stephan-brumme.com/39/

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

// http://www.friesian.com/pythag.htm
//Project Euler #39: Integer right triangles, perimeter N=[12, 5x10^6]
vector<vector<int>> rightTrianglesFundamental(long long N) // primitive right angled triangle
{
	// Euclid method: M^2-n^2, 2mn, m^2+n^2, p=2m(m+n)>=4n^2, m>n
	vector<vector<int>> triangles;
	const int MAX_N = static_cast<int>(sqrt(N / 4)) + 1; // 4n^2 <= p
	const int MAX_M = static_cast<int>(sqrt(N / 2)) + 1;  // 2m^2 <= p
//	cout << MAX_N << " " << MAX_M << "\n";
	for (int n = 1; n < MAX_N; n++) {
		int n_sq = n*n;
		for (int m = n + 1; m < MAX_M; m+=2) { // m and n must be odd parity
			if (gcd(m, n) == 1) {
				int m_sq = m*m;
				triangles.push_back({ m_sq - n_sq, 2 * m*n, m_sq + n_sq });
			}
		}
	}
//	cout << "fundamental triangles " << triangles.size() << "\n";
	return triangles;
}

set<int> max_right_triangles_by_perimeter(int N)
{
	auto x = rightTrianglesFundamental(N);
	vector<unsigned char> perimeter_count(N+1,0);  // speed up a lot to use vector instead of map
	for (const auto& v : x) {
		int perim = accumulate(cbegin(v), cend(v),0);
		for (int p = perim; p <= N; p += perim)
			perimeter_count[p]++;  // step 1, count repeats of perimeter
	}

	set<int> best_perimeter{ 0 };  // only store the perimeter with better # of triangles
	int max_count = 0;
	for (size_t i = 0; i < perimeter_count.size(); i++) {
		if (perimeter_count[i] > max_count) {
			max_count = perimeter_count[i];
			best_perimeter.insert(i);
		}
	}
	//cout << " max count " << max_count << " " << best_perimeter .size() << "\n";  // max count 168 when N=5000000
	return best_perimeter;
}

TEST_CASE("Euler #39 fundamental right Triangles", "[LARGE]")
{
	auto x = rightTrianglesFundamental(5000000);
	set<vector<int>> uniq;
	for (auto& v : x) {
		sort(begin(v), end(v));
		uniq.insert(v);
	}
	CHECK(uniq.size() == x.size());
	set<int> best_perimeter = max_right_triangles_by_perimeter(5000000);
	CHECK(best_perimeter.size() == 30);
}

TEST_CASE("Euler #39 rightTriangles", "[NEW]")
{
	set<int> best_perimeter = max_right_triangles_by_perimeter(121);
	auto find_max_p = [&best_perimeter](int N) {
		auto found = best_perimeter.upper_bound(N);
		return *(--found);
	};
	CHECK(find_max_p(121) == 120);  // edge case, perimeter 121 has no triangle
	CHECK(find_max_p(120) == 120);
	CHECK(find_max_p(12) == 12);
	CHECK(find_max_p(80) == 60);
}
int main_func () {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	int n;
	cin >> n;
	vector<int> perimeters(n);
	int max_val = 0;
	for (int i = 0; i<n; i++) {
		cin >> perimeters[i];
		max_val = max(max_val, perimeters[i]);
	}
	set<int> best_perimeter = max_right_triangles_by_perimeter(max_val + 1);
	auto find_max_p = [&best_perimeter](int N) {
		auto found = best_perimeter.upper_bound(N);
		return *(--found);
	};
	for (int p : perimeters)
		cout << find_max_p(p) << "\n";
	return 0;
}

bool isPerfectSquare(long x)
{
	// Find floating point value of  
	// square root of x. 
	long double sr = sqrt(x);

	// If square root is an integer 
	return ((sr - floor(sr)) == 0);
}

// Project Euler #218: Perfect right-angled triangles
int perfect_right_triangle_not_super(long long N)
{
	vector<vector<int>> right_triangles = rightTrianglesFundamental(N);
	auto not_perfect = remove_if(begin(right_triangles), end(right_triangles), [](const auto&abc) {
		return !isPerfectSquare(abc[2]);
	});
	//right_triangles.erase(bad, end(right_triangles));
	auto super_perfect = count_if(begin(right_triangles), not_perfect, [](const auto&abc) {
		long long area = abc[0];  // test divisible by 6 and 28
		area *= abc[1];  // twice of area must be divisible by 28, so it should be divisible by 8
		return area % 8 == 0 && area % 21 == 0;  // 21 is multiple of factors from 6 and 28
	});
	return distance(begin(right_triangles), not_perfect)- super_perfect;
}

TEST_CASE("Project Euler #218: Perfect right-angled triangles", "[NEW]")
{
	CHECK(perfect_right_triangle_not_super(1000000) == 0); // trick q, all perfect right triangles are super perfect
}


// Project Euler #125: Palindromic sums
bool consecutiveSquareSum(int sum, int d) {

	for (int i = 1; i*i < sum; i++) {
		int sq = 0;
		for (int j = i; sq < sum; j += d) {
			sq += j*j;
			if (sq == sum)
				return j>i;
		}
	}
	return false;
}
long long sumOfNicePalindrome(const vector<int>& palin, int N, int d)  // 1<=N,d<=1000000000
{
	vector<int> result;
	copy_if(begin(palin), end(palin), back_inserter(result), [N, d](int p) { return p < N && consecutiveSquareSum(p, d); });
	return accumulate(begin(result), end(result), 0LL);
}

// much faster comparing to previous function
long long sumOfNicePalindrome2(int N, int d)  // 1<=N,d<=1000000000
{
	set<int> result;
	for (int i = 1; 2 * i*i < N; i++) {
		long long sq = i*i;
		for (long long j = i + d; sq < N; j += d) {
			sq += j*j;
			if (sq<N && isPalindrome(sq))
				result.insert((int)sq);
		}
	}
	return accumulate(begin(result), end(result), 0LL);
}

TEST_CASE("Project Euler #125: Palindromic sums, test palindrome generator", "[NEW]")
{
	CHECK(allPalindrome(10).size() == 108);
	vector<int> result = allPalindrome(10000);

	CHECK(sumOfNicePalindrome(result, 1000, 1) == 4164);
	CHECK(sumOfNicePalindrome(result, 1000, 2) == 3795);
	CHECK(sumOfNicePalindrome(result, 1000000, 1) == 14893023);
	CHECK(sumOfNicePalindrome(result, 1000000, 2) == 6398298);

	CHECK(sumOfNicePalindrome2(1000, 1) == 4164);
	CHECK(sumOfNicePalindrome2(1000, 2) == 3795);
	CHECK(sumOfNicePalindrome2(1000000, 1) == 14893023);
	CHECK(sumOfNicePalindrome2(1000000, 2) == 6398298);
	CHECK(sumOfNicePalindrome2(1000000000, 2) == 39283936423LL);
}

// Project Euler #4: Largest palindrome product < N, from 3 digit number
int largestPalindrome(int N)  // easy after isPalindrome from #125 
{
	int ans = 0;
	for (int i = 100; i < 1000; i++) {
		for (int j = 100; j < 1000; j++) {
			int prod = i*j;
			if (prod <N && isPalindrome(prod))
				ans = max(ans, prod);
		}
	}
	return ans;
}