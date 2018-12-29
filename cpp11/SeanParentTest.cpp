#include "seanparentBetterCode.h"

#include "..\catch.hpp"

#include <vector>
#include <iterator>
#include <functional>
#include "myalgo.h"

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

TEST_CASE("sort subrange", "[SUBR]")
{
	vector<int>  target{ 1,2,3,4,5,6,7,8,9 };
	WXU::shuffle(target);
	sort_subrange(begin(target), end(target), begin(target) + 3, begin(target) + 6);
	vector<int> ans;
	copy(begin(target)+3, begin(target) + 6, back_inserter(ans));
	CHECK(ans == vector<int>{4,5,6});
}

TEST_CASE("slide", "[SLIDE]")
{
	vector<int>  target{ 1,2,3,4,5,6,7,8,9 };
	slide(begin(target)+3, begin(target) + 6, begin(target)); // slide up
	CHECK(target == vector<int>{4, 5, 6,1,2,3,7,8,9});

	slide(begin(target), begin(target) + 3, begin(target)+6); // slide down
	CHECK(target == vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9});
}

TEST_CASE("gather", "[NEW]")
{
	vector<int>  target{ 1,2,3,4,5,6,7,8,9 };
	function<bool(int)> comp=[](const int&x) {return x % 2 == 0; }; // not1 requires function, not lambda
	auto a = std::stable_partition(begin(target), begin(target) + 4, comp);
	gather(begin(target), end(target), begin(target) + 4,comp);
	CHECK(target == vector<int>{1, 3, 2, 4, 6, 8, 5, 7, 9});
}

TEST_CASE("gather2 lambda friendly", "[NEW]")
{
	vector<int>  target{ 1,2,3,4,5,6,7,8,9 };
	auto comp = [](const int&x) {return x % 2 == 0; };
	auto a = std::stable_partition(begin(target), begin(target) + 4, comp);
	gather2(begin(target), end(target), begin(target) + 4, comp);
	CHECK(target == vector<int>{1, 3, 2, 4, 6, 8, 5, 7, 9});
}