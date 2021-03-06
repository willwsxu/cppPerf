////////////////////////////////////////////////
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "mathlib.h"
#include "mathlib_largeInt.h"
#include "large_int_basic.h"
#include <map>
#include <set>
#include <unordered_set>
#include <numeric>
#include "myalgo.h"
#include "helper.h"
#include "UnionFind.h"
#include "digits-helper.h"
#include "mathlib_primes.h"
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

TEST_CASE("Euler #39 rightTriangles", "[OLD]")
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

TEST_CASE("Project Euler #218: Perfect right-angled triangles", "[OLD]")
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

TEST_CASE("Project Euler #125: Palindromic sums, test palindrome generator", "[OLD]")
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
	auto carry = sum_int(rbegin(s2), rend(s2), rbegin(s1), rbegin(s1));
	if (carry>0 && s1.size()>s2.size()) {
		transform(rbegin(s1) + s2.size(), rend(s1), rbegin(s1) + s2.size(), [&carry](char c) {
			carry += c - '0';
			c = carry % 10 + '0';
			carry /= 10;
			return c;
		});
	}
	if (carry)
		s1.insert(0, 1, (char)carry + '0');
	return move(s1);
}

TEST_CASE("Project Euler #13: Large sum", "[OLD]")
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
TEST_CASE("Project Euler #3: Largest prime factor", "[OLD]")
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

TEST_CASE("Project Euler #9: Special Pythagorean triplet", "[OLD]")
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

// Project Euler #16: Power digit sum
class Power2DigitSum
{
	vector<int> digitSum;
public:
	Power2DigitSum(int N): digitSum(N) { //2^N
		string s1 = "1";
		for (int i = 0; i < N; i++) {
			auto carry = (int)sum_int(rbegin(s1), rend(s1), rbegin(s1), rbegin(s1));
			if (carry > 0)
				s1.insert(0, 1, carry + '0');
			digitSum[i] = accumulate(begin(s1), end(s1), 0, [](int init, char c) { return init + c - '0'; });
		}
	}
	int get(int N) const {
		return digitSum[N-1];
	}
};

TEST_CASE("Project Euler #16: Power digit sum", "[POWER]")
{
	Power2DigitSum digitSum(30);
	CHECK(digitSum.get(9) == 8);
}
int power_digit_sum_max(int a, int N)  // a^b, b<N
{
	vector<char> large_int;
	large_int_fill(large_int, a);
	int max_sum = accumulate(begin(large_int), end(large_int), 0);
	for (int b = 1; b < N; b++) {
		large_int_multiply(large_int, a);
		max_sum = max(max_sum, accumulate(begin(large_int), end(large_int), 0));
	}
	return max_sum;
}
int power_digit_sum_max(int N) { // a^b for a<N, b<N
	int max_sum = 0;
	for (int a = 2; a < N; a++) {
		max_sum = max(max_sum, power_digit_sum_max(a, N));
	}
	return max_sum;
}
TEST_CASE("Project Euler #56: Powerful digit sum", "[POWER]")
{
	CHECK(power_digit_sum_max(5) == 13);
	CHECK(power_digit_sum_max(200) == 2205);
}
vector<long long> power_same_digit_count(int power)
{
	if (power == 1)
		return { 1,2,3,4,5,6,7,8,9 };
	vector<long long> ans;
	for (int a = 2; a < 10; a++) {
		auto result = Power_Large::compute_2(a, power, 19);
		if (result.second == power)
			ans.push_back(result.first);
	}
	return ans;
}
TEST_CASE("Project Euler #63: Powerful digit counts", "[NEW]")
{
	CHECK(power_same_digit_count(2) == vector<long long>{16,25,36,49,64,81});
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
			large_int_multiply(factorial, n);
			digitSum.push_back(accumulate(begin(factorial), end(factorial), 0));
		}
	}
	int get(int N) const
	{
		return digitSum[N];
	}
};
TEST_CASE("Project Euler #20: Factorial digit sum", "[OLD]")
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
			s.append(decode_hundred(static_cast<int>(N / unit))).append(move(unit_name));
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
TEST_CASE("Project Euler #17: Number to Words", "[WORDS]")
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


