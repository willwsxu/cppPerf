#include "..\catch.hpp"

#include <vector>

using namespace std;

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


TEST_CASE("XR brick by brick- test", "[TEST]")
{
	vector<vector<int>> patterns;
	vector<int> p;
	valid_tile(9, 0, p, patterns);  // tile patterns with len=9
	CHECK(patterns == vector<vector<int>>{ {2, 4, 6, 9}, { 2,4,7,9 }, { 2,5,7,9 }, { 3,5,7,9 }, { 3,6,9 }});
}