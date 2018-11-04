#include "stdafx.h"
#include <cassert>
#include <fstream>
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "Graph.h"
#include "helper.h"
#include "myalgo.h"
using namespace std;

// some number in arr are missing from original brr
vector<int> missingNumbers(vector<int> arr, vector<int> brr) {  // easy
	map<int, int> count_a;
	for (int a : arr)
		count_a[a]++;
	set<int> missing;
	for (int b : brr) { // scan original
		if (count_a.count(b) && count_a[b]>0) {
			count_a[b]--;
		}
		else
			missing.insert(b);
	}
	vector<int> ans;
	copy(begin(missing), end(missing), back_inserter(ans));
	return ans;
}
TEST_CASE("Hacker rank search missing number", "[NEW]")
{
	CHECK(missingNumbers(vector<int>{203, 204, 205, 206, 207, 208, 203, 204, 205, 206},
		vector<int>{203, 204, 204, 205, 206, 207, 205, 208, 203, 206, 205, 206, 204}) == vector<int>{204, 205, 206});
}

// find all integer pairs with difference =k, (k>0), all values are unique
int pairs(int k, vector<int> arr) {  // easy binay search
	sort(begin(arr), end(arr));
	int ans = 0;
	for (auto first = begin(arr); first != end(arr); ++first) {
		auto p2 = lower_bound(first + 1, end(arr), *first + k);
		if (p2 == end(arr))
			break;
		if (*p2 - *first == k)
			ans++;
	}
	return ans;
}

// find index of two values in arr, with sum=m
vector<int> icecreamParlor(int m, vector<int> arr) {
	vector<int> sorted{ begin(arr), end(arr) };
	sort(begin(sorted), end(sorted));
	for (auto first = begin(sorted); first != end(sorted); ++first) {
		auto second = lower_bound(first + 1, end(sorted), m - *first);
		if (second != end(arr) && *second + *first == m) {
			auto found_first = find(begin(arr), end(arr), *first);
			auto second_start = *first == *second ? found_first + 1 : begin(arr); //if number same, start next one
			auto found_second = find(second_start, end(arr), *second);
			vector<int> ans{ distance(begin(arr),found_first) + 1, distance(begin(arr),found_second) + 1 };
			sort(begin(ans), end(ans));
			return ans;
		}
	}
	return{};  // should not happen
}

// determine if exist an index that sum of elem before and after are same
string balancedSums(vector<int> arr) {
	partial_sum(begin(arr), end(arr), begin(arr));
	int sum_left = 0;
	int sum_right = 0;
	for (size_t i = 0; i<arr.size(); i++) {
		if (i>0)
			sum_left = arr[i - 1];
		sum_right = i<arr.size() - 1 ? arr.back() - arr[i] : 0;
		if (sum_left == sum_right)
			return "YES";
	}
	return "NO";
}
// minimum loss, find min p[i]-p[j] where i<j, all p[i] distinct
int minimumLoss(vector<long> price) {
	vector<long> sorted_desc{ price[0] };  // as processing each elem, keep them sorted
	sorted_desc.reserve(price.size());
	long min_val = INT32_MAX;
	for (size_t i = 1; i < price.size(); i++) {
		if (price[i] == sorted_desc.back()) // not possible
			return 0;
		else if (price[i] < sorted_desc.back()) {
			min_val = min(min_val, sorted_desc.back() - price[i]);
			sorted_desc.push_back(price[i]);
		}
		else { // find a larger value see before
			auto larger = upper_bound(rbegin(sorted_desc), rend(sorted_desc), price[i]);
			if (larger == rend(sorted_desc)) // largest
				sorted_desc.insert(begin(sorted_desc), price[i]);
			else {
				min_val = min(min_val, *larger - price[i]);
				sorted_desc.insert(larger.base(), price[i]);
			}
		}
	}
	return min_val;
}
TEST_CASE("Hacker rank search min loss", "[NEW]")
{
	CHECK(minimumLoss(vector<long>{5, 10, 7, 6}) == 1);
	CHECK(minimumLoss(vector<long>{20, 7, 8, 2, 5}) == 2);
}