template<typename T, typename R>
class ProductSlidingWindow
{
	deque<T>  slider;
	int			size;
	R			runningVal = 1;
	R			maxVal = 0;
public:
	ProductSlidingWindow(int k) :size(k)
	{
	}

	void operator()(const T& v)
	{
		if (v == 0) {
			slider.clear();
			runningVal = 1;
		}
		else {
			runningVal *= v;
			slider.push_back(v);
			if (slider.size() == size) {
				maxVal = max(maxVal, runningVal);
				runningVal /= slider.front();
				slider.pop_front();
			}
		}
	}
	void operator()(char x)  // this is not specialization, just additional overload for string
	{
		operator()(T(x - '0'));
	}
	R get() const {
		return maxVal;
	}
};
TEST_CASE("Project Euler #8: Largest product in a series", "[OLD]")
{
	ProductSlidingWindow<short, long> maxProd(4);
	maxProd(short(5));
	maxProd(short(2));
	maxProd(short(3));
	maxProd(short(4));
	CHECK(maxProd.get() == 120);
	maxProd(short(10));
	CHECK(maxProd.get() == 240);

	string x = "3675356291";
	auto ans = for_each(begin(x), end(x), ProductSlidingWindow<short, long>(5));
	CHECK(ans.get() == 3150);
}

// Project Euler #11: Largest product in a grid 20x20
int maxProductGrid(vector<vector<int>> grid)
{
	int max_p = 0;
	for (size_t i = 0; i < grid.size(); i++) { // rows
		auto ans = for_each(begin(grid[i]), end(grid[i]), ProductSlidingWindow<int, int>(4));
		max_p = max(max_p, ans.get());
	}
	for (size_t j = 0; j < grid[0].size(); j++) {  // columns
		ProductSlidingWindow<int, int> slidingProd(4);
		for (size_t i = 0; i < grid.size(); i++)
			slidingProd(grid[i][j]);
		max_p = max(max_p, slidingProd.get());
	}
	int grid_size = grid.size();
	for (int diff = -grid_size + 4; diff <= 0; diff++) {  // backward diagnonal, right half, r-c same for each
		ProductSlidingWindow<int, int> slidingProd(4);
		for (int r = 0, c = r - diff; c<grid_size; r++, c = r - diff)
			slidingProd(grid[r][c]);
		max_p = max(max_p, slidingProd.get());
	}
	for (int diff = 1; diff <= grid_size - 4; diff++) {  // backward diagnonal, left half, r-c same for each
		ProductSlidingWindow<int, int> slidingProd(4);
		for (int c = 0, r = c + diff; r<grid_size; c++, r = c + diff)
			slidingProd(grid[r][c]);
		max_p = max(max_p, slidingProd.get());
	}
	for (int sum = 3; sum < grid_size; sum++) {  // forward diagnonal, top half, r+c same for each
		ProductSlidingWindow<int, int> slidingProd(4);
		for (int c = 0, r = sum - c; r >= 0; c++, r = sum - c)
			slidingProd(grid[r][c]);
		max_p = max(max_p, slidingProd.get());
	}
	for (int sum = grid_size; sum < 2 * grid_size - 4; sum++) {  // forward diagnonal, bottom half, r+c same for each
		ProductSlidingWindow<int, int> slidingProd(4);
		for (int r = grid_size - 1, c = sum - r; c < grid_size; r--, c = sum - r)
			slidingProd(grid[r][c]);
		max_p = max(max_p, slidingProd.get());
	}
	return max_p;
}

