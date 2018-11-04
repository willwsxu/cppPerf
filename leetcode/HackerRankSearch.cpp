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
	int rows = matrix.size();
	if (rows == 0)
		return 0;
	int cols = matrix[0].size();
	auto empty_cell = [](int val) { return val == 0; };
	vector<set<int>> components;
	set<int>		visited;
	vector<vector<int>> dir{ {0,-1}, {-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1} };  // left, top left, top, top right
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if (!empty_cell(matrix[r][c]) && !visited.count(r*cols + c)) {
				deque<pair<int, int>> bfs{ {r,c} };
				components.push_back(set<int>{});
				while (!bfs.empty()) {
					int idx = bfs.front().first*cols + bfs.front().second;  // process front
					components.back().insert(idx);
					visited.insert(idx);
					for (const auto& d : dir) {  // check 8 directions
						int r1 = bfs.front().first + d[0];
						int c1 = bfs.front().second + d[1];
						if (r1 >= 0 && r1 < rows &&c1 >= 0 && c1 < cols && !empty_cell(matrix[r1][c1])) {
							if (!visited.count(r1*cols + c1))
								bfs.emplace_back(r1, c1);
						}
					}
					bfs.pop_front();
				}
			}
		}
	}
	return max_element(begin(components), end(components), [](const auto& c1, const auto&c2) { return c1.size() < c2.size(); })->size();
}
TEST_CASE("Hacker rank search Connected Cells in a Grid", "[NEW]")
{
	vector<vector<int>> matrix{ { 1,1,0,0,0 },{ 0,1,1,0,0 },{ 0,0,1,0,1 },{ 1,0,0,0,1 },{ 0,1,0,1,1 } };
	CHECK(connectedCell(matrix)==5);
}