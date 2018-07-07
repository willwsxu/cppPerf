#include "seanparentAlgorithm.h"

#include <vector>
#include <iterator>
#include "..\catch.hpp"

using namespace std;

TEST_CASE("stable partition position", "[NEW]")
{
	vector<int>  target{ 1,2,3,4,5,6,7,8,9 };
	vector<int>  sq    { 1,0,0,1,0,0,0,0,1 };
	auto x = stable_partition_position(begin(target), end(target), [&sq,&target](auto p) {
		return *(begin(sq) + (p - begin(target))); });
	vector<int> ans;
	copy(begin(target), x, back_inserter(ans));
	CHECK(ans == vector<int>{1, 4, 9});
}