//Project Euler #12: Highly divisible triangular number
// What is the value of the first triangle number to have over N divisors?
class DivisibleTriangluar
{
	map<int, int> divisors_first_Num;  // first triangle number to have N divisors
public:
	DivisibleTriangluar(int N)
	{
		divisors_first_Num[1] = 1;
		int triangular = 1;  // triangular value
		int max_divisors = 1;
		int i = 2;
		while (max_divisors < N) {
			triangular += i++;
			int divisors = 2;  // 1 and itself
			for (int j = 2; j*j <= triangular; j++) {
				if (triangular%j == 0) {
					divisors += 2;
					if (j * j == triangular) {  // perfect square
						divisors--;
						break;
					}
				}
			}
			if (divisors > max_divisors) {  // map a new divisor count to the first triangular value, critical for speed
				max_divisors = divisors;
				divisors_first_Num[divisors] = triangular;
			}
		}
	}
	int get(unsigned int divisors) // first triangle number to have more divisors
	{
		auto found = divisors_first_Num.upper_bound(divisors);
		if (found != end(divisors_first_Num))
			return found->second;
		return -1;
	}
};
TEST_CASE("Project Euler #12: Highly divisible triangular number", "[TRIANG]")
{
	DivisibleTriangluar divisors(1000);
	CHECK(divisors.get(1) == 3);
	CHECK(divisors.get(2) == 6);
	CHECK(divisors.get(4) == 28);
	CHECK(divisors.get(1000) == 842161320);
}

int proper_divisor_sum(int n)
{
	int sum = 1;
	for (int j = 2; j*j <= n; j++) {
		if (n%j == 0) {
			sum += j;
			if (j*j < n)
				sum += n / j;
		}
	}
	return sum;
}
// Project Euler #21: Amicable numbers
// Let d(n) be defined as the sum of proper divisors of n (numbers less than n which divide evenly into n)
class AmicableNumbers
{
	map<int, int> amicable_sum;  // accumulative sum of all amicables up to this point
public:
	AmicableNumbers(int N) {
		vector<int> divisor_sum(N+1, 0);
		for (int n = 2; n <= N; n++) {
			int sum = proper_divisor_sum(n);
			divisor_sum[n] = sum;
			if (sum < n && divisor_sum[sum] == n) {  // check amicable pair at lower range
				amicable_sum[n] = 0;
				amicable_sum[sum] = 0;
			}
		}
		// compute partial prefix sum
		int running_sum = 0;
		for (auto start = begin(amicable_sum); start != end(amicable_sum); ++start) {
			running_sum += start->first;
			start->second = running_sum;
		}
	}
	int get(int N) {
		auto found = amicable_sum.lower_bound(N);
		if (found == end(amicable_sum) || found->first > N) {
			if (found == begin(amicable_sum))
				return 0;
			--found;
		}
		return found->second;
	}
};

TEST_CASE("Project Euler #21: Amicable numbers", "[AMIC]")
{
	AmicableNumbers amicables(100000);
	CHECK(amicables.get(1) == 0);
	CHECK(amicables.get(220) == 220);
	CHECK(amicables.get(300) == 504);
	CHECK(amicables.get(3000) == 8442);
	CHECK(amicables.get(30000) == 115818);
	CHECK(amicables.get(100000) == 852810);
}

