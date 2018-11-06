#include "stdafx.h"
#include <cassert>
#include <fstream>
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "Graph.h"
#include "helper.h"
#include "myalgo.h"
#include "UnionFind.h"
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
template<typename T>
int minimumLoss(vector<T>& price) {
	set<T> sorted_desc{ price[0] };  // as processing each elem, keep them sorted
	T min_loss = INT32_MAX;
	for (size_t i = 1; i < price.size(); i++) {
		if (price[i] < *sorted_desc.begin()) {
			min_loss = min(min_loss, *sorted_desc.begin() - price[i]);
			sorted_desc.insert(begin(sorted_desc), price[i]);
		}
		else { // find a larger value see before
			auto larger = sorted_desc.upper_bound(price[i]);  // map upper_bound solved TLE 
			if (larger != end(sorted_desc)) // largest
				min_loss = min(min_loss, *larger - price[i]);
			sorted_desc.insert(larger, price[i]);
		}
	}
	return static_cast<int>(min_loss);
}
TEST_CASE("Hacker rank search min loss", "[SRCH]")
{
	CHECK(minimumLoss(vector<long>{5, 10, 7, 6}) == 1);
	CHECK(minimumLoss(vector<long>{20, 7, 8, 2, 5}) == 2);
	ifstream fin("minimumloss.txt");
	int N;
	fin >> N;
	vector<int64_t> v;
	v.reserve(N);
	for (int i = 0; i < N; i++) {
		int64_t p;
		fin >> p;
		v.push_back(p);
	}
	CHECK(minimumLoss(v) == 47175);
}

// Connected Cells in a Grid, return size of largest region
int connectedCell(vector<vector<int>> matrix) {
	vector<vector<int>> dir{ { 0,-1 },{ -1,-1 },{ -1,0 },{ -1,1 },{ 0,1 },{ 1,1 },{ 1,0 },{ 1,-1 } };  // left, top left, top, top right
	vector<set<int>> components = connectedComponents(matrix, dir);
	if (components.empty())
		return 0;
	return max_element(begin(components), end(components), [](const auto& c1, const auto&c2) { return c1.size() < c2.size(); })->size();
}
TEST_CASE("Hacker rank search Connected Cells in a Grid", "[NEW]")
{
	vector<vector<int>> matrix{ { 1,1,0,0,0 },{ 0,1,1,0,0 },{ 0,0,1,0,1 },{ 1,0,0,0,1 },{ 0,1,0,1,1 } };
	CHECK(connectedCell(matrix)==5);
}
int hackerlandRadioTransmitters(vector<int> x, int k) {
	sort(begin(x), end(x));
	auto start = begin(x); // greedy method, add transmitter from near to far
	int count = 0;
	while (start != end(x)) {
		count++;
		int target = *start + k;
		auto last = equal_range(start, end(x), target);
		if (last.first != last.second || distance(start, last.first) > 1) {  // find a middle house to put on transmitter
			start = last.second - 1;
			last = equal_range(start, end(x), *start + k);
		}  // or transmitter on this house
		start = last.second;
	}
	return count;
}
TEST_CASE("Hacker rank search transmitter", "[NEW]")
{
	CHECK(hackerlandRadioTransmitters(vector<int>{9, 5, 4, 2, 6, 15, 12}, 2) == 4);
}

// n x m grid, k horizontal train tracks
// find total cells, not occupied by tracks
long long gridlandMetro(int n, int m, int k, vector<vector<int>> track) {
	map<int, map<int, int>>  tracks;  //keep tracks of start and stop, start 1, stop -1
	for (const auto& t : track) {
		tracks[t[0]][t[1]] += 1;
		tracks[t[0]][t[2]+1] -= 1;
	}
	long long track_cells = 0;
	for (const auto& tr : tracks) {
		int start = 0;
		int start_pos = -1;
		for (const auto& stop : tr.second) {
			start += stop.second;
			if (start > 0 && start_pos<0)
				start_pos = stop.first;
			else if (start == 0 && start_pos > 0) {
				track_cells += stop.first - start_pos;
				start_pos = -1;
			}
		}
	}
	return n*m - track_cells;
}
TEST_CASE("Hacker rank search grid land metro", "[NEW]")
{
	CHECK(gridlandMetro(4, 4, 3, vector<vector<int>>{ {2,2,3}, {3,1,4}, {4,4,4}}) == 9);
}