#pragma once
#include <set>
#include <map>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>

using std::vector;
using std::set;

// formula n(2n-1)(2n+1)/3
// https://brilliant.org/wiki/sum-of-n-n2-or-n3/
inline long long sum_odd_square(long long N, int MOD) { // 1 to n, odd
	long long n = N / 2 + 1;
	long long n2 = 2 * n - 1;
	long long n3 = 2 * n + 1;
	if (n % 3 == 0)
		n /= 3;
	else if (n2 % 3 == 0)
		n2 /= 3;
	else if (n3 % 3 == 0)
		n3 /= 3;
	long long result = (n % MOD) *(n2%MOD) % MOD;
	return result*(n3%MOD) % MOD;
}

inline long long sum_arithmetic_sequence(long long first, long long last, int step, int MOD)
{
	long long n = (last - first) / step + 1;
	long long sum = first + last;
	if (n % 2 == 0)
		n /= 2;
	else
		sum /= 2;
	return (n%MOD)*(sum%MOD) % MOD;
}

inline long long sum_arithmetic_sequence(long long first, long long last, int step)
{
	long long n = (last - first) / step + 1;
	long long sum = first + last;
	if (n % 2 == 0)
		n /= 2;
	else
		sum /= 2;
	return n*sum;
}

inline vector<int> count_digit(int x) {
	vector<int> count(10, 0);
	while (x>0) {
		count[x % 10]++;
		x /= 10;
	}
	return count;
}

// enumerate number contain all digits in given set, no duplicate
// much slower than std next permutation (see euler #43)
template<typename CallBack>
void pandigital_enumerate(set<int> digits, long long val, CallBack f) {
	if (digits.empty()) {
		f(val);
		return;
	}
	for (int d : digits) {
		set<int> digits_next(digits);
		digits_next.erase(d);
		pandigital_enumerate(digits_next, val * 10 + d, f);
	}
}


class ChooseAll {  // compute N Choose 0 to n until value exceeds limit
	vector<long long> choices;
	int N;
public:
	ChooseAll(int N, long long limit) :choices{ 1 }, N(N) { //0!=1
		choices.reserve(N / 2 + 1);
		int n = N;
		int k = 1;
		while (N - k >= (int)choices.size()) {
			if ((double)choices.back()*n / k>limit)  // strive to not over flow
				break;
			long long choice = choices.back() / k;
			long long faction = choices.back() % k;
			choice *= n;
			choice += n*faction / k;
			choices.push_back(choice);
			n--;
			k++;
		}
	}
	int K_exceed_limit()
	{
		int low = choices.size() - 1;
		int high = N - low;
		return high == low ? 0 : high - low - 1;
	}
};

inline int euclid_distance_square(vector<int>& p1, vector<int>& p2)
{
	return (p1[0] - p2[0])*(p1[0] - p2[0]) + (p1[1] - p2[1])*(p1[1] - p2[1]);
}

inline int repeatingDecimals(int numer, int denom)  // find repeating decimals of a fraction
{
	vector<int> decimals;
	numer %= denom;
	std::map<int, int> remainders_pos{ { numer, 0 } };
	int pos = 1;
	while (numer) {
		numer *= 10;
		decimals.push_back(numer / denom);
		numer %= denom;
		auto found = remainders_pos.find(numer);
		if (found == end(remainders_pos)) {
			remainders_pos[numer] = pos++;
		}
		else
			return (pos - found->second);
	}
	return 0;
}

template<typename T>
T reverse(T n)
{
	T reversed = 0;
	while (n > 0) {
		reversed = reversed * 10 + n % 10;
		n /= 10;
	}
	return reversed;
}

inline bool isPalindrome(int64_t n)
{
	if (n<0 || n % 10 == 0)  // palindrome last digit cannot be 0
		return false;
	return n == reverse(n);
}

// generate palindrome number with prefix (first half, digits/2) [1,prefix), produce both odd and even length numbers
inline vector<int> allPalindrome(int prefix)
{
	vector<int> palindrome{ 1,2,3,4,5,6,7,8,9 };
	auto compute_palin = [](int palin, int prefix) {
		while (prefix > 0) {
			palin = palin * 10 + prefix % 10;
			prefix /= 10;
		}
		return palin;
	};
	for (int p = 1; p < prefix; p++) {
		palindrome.push_back(compute_palin(p, p));
		for (int i = 0; i < 10; i++)   // add center digit for odd len number
			palindrome.push_back(compute_palin(p * 10 + i, p));
	}
	return palindrome;// RVO
}

inline std::string base_conversion(int n, int b) { // convert int to any base
	std::string s;
	while (n>0) {
		s.append(1, n%b + '0');
		n /= b;
	}
	return s;
}

template<typename RandIter>
bool isPalindrome(RandIter first, RandIter last) {
	while (first != --last) {
		if (*first != *last)
			return false;
		if (++first == last)
			break;
	}
	return true;
}

// find digit permutation with largest value(sort digit in reverse order), function as key
template<typename T>
T get_largest_permu(T num) {
	std::vector<char> largest;
	while (num>0) {
		largest.push_back(num % 10);
		num /= 10;
	}
	sort(largest.begin(), largest.end(), std::greater<char>());
	num = 0;
	for (int i : largest)
		num = num * 10 + i;
	return num;
}
// similar to above but one more improvement for factorial
// e.g 870 and 871 have same sum of digit factorial, replace 0 with 1 as 0!=1!
inline long long get_largest_permu_factorial(long long num) {
	std::vector<char> largest;
	while (num>0) {
		largest.push_back(num % 10);
		num /= 10;
	}
	sort(largest.begin(), largest.end(), std::greater<char>());
	num = 0;
	for (int i : largest) {
		if (i == 0)
			i++;
		num = num * 10 + i;
	}
	return num;
}

class FactorialDigit
{
	vector<int> factorials;  // cache factorial of digits
public:
	FactorialDigit() :factorials(10, 1) {
		for (int i = 1; i<10; i++)
			factorials[i] = i*factorials[i - 1];
	}
	int get_factorial_digit_sum(int n) {  // sum of digit factorial
		int sum = 0;
		while (n>0) {
			sum += factorials[n % 10];
			n /= 10;
		}
		return sum;
	}
};