// Project Euler #19: Counting Sundays
class Calendar {
	struct Date {
		int wday;  // weekday 0 Sunday, ..., 6 Saturday
		int day;   // 1 to 31
		int month; // 1 to 12
		int64_t year;  // from 1900
		int epoch_days = 0;
		Date(int wd, int d, int m, int64_t y) {
			wday = wd;
			day = d;
			month = m;
			year = y;
		}
	};
	Date start_day;
public:
	Calendar():start_day(1,1,1,1900) {
	}
	static bool is_leap(int64_t year) {
		if (year % 4 != 0)
			return false;
		if (year % 100 != 0)
			return true;
		return year % 400 == 0;
	}
	static int64_t leap_days(int64_t year) {  // since year 0 (excluded)
		return year / 4 - year / 100 + year / 400;
	}
	static int get_days_in_month(int m, int64_t y) {
		switch (m) {
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			return 31;
		case 4: case 6: case 9: case 11:
			return 30;
		case 2:
			return is_leap(y) ? 29 : 28;
		}
		return 0;
	}
	int countSundays(int64_t y1, int m1, int d1, int64_t y2, int m2, int d2) {
		assert(y1 >= 1900);
		assert(y1 <= y2);
		assert(y2 - y1 <= 1000);
		// since we count sundays on first day of month, let's standardize input values
		if (d1 > 1) {
			d1 = 1;
			m1++;
			if (m1 > 12) {
				m1 = 1;
				y1++;
			}
		}
		if (y1 > y2)
			return 0;
		int64_t total_days = 0;
		int64_t years = y1 - start_day.year;
		int first_wdays = start_day.wday;
		if (years>0) {
			total_days += years * 365;
			total_days += leap_days(y1 - 1) - leap_days(start_day.year - 1);
			first_wdays = (first_wdays + total_days) % 7;  // bug fix 
		}
		int first_month = 1;
		while (first_month < m1) {
			first_wdays = (first_wdays + get_days_in_month(first_month++, y1)) % 7;
		}
		int ans = 0;
		while (y1 < y2 || y1==y2 && first_month <= m2) {
			if (first_wdays == 0)
				ans++;
			first_wdays = (first_wdays + get_days_in_month(first_month++, y1)) % 7;
			if (first_month > 12) {
				first_month = 1;
				y1++;
			}
		}
		return ans;
	}
};

TEST_CASE("Project Euler #19: Counting Sundays", "[COUNT]")
{
	Calendar cal;
	CHECK(cal.countSundays(1905, 1, 1, 1909, 8, 2) == 9);
	CHECK(cal.countSundays(1900, 1, 1, 1909, 8, 2) == 18);
	CHECK(cal.countSundays(2000, 1, 1, 2020, 1, 1) == 35);
	CHECK(cal.countSundays(1999, 12, 12, 2020, 12, 1) == 37);
	CHECK(cal.countSundays(2000, 1, 30, 3000, 2, 2) == 1720);
}

// Project Euler #23: Non - abundant sums
// By mathematical analysis, it can be shown that all integers greater than 28123 can be written as the sum of two abundant numbers
class AbundantNumbers
{
	set<int> abundant;
	const int ALWAYS_ABUNDANT = 28124;
public:
	AbundantNumbers()
	{
		for (int i = 2; i < ALWAYS_ABUNDANT; i++)
			if (proper_divisor_sum(i) > i)
				abundant.insert(i);
	}
	bool abundant_sum(int N) {
		if (N >= ALWAYS_ABUNDANT)
			return true;
		for (int n : abundant) {
			if (n >= N)
				return false;
			if (abundant.count(N - n))
				return true;
		}
		return false;
	}
};

TEST_CASE("Project Euler #23: Non - abundant sums", "[ABUND]")
{
	AbundantNumbers abund;
	for (int i = 1; i < 24; i++)
		CHECK(abund.abundant_sum(i) == false);
	CHECK(abund.abundant_sum(24) == true);
	CHECK(abund.abundant_sum(49) == false);
	CHECK(abund.abundant_sum(28123) == true);
	CHECK(abund.abundant_sum(28124) == true);
}
// Project Euler #24: Lexicographic permutations
class LexicoPerm
{
	vector<long long> factorial;
public:
	LexicoPerm(int N) :factorial(N, 1) {
		for (int i = 2; i <= N; i++)
			factorial[i - 1] = factorial[i - 2] * i;
	}
	string permu(string&& word, long long Nth) { // find Nth permutation
		set<char> avail{ begin(word), end(word) };
		Nth--;  // count from 0
		word.clear();
		while (avail.size() > 1
			) {
			long long fact = factorial[avail.size() - 2];// from N letters, find (N-1)!
			int pick = static_cast<int>(Nth / fact); // pick from left to right
			auto found = begin(avail);
			advance(found, pick);
			word.push_back(*found);
			avail.erase(found);
			Nth %= fact;
		}
		word.push_back(*begin(avail));
		return word;
	}
};

