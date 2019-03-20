#include "..\catch.hpp"

#include "myalgo.h"
#include <vector>
#include <iterator>
#include <iostream>
#include <functional>

using namespace std;

template<typename RandIter, typename Comp >
RandIter my_partition(RandIter first, RandIter last, Comp comp) {  // return a pivot where all elements before is < target
	RandIter second = first; // belong to second partition
	while (second != last && comp(*second))
		++second;
	if (second == last)
		return second;
	// find first item that should be in second partition
	for (first = second + 1; first != last; ++first) {
		if (comp(*first)) {
			iter_swap(first, second);
			++second;
		}
	}
	return second;
}
int nth_smallest(vector<int> arr, int n)
{
	auto first = begin(arr);
	auto last = end(arr);
	while(first !=last) {
		// partition scheme won't work on [10,20,30] with < operator
		auto pivot = my_partition(first, last, [first](int t) { return t < *first; });  // bug 1: ++first
		int pos = distance(begin(arr), pivot);
		if (pos == n - 1)  // all n-1 element left of pivot is <target
		{
			auto minimum = min_element(pivot, end(arr));
			if (minimum != pivot)  // minimum of second partition is the answer, not necessary at pivot
				iter_swap(minimum, pivot);
			return *pivot;
		}
		else if (pos < n - 1) {   // nth smallest lie to right of pivot, or pivot
			if (first == pivot)  // infinite loop on [10,20,30,40] when n>1
				++first;
			else
				first = pivot;
		}
		else       // nth smallest lie left of pivot
			last = pivot;  // pivot is the last target value
	}
	return *first;
}
TEST_CASE("find nth smallest", "[NEW]")
{
	SECTION("partition test") {
		vector<int> case0{ 40, 50, 20, 10, 30 };
		auto fourty = my_partition(begin(case0), end(case0), [](int t) { return t < 40; });
		CHECK(*fourty >= 40);
	}
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