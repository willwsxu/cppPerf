// require performance optimization
#include "..\catch.hpp"
#include "mathlib_primes.h"

// Project Euler #58: Spiral primes
// performance enhancements:
// 1. % is expensive, revise code in russianPeasant_mul_mod (biggest factor probably)
// 2. mt19937 object creation is expensive, make it static
// 3. other minor change: MillerRabinPrimality repeat =4, the minimal to pass all tests
// 4. MillerRabinPrimality::test is run 3 times instead of 4
int spiral_prime_diag(int pct)  // below pct%
{
	int primes_on_diag = 0;
	for (int side = 3; ; side += 2) {
		long long diag = (long long)side*side;
		diag -= (side - 1);// skip  first number, a perfect square
		for (int loop = 0; loop<3; diag -= (side - 1), loop++) {
			if (MillerRabinPrimality::test(diag, 4))   // repeat 4 times in prime test is good enough
				primes_on_diag++;
		}
		int cur_pct = primes_on_diag * 100 / (2 * side - 1);  // round down pct is good
		if (cur_pct < pct)
			return side;
		//cout << " side =" << side << " pct=" << cur_pct << " primes on diag " << primes_on_diag << "\n";
	}
	return 0;
}
TEST_CASE("Project Euler #58: Spiral primes", "[LARGE]")
{
	CHECK(spiral_prime_diag(60) == 5);
	CHECK(spiral_prime_diag(40) == 31);
	CHECK(spiral_prime_diag(15) == 981);
	CHECK(spiral_prime_diag(14) == 1883);
	CHECK(spiral_prime_diag(12) == 6523);
	CHECK(spiral_prime_diag(11) == 12201);
	CHECK(spiral_prime_diag(9) == 74373);
	CHECK(spiral_prime_diag(8) == 238733);
}

// special phi, only ingterested in find phi when n/phi is <= previous min
int euler_phi_min_q(int n, const vector<int>& primes, double min_q) { //phi(n)=n*(1-1/p)(1-1/q)...
																//auto factors=prime_factors(n, primes);
	int phi = n;
	int n_reduce = n;
	for (int f : primes) {
		if (f*f>n_reduce)
			break;
		if (n_reduce%f != 0)
			continue;
		while (n_reduce%f == 0)
			n_reduce /= f;
		phi -= phi / f;
		if (phi*min_q<n)  // n/phi > min_q
			break;
	}
	if (n_reduce>1)
		phi -= phi / n_reduce;
	return phi;
}
// two critical optimizations to pass largest test
// euler_phi_min_q optimized to break out loop if n/phi is larger
//   also don't calculator prime factors early, do it while calculating phi, to take advantge of the optmization above
// call get_largest_permu after n/phi is not worse
int totient_permutation_min(int N) // 
{
	double ratio = 99999999;
	auto primes = make_primes((int)sqrt(N));
	int ans = 0;
	for (int n = N - 1; n>1; n--) {
		auto phi = euler_phi_min_q(n, primes, ratio);
		double new_ratio = (double)n / phi;
		//cout << n << " phi=" << phi.first << " ratio =" << new_ratio << "\n";
		if (new_ratio > ratio)
			continue;
		if (get_largest_permu(n) == get_largest_permu(phi)) { // n and phi are permutation
			ans = n;
			ratio = new_ratio;
		}
	}
	return ans;
}
TEST_CASE("Project Euler #70: Totient permutation", "[LARGE]")
{
	CHECK(totient_permutation_min(1000000) == 783169);
	CHECK(totient_permutation_min(10000000) == 8319823);
}