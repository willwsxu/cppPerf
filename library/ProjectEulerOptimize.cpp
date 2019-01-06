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
TEST_CASE("Project Euler #58: Spiral primes", "[NEW]")
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
