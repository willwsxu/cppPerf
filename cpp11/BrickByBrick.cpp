#include "..\catch.hpp"

#include <vector>
#include <iostream>
#include <map>

using namespace std;

namespace std
{	// hash for vector class
	template<typename value_type> struct hash<vector<value_type>>
	{
		typedef vector<value_type> argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& vec) const noexcept
		{
			result_type h1 = 0;
			int shift = 0;
			for (const auto& v : vec) {
				h1 ^= hash<value_type>(v) << shift++;
			}
			return h1;
		}
	};
}
// valid tile patterns of size N
void valid_tile(int N, int len, vector<int>&pattern, vector<vector<int>>& patterns)
{
	if (len >= N) {
		if (len==N)
			patterns.push_back(pattern);
		return;
	}
	pattern.push_back(len + 2);
	valid_tile(N, len + 2, pattern, patterns);
	pattern.back() += 1;
	valid_tile(N, len + 3, pattern, patterns);
	pattern.pop_back();
}

// find all pattern which did not form crack
void neighbor_pattern(const vector<int>& prev_pattern, int len, vector<int>&pattern, map<vector<int>, vector<vector<int>>>& patterns_no_crack) 
{
	if (len >= prev_pattern.back()) {
		if (len == prev_pattern.back())
			patterns_no_crack[prev_pattern].push_back(pattern);
		return;
	}
	if (len+2== prev_pattern.back() || !binary_search(begin(prev_pattern), end(prev_pattern), len + 2)) {
		pattern.push_back(len + 2);
		neighbor_pattern(prev_pattern, len + 2, pattern, patterns_no_crack);
		pattern.pop_back();
	}
	if (len+3==prev_pattern.back() || !binary_search(begin(prev_pattern), end(prev_pattern), len + 3)) {
		pattern.push_back(len + 3);
		neighbor_pattern(prev_pattern, len + 3, pattern, patterns_no_crack);
		pattern.pop_back();
	}
}
long long build_wall(const vector<int>& prev_pattern, int height, map<vector<int>, vector<vector<int>>>& patterns_no_crack, vector<map<vector<int>, long long>>& dp)
{
	if (height == 0)
		return 1;
	auto found = dp[height].find(prev_pattern);
	if (found == end(dp[height])) {
		const auto& patterns = patterns_no_crack[prev_pattern];
		long long result = 0;
		for (const auto& p : patterns) {
			result += build_wall(p, height - 1, patterns_no_crack, dp);
		}
		dp[height][prev_pattern] = result;
		return result;
	}
	return found->second;
}
long long W(int width, int height) 
{
	vector<vector<int>> patterns;
	vector<int> p;
	valid_tile(width, 0, p, patterns);
	map<vector<int>, vector<vector<int>>> patterns_no_crack;
	for (const auto& prev : patterns) {
		neighbor_pattern(prev, 0, p, patterns_no_crack);
	}
	//cout << "no crack done\n";
	long long ways = 0;
	long long small = 99999999999;
	long long large = -9999999999;
	vector<map<vector<int>, long long>> dp(height);
	for (const auto& prev : patterns) {
		auto res = build_wall(prev, height - 1, patterns_no_crack, dp);
		small = min(small, res);
		large = max(large, res);
		ways += res;
	}
	cout << small << " " << large << "\n";
	return ways;
}
TEST_CASE("brick by brick- test", "[TEST]")
{
	vector<vector<int>> patterns;
	vector<int> p;
	valid_tile(9, 0, p, patterns);  // tile patterns with width=9
	CHECK(patterns == vector<vector<int>>{ {2, 4, 6, 9}, { 2,4,7,9 }, { 2,5,7,9 }, { 3,5,7,9 }, { 3,6,9 }});

	patterns.clear();
	valid_tile(10, 0, p, patterns);  // tile patterns with width=10
	CHECK(patterns == vector<vector<int>>{ {2, 4, 6, 8, 10}, { 2,4,7,10 }, { 2,5,7,10 }, { 2,5,8,10 }, { 3,5,7,10 }, {3,5,8,10}, {3,6,8,10}});

	patterns.clear();
	valid_tile(32, 0, p, patterns);  // tile patterns with width=32
	CHECK(patterns.size() == 3329);

	CHECK(W(9, 3) == 8);
	CHECK(W(32, 10) == 806844323190414LL);
}