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
#include "large_int.h"
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

string sum_str(string& s1, string& s2)
{
	if (s1.size()<s2.size()) {
		swap(s1, s2);
	}
	int carry = sum_str(rbegin(s2), rend(s2), rbegin(s1), rbegin(s1));
	if (carry>0 && s1.size()>s2.size()) {
		transform(rbegin(s1) + s2.size(), rend(s1), rbegin(s1) + s2.size(), [&carry](char c) {
			carry += c - '0';
			c = carry % 10 + '0';
			carry /= 10;
			return c;
		});
	}
	if (carry)
		s1.insert(0, 1, carry + '0');
	return move(s1);
}

TEST_CASE("Project Euler #13: Large sum", "[NEW]")
{
	string sum = sum_str(string("37107287533902102798797998220837590246510135740250"), string("46376937677490009712648124896970078050417018260538"));
	string sum2 = sum_str(sum, string("74324986199524741059474233309513058123726617309629"));
	string sum3 = sum_str(sum2, string("91942213363574161572522430563301811072406154908250")); 
	string sum4 = sum_str(sum3, string("23067588207539346171171980310421047513778063246676"));
	CHECK(sum4.substr(0, 10) == "2728190129");
}
// Project Euler #3: Largest prime factor 
vector<long long> largestPrime(vector<long long> N)  // [10, 10^12]
{
	vector<int> primes = make_primes(1000000);
	vector<long long> ans;
	for (long long n : N) {
		long long max_p = 1;
		for (int p : primes) {
			if (p > n)
				break;
			if (n%p != 0)
				continue;
			max_p = max<long long>(max_p, p);
			while (n%p == 0) {
				n /= p;
			}
		}
		ans.push_back(max(n, max_p)) ;
	}
	return ans;
}
TEST_CASE("Project Euler #3: Largest prime factor", "[NEW]")
{
	CHECK(largestPrime(vector<long long>{10,17, 13195, 1000000000000LL-9, 987654321111}) == vector<long long>{5, 17, 29, 1000003, 20426761});
}

// Project Euler #9: Special Pythagorean triplet 
// Find maximum possible value of among all such Pythagorean triplets, a+b+c=N <= 3000
int maxABC(int N)
{
	static const auto uniq_right_triangles = rightTrianglesFundamental(3000);
	int ans = -1;
	for (const auto& t : uniq_right_triangles) {
		int perimeter = t[0] + t[1] + t[2];
		if (N >= perimeter && N%perimeter == 0) {
			int abc = N / perimeter;
			ans = max(ans, abc*abc*abc*t[0] * t[1] * t[2]);
		}
	}
	return ans;
}

TEST_CASE("Project Euler #9: Special Pythagorean triplet", "[NEW]")
{
	CHECK(maxABC(36) == 1620);
	CHECK(maxABC(3000) == 937500000);
}
// Project Euler #14: Longest Collatz sequence 
// find the start # (<=N) which produce longest chain, return the largest if more than one answer
// facts the longest chain is 597
class CollatzSequence
{
	// map<int64_t, int> seq_count; // hacker rank crash ( run out of stack), remove this extra caching solved problem
	vector<int>	  longest_chain;

	inline int64_t next_n(int64_t n)
	{
		return (n % 2 == 0) ? (n / 2) : (3 * n + 1);
	}
	int map_sequence(int64_t n) {
		if (n < longest_chain.size())
			return vec_sequence(n);
//		auto x = seq_count.find(n);
//		if (x != end(seq_count))
//			return x->second;
		return 1 + map_sequence(next_n(n));
//		return seq_count[n];
	}
	int vec_sequence(int64_t n) {
		if (n >= longest_chain.size())
			return map_sequence(n);
		if (longest_chain[static_cast<int>(n)] == 0)
			longest_chain[static_cast<int>(n)] = 1 + vec_sequence(next_n(n));
		return longest_chain[static_cast<int>(n)];
	}
public:
	CollatzSequence(int N):longest_chain(N+1, 0)
	{
		longest_chain[1] = 1;
		for (int i = 2; i <= N; i++)
			longest_chain[i] = vec_sequence(i);
		int max_chain = 0;
		int max_index = 0;
		for (int i = 1; i <= N; i++) {
			if (longest_chain[i] >= max_chain) {
				max_chain = longest_chain[i];
				max_index = i;
			}
			longest_chain[i] = max_index;
		}
		//cout << " map size " << seq_count.size() << " highest key=" << seq_count.rbegin()->first << ", val " <<seq_count.rbegin()->second << "\n";
	}
	int longest_N(int n)  // 
	{
		if (n < (int)longest_chain.size())
			return longest_chain[n];
		return 0;
	}
};
TEST_CASE("Project Euler #14: Longest Collatz sequence", "[LARGE]")
{
	CollatzSequence colla(5000000);
	CHECK(colla.longest_N(13) == 9);
	CHECK(colla.longest_N(20) == 19);
	CHECK(colla.longest_N(1000000) == 837799);
	CHECK(colla.longest_N(5000000) == 3732423);
}

