#include "..\catch.hpp"  // don't put this file in stdafx.h
#include <vector>
using namespace std;

vector<char> make_primes(int N)
{
	vector<char> primes(1000000, 1); // 
	primes[0] = primes[1] = 0;
	for (size_t i = 2; i < primes.size(); i++)
	{
		if (primes[i]) {
			for (size_t j = i + i; j < primes.size(); j += i)
				primes[j] = 0;
		}
	}
	return primes;
}
vector<int> waiter(vector<int> number, int q) {
}

TEST_CASE("Hacker rank stack Waiter", "[NEW]")
{
	vector<char> primes = make_primes(1000000);
	CHECK(primes[2] == 1);
	CHECK(primes[3] == 1);
	CHECK(primes[5] == 1);
	CHECK(primes[7] == 1);
	CHECK(primes[27] == 0);
}