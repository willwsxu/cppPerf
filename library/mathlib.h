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