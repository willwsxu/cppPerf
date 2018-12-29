#include "..\catch.hpp"

#include "myalgo.h"
#include <vector>

using namespace std;

template<typename RandIter>
RandIter my_partition(RandIter first, RandIter last, int target) {  // return a pivot where all elements before is < target
	while (first != last) {
		if (*first < target) {
			++first;
			continue;
		}
		if (*--last >= target)
			continue;
		iter_swap(first, last);
		++first;
		--last;
	}
	return first;
}
int nth_smallest(vector<int> arr, int n)
{
	auto first = begin(arr);
	auto last = end(arr);
	while(true) {
		auto target = first;
		auto pivot = my_partition(first+1, last, *target);  // bug 1: ++first
		int pos = distance(begin(arr), pivot);
		if (pos == n)  // all n element left of pivot is <target, except the target itself
			return *target;
		if (pos < n)   // nth smallest lie to right of pivot, or pivot
			first = pivot;
		else {         // nth smallest lie left of pivot
			last = --pivot;   // safe to skip the target value too
			iter_swap(first, last); // make the target value last
		}
	}
}
TEST_CASE("find nth smallest", "[NEW]")
{
	SECTION("in random order") {
		auto rand_v = vector<int>{ 50, 40, 30, 20, 10 };
		WXU::shuffle(rand_v);
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