TEST_CASE("Project Euler #24: Lexicographic permutations", "[PERMU]")
{
	LexicoPerm lexi(13);
	CHECK(lexi.permu("abcdefghijklm", 1) == "abcdefghijklm");
	CHECK(lexi.permu("abcdefghijklm", 2) == "abcdefghijkml");
}

// Project Euler #34: Digit factorials 
// sum of digit factorials is divisible by the number, 1!+9! divisible by 19
// find sum of such numbers <=N
class DigitFactorial
{
	vector<int> factorial;
public:
	DigitFactorial() : factorial(10, 1) {
		for (int i = 2; i < 10; i++)
			factorial[i] = factorial[i - 1] * i;
	}

	int sum_divisible(int N) { // N<=100000
		int sum = 0;
		for (int i = 10; i <= N; i++) {
			int temp = i;
			int fact_sum = 0;
			while (temp > 0) {
				fact_sum += factorial[temp % 10];
				temp /= 10;
			}
			if (fact_sum%i == 0)
				sum += i;
		}
		return sum;
	}
};

TEST_CASE("Project Euler #34: Digit factorials", "[FACT]")
{
	DigitFactorial digitFact;
	CHECK(digitFact.sum_divisible(20) == 19);
}

const static int MOD7 = 1000000007;
TEST_CASE("Project Euler #28: sum_odd_square", "[SUM]")
{
	CHECK(sum_odd_square(5, MOD7) == 35);
}
// Project Euler #28: Number spiral diagonals, find sum of numbers on diagonals
// idea: N=1, ans=1
//       N=3, ans=4*(1+3^2) - 3 - 2(1+2+3) 4 corners of the square: 9, 7, 5, 3
//       N=5, ans=4*(1+3^2+5^2) - 3, -4(1+2+3)  4 corners of the square: 25, 21, 17, 13
// in general: ans=4 * sum of odd square -3 -6 (sum of 2, 4, ... N-1)
int spiralDiagnoalSum(long long N)
{
	if (N == 1)
		return 1;
	long long ans = (4 * sum_odd_square(N, MOD7) - 3) % MOD7;
	ans -= 6 * sum_arithmetic_sequence(2, N - 1, 2, MOD7)%MOD7;
	if (ans < 0)
		ans += MOD7;
	return static_cast<int>(ans);
}
TEST_CASE("Project Euler #28: Number spiral diagonals", "[SPIRAL]")
{
	CHECK(spiralDiagnoalSum(99999999) == 370999997);
	CHECK(spiralDiagnoalSum(5) == 101);
	CHECK(spiralDiagnoalSum(10001) == 916705339);
}
// Project Euler #29: Distinct powers
// a^b, 2<= a, b <= N (10^5)
// if c=a^k,   ik is duplicate all i when 2<=iK<=N
long long countDistinctPower(int N)  // 10^5 is about 2^16
{
	int MAX_BASE_POWER = static_cast<int>(log2(N)); // a base is power of another base
	vector<char> exponents(N*MAX_BASE_POWER+1, 0);  // all possible exponents
	vector<int> repeats(MAX_BASE_POWER+1, 0);  // power repeats for any base is same
	for (int bp = 1; bp <= MAX_BASE_POWER; bp++) { 
		for (int p = 2; p <= N; p++) {
			int idx = bp*p;
			if (exponents[idx] == 0)
				exponents[idx] = bp; // store lower base power
			else
				repeats[bp]++;  // (2^3)^2==2^6 ==8^2, count as repeats for bp=3
		}
	}
	vector<char> bases_used(N + 1, 0);
	// count repeat in all base which is some power of prime base
	long long total_repeats = 0;
	for (int b = 2; b*b <= N; b++) {  // limit b to avoid overflow in inner for loop
		if (bases_used[b])
			continue;
		for (int bp=2, base = b*b; base <= N; base *= b, bp++) {
			bases_used[base] = 1;
			total_repeats += repeats[bp];
		}
	}
	long long total = (long long)(N - 1)*(N-1);
	return total - total_repeats;
}

