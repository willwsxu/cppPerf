#pragma once
#include "mathlib.h"
#include <random>

unsigned long long russianPeasant_mul_mod(unsigned long long a, unsigned long long b, unsigned long long mod)
{
	unsigned long long res = 0;  // initialize result 
	a %= mod;
	b %= mod;
	if (b > a)
		std::swap(a, b);  // second number determine loops, minimize it

	while (a>0 && b > 0)
	{
		if (a <= 0x7FFFFFFF && b <= 0x7FFFFFFF)
			return (a*b%mod + res) % mod;
		// If second number becomes odd, add the first number to result 
		if (b & 1) {
			res += a;
			if (res >= mod)
				res -= mod;          // res %= mod;  slow
		}
		// Double the first number and halve the second number 
		a <<= 1;
		b >>= 1;
		if (a >= mod)
			a -= mod;         // a %= mod; SLOW
	}
	return res;
}
unsigned long long pow_mod(unsigned long long base, unsigned long long exp, unsigned long long mod) // a^b % mod
{
	unsigned long long res = 1;  // initialize result 
	while (exp > 0) {
		if (exp & 1)
			res = russianPeasant_mul_mod(res, base, mod);
		base = russianPeasant_mul_mod(base, base, mod);
		exp >>= 1;
	}
	return res;
}
class MillerRabinPrimality
{
	unsigned long long num;
	bool mill_rabin_test_prime(unsigned long long d, int repeats) {
		static std::random_device rd;
		static std::mt19937 g(rd());
		std::uniform_int_distribution<unsigned long long> dis(2, std::min<unsigned long long>(num - 2, 2100000000)); //0x3FFFFFFFFFFFFFFF
		auto a = dis(g);
		auto x = pow_mod(a, d, num);
		if (x == 1 || x == num - 1)
			return true;
		while (repeats--) {
			x = russianPeasant_mul_mod(x, x, num);
			if (x == num - 1)
				return true;  // maybe a prime
			if (x == 1)
				return false;  // not a prime
		}
		return false;
	}
	MillerRabinPrimality(unsigned long long num_) : num(num_) {}
	bool miller_rabin_prime(int k)
	{
		int power2 = 0;
		auto d = num - 1; // num must be odd, num-1=2^r * d
		while ((d & 1) == 0) {
			power2++;
			d >>= 1;
		}
		while (k--) {
			if (!mill_rabin_test_prime(d, power2 - 1))
				return false;
		}
		return true; // probably prime
	}
public:
	static bool test(unsigned long long num, int k) {
		static const std::vector<int> small_primes{ 2,3,5,7,11,13,17 }; //,19,23
		static const int small_number = 17 * 19;
		//if (any_of(begin(small_primes), end(small_primes), [num](int p) { return num>p && num%p == 0; }))
		//    return false;
		if (num < 19) {
			if (std::find(begin(small_primes), end(small_primes), num) == end(small_primes))  // efficiency is not a concern if number is small
				return false;
			return true;
		}
		for (int i = 0; i < (int)small_primes.size(); i++) {
			if (num%small_primes[i] == 0)
				return false;
		}
		if (num < small_number)
			return true;
		MillerRabinPrimality miller_test(num);
		return miller_test.miller_rabin_prime(k);
	}
};
