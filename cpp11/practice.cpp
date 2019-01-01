#include "..\catch.hpp"

#include "myalgo.h"
#include <vector>
#include <iterator>
#include <iostream>

using namespace std;

template<typename RandIter>
RandIter my_partition(RandIter first, RandIter last, RandIter target) {  // return a pivot where all elements before is < target
	while (first != last) {
		if (*first < *target) {
			++first;
			continue;
		}
		if (*--last >= *target)
			continue;
		iter_swap(first, last);
		++first;
	}
	if (--first != target)
		iter_swap(first, target);  // pivot has value of target
	return first;
}
int nth_smallest(vector<int> arr, int n)
{
	auto first = begin(arr);
	auto last = end(arr);
	while(true) {
		auto pivot = my_partition(first+1, last, first);  // bug 1: ++first
		int pos = distance(begin(arr), pivot);
		if (pos == n-1)  // all n-1 element left of pivot is <target
			return *pivot;
		if (pos < n-1)   // nth smallest lie to right of pivot, or pivot
			first = pivot+1;
		else       // nth smallest lie left of pivot
			last = pivot;  // pivot is the last target value
	}
}
TEST_CASE("find nth smallest", "[NEW]")
{
	SECTION("corner case") {
		auto case1 = vector<int>{ 40, 50, 20, 10, 30 };
		CHECK(nth_smallest(case1, 2) == 20);  // fix bug in partition
		CHECK(nth_smallest(case1, 1) == 10);
	}
	SECTION("in random order") {
		auto rand_v = vector<int>{ 50, 40, 30, 20, 10 };
		WXU::shuffle(rand_v);
		copy(begin(rand_v), end(rand_v), ostream_iterator<int>(cout, " "));
		cout << "\n";
		CHECK(nth_smallest(rand_v, 2) == 20);
		CHECK(nth_smallest(rand_v, 1) == 10);
		REQUIRE(nth_smallest(rand_v, 5) == 50);
	}
	SECTION("sorted in descending order") {
		CHECK(nth_smallest(vector<int>{50, 40, 30, 20, 10}, 1) == 10);
		CHECK(nth_smallest(vector<int>{50, 40, 30, 20, 10}, 2) == 20);
		REQUIRE(nth_smallest(vector<int>{50, 40, 30, 20, 10}, 5) == 50);
	}
	SECTION("sorted in ascending order") {
		CHECK(nth_smallest(vector<int>{10, 20, 30, 40, 50}, 2) == 20);
		CHECK(nth_smallest(vector<int>{10, 20, 30, 40, 50}, 1) == 10);
		REQUIRE(nth_smallest(vector<int>{10, 20, 30, 40, 50}, 5) == 50);
	}
}