TEST_CASE("Project Euler #29: Distinct powers", "[POWER]")
{
	CHECK(countDistinctPower(100000) == 9981236306LL);
	CHECK(countDistinctPower(5) == 15);
	CHECK(countDistinctPower(200) == 37774);
}

//Project Euler #32: Pandigital products
// Given N, find numbers a*b =c, combined digits from a, b, c are [1, N], all unique
// return sum of c
int pan_digital_prod(int N)
{
	const static vector<int> powers{ 1,10,100,1000,10000 };
	int max_val = powers[N / 2];
	int sum_ans = 0;
	for (int i = 12; i < max_val; i++) {  // check each product
		for (int j = 2; j*j < i; j++) {
			if (i%j == 0) {
				set<int> digits;
				auto add_digits = [&digits, N](int n) {
					while (n > 0) {
						int d = n % 10;
						if (d == 0 || d > N || digits.count(d))
							return false;
						digits.insert(d);
						n /= 10;
					}
					return true;
				};
				if (add_digits(i) && add_digits(j) && add_digits(i / j) && digits.size() == N) {
					sum_ans += i;
					break;
				}
			}
		}
	}
	return sum_ans;
}

TEST_CASE("Project Euler #32: Pandigital products", "[PANDIGIT]")
{
	CHECK(pan_digital_prod(8) == 13458);
	CHECK(pan_digital_prod(4) == 12);
}


// Project Euler #27: Quadratic primes, problem stated in Hack Rank is very confusing
// clarify: find a and b that generate max consecutive primes from n=0, given |a|<=N, |b|<=N
int quadratic_formular(int n, int a, int b)
{
	return n*n + a*n + b;
}

pair<int, int> solve_coefficient(int N)
{
	auto primes = make_primes(N + 1);
	int max_primes = 0;
	pair<int, int> ans{ 0,0 };
	for (int a = -N; a <= N; a++) {  // brute force, try all a, b combinations
		for (int b : primes) {
			int n = 1;   // no need to start from 0 as b is prime
			for (;;) {
				auto x = quadratic_formular(n++, a, b);
				if (!is_prime(x))
					break;
			}
			if (n > max_primes) {  // update a, b when there is higher prime
				ans.first = a;
				ans.second = b;
				max_primes = n;
			}
		}
	}
	return ans;
}

TEST_CASE("Project Euler #27: Quadratic primes", "[PRIME]")
{
	auto p = solve_coefficient(42);
	CHECK(p.first == -1);
	CHECK(p.second == 41);
	auto p2 = solve_coefficient(2000);
	CHECK(p2.first == -79);
	CHECK(p2.second == 1601);
}

TEST_CASE("Project Euler #26: repeatingDecimals", "[DECIMAL]")
{
	CHECK(repeatingDecimals(1, 8) == 0);
	CHECK(repeatingDecimals(1, 7) == 6);
}
// Project Euler #26: Reciprocal cycles
class LongestRepeatingDecimals
{
	map<int, int> max_repeating;  // smallest int with this many repeating 
public:
	LongestRepeatingDecimals(int N) {
		max_repeating[1] = 0; //0 repeating
		int max_r = 0;
		for (int i = 3; i <= N; i++) {
			if (is_prime(i)) {
				int repeat = repeatingDecimals(1, i);
				if (repeat > max_r) {
					max_r = repeat;
					max_repeating[i] = max_r;
				}
			}
		}
	}
	int findSmallestIntWithLongestRepeating(int n) {
		auto found = max_repeating.lower_bound(n);
		--found;
		return found->first;
	}
};