// Project Euler #15: Lattice paths 
class LatticePath
{
	static const int MOD = 1000000007;
	vector<vector<int>> grid;
	int result=0;
public:
	LatticePath(int N, int M) : grid(N+1, vector<int>(M+1, 0))  // NxM lattice is 
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

// Project Euler #16: Power digit sum
class Power2DigitSum
{
	vector<int> digitSum;
public:
	Power2DigitSum(int N): digitSum(N) { //2^N
		string s1 = "1";
		for (int i = 0; i < N; i++) {
			int carry = sum_str(rbegin(s1), rend(s1), rbegin(s1), rbegin(s1));
			if (carry > 0)
				s1.insert(0, 1, carry + '0');
			digitSum[i] = accumulate(begin(s1), end(s1), 0, [](int init, char c) { return init + c - '0'; });
		}
	}
	int get(int N) const {
		return digitSum[N-1];
	}
};

TEST_CASE("Project Euler #16: Power digit sum", "[NEW]")
{
	Power2DigitSum digitSum(30);
	CHECK(digitSum.get(9) == 8);
}
// Project Euler #22: Names scores
class NameScore
{
	map<string, int> scores;
public:
	NameScore(vector<string>&& names) {
		sort(begin(names), end(names));
		int pos = 1;
		for (string& name : names) {
			int score = accumulate(begin(name), end(name), 0, [](int init, char c) { return init + c - 'A' + 1; });
			scores[move(name)] = score*(pos++);
		}
	}
	int get(const string& name)
	{
		return scores[name];
	}
};

// Project Euler #18: Maximum path sum I , move from top to adjacent number below
int MaxPathSum(vector<vector<int>>& triangle)  // bottom up dp
{
	for (int r = triangle.size() - 1; r > 0; r--) {
		for (int c = 0; c < (int)triangle[r].size()-1; c++) {
			triangle[r - 1][c] += max(triangle[r][c], triangle[r][c + 1]);  // add max of 2 adjacent nodes to the parent node
		}
	}
	return triangle[0][0];
};

// Project Euler #20: Factorial digit sum
class FactorialDigitSum
{
	vector<int> digitSum;  // 0! =1
public:
	FactorialDigitSum(int N) :digitSum{ 1, 1 } {
		int max_size = static_cast<int>(N*log10(N));
		digitSum.reserve(max_size);
		vector<char> factorial{ 1 };  // int digits in reverse order, least significant at left
		for (int n = 2; n <= N; n++) {
			int carry = large_int_multiply(begin(factorial), end(factorial), begin(factorial), n);
			while (carry > 0) {
				factorial.push_back(carry % 10);
				carry /= 10;
			}
			digitSum.push_back(accumulate(begin(factorial), end(factorial), 0));
		}
	}
	int get(int N) const
	{
		return digitSum[N];
	}
};
TEST_CASE("Project Euler #20: Factorial digit sum", "[NEW]")
{
	FactorialDigitSum fact_digits(1000);
	CHECK(fact_digits.get(0) == 1);
	CHECK(fact_digits.get(1) == 1);
	CHECK(fact_digits.get(998) == 10287);
	CHECK(fact_digits.get(999) == 10539);
	CHECK(fact_digits.get(1000) == 10539);
}

// Project Euler #17: Number to Words 
class NumberToWords {
	const vector<string> ones;
	const vector<string> tens;

	string decode_hundred(int h)  // less then a thousand, > 0
	{
		string s;
		if (h >= 100) {
			s.append(ones[h / 100]).append(" Hundred");
			h %= 100;
		}
		if (h >= 20) {
			if (!s.empty())
				s.append(1, ' ');
			s.append(tens[h / 10 - 2]);
			h %= 10;
		}
		if (h > 0) {
			if (!s.empty())
				s.append(1, ' ');
			s.append(ones[h]);
		}
		return s;
	}

	long long encode_by_3(string& s, long long N, long long unit, string unit_name) {
		if (N >= unit) {
			if (!s.empty())
				s.append(1, ' ');
			s.append(decode_hundred(N / unit)).append(move(unit_name));
		}
		return N % unit;
	}
public:
	NumberToWords() :ones{ "Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
		"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen" },
		tens{ "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" }
	{}

	string number2words(long long N)
	{
		if (N == 0)
			return ones[0];
		string s;
		N = encode_by_3(s, N, 1000000000000LL, " Trillion");
		N = encode_by_3(s, N, 1000000000, " Billion");
		N = encode_by_3(s, N, 1000000, " Million");
		N = encode_by_3(s, N, 1000, " Thousand");
		N = encode_by_3(s, N, 1, "");
		return s;
	}
};
TEST_CASE("Project Euler #17: Number to Words", "[NEW]")
{
	NumberToWords words;
	CHECK(words.number2words(900090009999LL) == "Nine Hundred Billion Ninety Million Nine Thousand Nine Hundred Ninety Nine");
	CHECK(words.number2words(0) == "Zero");
	CHECK(words.number2words(1) == "One");
	CHECK(words.number2words(40050) == "Forty Thousand Fifty");
	CHECK(words.number2words(1234567890) == "One Billion Two Hundred Thirty Four Million Five Hundred Sixty Seven Thousand Eight Hundred Ninety");
	CHECK(words.number2words(1000000000000LL) == "One Trillion");
	CHECK(words.number2words(111111111111LL) == "One Hundred Eleven Billion One Hundred Eleven Million One Hundred Eleven Thousand One Hundred Eleven");
}
