#pragma once

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


inline bool is_prime(int n)
{
	n = abs(n);
	if (n < 2)
		return false;
	for (int i = 2; i*i <= n; i++) {
		if (n%i == 0)
			return false;
	}
	return true;
}

inline vector<int> make_primes(int N)  // prime numbers < N
{
	vector<int> primes;
	vector<char> is_primes(N, 1); // initialize to true
	is_primes[0] = is_primes[1] = 0;
	for (size_t i = 2; i < is_primes.size(); i++)
	{
		if (is_primes[i]) {
			primes.push_back(i);
			for (size_t j = i + i; j < is_primes.size(); j += i)
				is_primes[j] = 0;
		}
	}
	return primes;
}

// enumerate number contain all digits in given set, no duplicate
template<typename CallBack>
void pandigital_enumerate(set<int> digits, int val, CallBack f) {
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

inline int euclid_distance_square(vector<int>& p1, vector<int>& p2)  // share it
{
	return (p1[0] - p2[0])*(p1[0] - p2[0]) + (p1[1] - p2[1])*(p1[1] - p2[1]);
}