class LongestRepeatingDecimals2  // use vector instead of map
{
	vector<int> repeating_smalles_val;  // smallest int with max repeating so far
public:
	LongestRepeatingDecimals2(int N) :repeating_smalles_val(N, 1) {
		int max_r = 0;
		int max_r_val = 1;
		for (int i = 3; i < N; i++) {
			if (is_prime(i)) {
				int repeat = repeatingDecimals(1, i);
				if (repeat > max_r) {
					max_r = repeat;
					max_r_val = i;
				}
			}
			repeating_smalles_val[i] = max_r_val;
		}
	}
	int findSmallestIntWithLongestRepeating(int n) {
		return repeating_smalles_val[n - 1];  // vector and map had same speed
	}
};
TEST_CASE("Project Euler #26: Reciprocal cycles", "[DECIMAL]")
{
	LongestRepeatingDecimals2 repeating(5000);
	CHECK(repeating.findSmallestIntWithLongestRepeating(7) == 3); // longest repeating < 7
	CHECK(repeating.findSmallestIntWithLongestRepeating(10) == 7); // longest repeating < 10
	CHECK(repeating.findSmallestIntWithLongestRepeating(5000) == 4967); // longest repeating < 5000
}
// 
// phi(n)=n(1-1/p)(1-1/q)... so n/phi(n)=p/(p-1) * q/(q-1) *...
// to maximize it, choose smallest prime factors made of n < N
long long totient_max(long long N) {
	auto primes = make_primes(100); // enough to make up value 10^18
	long long n = 1;
	//double ans = 1;
	for (int p : primes) {
		if (n*p >= N || n*p < 0)
			break;
		n *= p;
		//ans *= (double)p / (p - 1);
	}
	return n;
}
TEST_CASE("Project Euler #69: Totient maximum", "[NEW]")
{
	CHECK(totient_max(1000000000000000000) == 614889782588491410);
}

TEST_CASE("Project Euler: Euler Totient function phi", "[NEW]")
{
	auto primes = make_primes(10000);
	CHECK(euler_phi(2, primes) == 1);
	CHECK(euler_phi(9, primes) == 6);
	CHECK(euler_phi(10, primes) == 4);
	CHECK(euler_phi(87109, primes) == 79180);
}

// requirements not clear:
// 1. sort answer by first term
// 2. if the progression has more than K, count all, one, or none?
vector<vector<int>> prime_permutation(int N, int K)
{
	auto primes = make_primes(1000000);
	map<int, vector<int>> prime_permu;
	for (int p : primes)
		prime_permu[get_largest_permu(p)].push_back(p);  // group prime numbers by same permutation
	vector<vector<int>> result;
	for (const auto& permu : prime_permu) {
		const vector<int>& AP = permu.second;
		int size = AP.size();
		if ( size < K || AP[0] >= N)
			continue;
		//copy(begin(AP), end(AP), ostream_iterator<int>(cout, " "));
		//cout << "\n";
		map<int, int> ap_diff;  // count by difference, for all pairs of numbers
		for (int i = 0; i < size - 1; i++) {  // pick first term
			for (int j = i + 1; j < size; j++)  // pick second
				ap_diff[AP[j] - AP[i]]++;
		}
		set<int> ap_set(begin(AP), end(AP));
		for (const auto& p : ap_diff) {
			if (p.second >= K - 1) {  // find K or more terms with same diff!!
				for (int i = 0; i < size - K + 1; i++) { // check start number and see if rest of terms in the set
					if (AP[i] >= N)  // check constraint again here
						break;
					auto validate_AP = [&ap_set, K, term1=AP[i], diff= p.first]() {
						for (int j = 1; j < K; j++) {
							if (ap_set.count(term1 + j*diff) == 0)
								return false;
						}
						return true;
					};
					if (validate_AP()) {
						vector<int> valid_ap{ AP[i] };
						for (int j = 1; j < K; j++)
							valid_ap.push_back(AP[i] + j*p.first);
						result.push_back(valid_ap);
					}
				}
			}
		}
	}
	sort(begin(result), end(result), [](const auto&v1, const auto&v2) { return v1[0]<v2[0]; });
	return result;
}
// 
TEST_CASE("Project Euler #49: Prime permutations, and arithmetic progression", "[NEW]")
{
	CHECK(prime_permutation(100000, 4) == vector<vector<int>>{ {83987, 88937, 93887, 98837}});
	CHECK(prime_permutation(10000, 3) == vector<vector<int>>{ {1487, 4817, 8147}, {2969, 6299, 9629}});
}
