// require performance optimization
#include "mathlib_primes.h"

// Project Euler #58: Spiral primes
int spiral_prime_diag(int pct)  // below pct%
{
	//const long long max_diag_val = 1000000000000;
	int primes_on_diag = 0;
	for (int side = 3; ; side += 2) {
		long long diag = (long long)side*side;
		//if (diag > max_diag_val)
		//    break;
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
