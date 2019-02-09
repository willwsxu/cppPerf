#include "..\catch.hpp"

#include <vector>
#include <iostream>
#include <iterator>
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

#include "..\plain\xr\brickbybrick.cpp"

TEST_CASE("brick by brick- test", "[XR]")
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
