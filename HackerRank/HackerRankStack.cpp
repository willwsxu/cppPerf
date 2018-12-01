#include "..\catch.hpp"  // don't put this file in stdafx.h
#include <vector>
#include "helper.h"
using namespace std;


// plates from stack Ai-1, move to stack Bi if divisible by prime[i], or A[i]
vector<int> waiter(vector<int> number, int q) {
	vector<int> primes = make_primes(10000);
	vector<int> ans;
	ans.reserve(number.size());
	for (int i = 0; i < q; i++) {
		auto B= stable_partition(begin(number), end(number), [p = primes[i]](int n) { return n%p != 0; });
		copy(B, end(number), back_inserter(ans));
		if (B == begin(number))
			return ans;
		number.erase(B, end(number));
		reverse(begin(number), end(number));
	}
	copy(rbegin(number), rend(number), back_inserter(ans));
	return ans;
}

TEST_CASE("Hacker rank stack Waiter test primes", "[NEW]")
{
	vector<int> primes = make_primes(10000);
	CHECK(primes[0]== 2);
	CHECK(primes[1] == 3);
	CHECK(primes[2] == 5);
	CHECK(primes[3] == 7);
	vector<int> primes2 = make_primes(200000);
	CHECK( primes2.size() ==17984);
}

TEST_CASE("Hacker rank stack Waiter", "[NEW]")
{
	CHECK(waiter(vector<int>{3,3,4,4,9}, 2) == vector<int>{4,4,9,3,3});
	CHECK(waiter(vector<int>{3, 4, 7, 6, 5}, 1) == vector<int>{4, 6, 3, 7, 5});
}