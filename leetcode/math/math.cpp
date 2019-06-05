#include <string>
#include <map>

using namespace std;
//1041 robot started at origin, facing N. Given instructions
// G move a step, L turn left, R turn right
// if the instruction is repeated, is robot bounded
bool isRobotBounded(string instructions) {
	int L = 0, R = 0, U = 0, D = 0;  // bounds
	int x = 0, y = 0;  // location
	char dir = 'N';
	map<char, char> turns_L{ {'N','W'},{'W','S'},{'S','E'},{'E','N'} };
	map<char, char> turns_R{ {'N','E'},{'W','N'},{'S','W'},{'E','S'} };
	for (int r = 0; r < 20; r++) {
		bool no_change = true;  // compute bounds in each round and check if bound is changing
		auto update = [&no_change](int& max_val, int new_val) {
			if (new_val > max_val) {
				max_val = new_val;
				no_change = false;
			}
		};
		for (char m : instructions) {
			switch (m) {
			case 'G':
				switch (dir) {
				case 'N': y++;  break;
				case 'S': y--;  break;
				case 'E': x++;  break;
				case 'W': x--;  break;
				}
				update(L, -x);
				update(R, x);
				update(U, y);
				update(D, -y);
				break;
			case 'L':
				dir = turns_L[dir]; break;
			case 'R':
				dir = turns_R[dir]; break;
			}
		}
		if (no_change)
			return true;
	}
	return false;
}

#include <vector>
#include <iostream>
#include <algorithm>
// swap two numbers to generate largest permutation that is smaller than current
// numbers in array may not be unique
// if no swap is possible, return original
// idea: search from back and find the first number that is increasing, say it is target1
//       search left-to-right increasing sequence after target1, find target2<target1 (gurranteed if target1 exists)
//       swap(target1, target2)
vector<int> prevPermOpt1(vector<int>& A) {
	auto increasing_from_back = adjacent_find(rbegin(A), rend(A), [](int a, int b) { return a < b; });
	if (increasing_from_back != rend(A)) {
		auto start = increasing_from_back.base()-1;  // base point to to right, find start of increasing sequence
		auto target1 = start - 1;  // target1>start, *target1>0
		auto target2 = lower_bound(start, end(A), *target1 - 1);  // find largest digit smaller than target1
		if (target2==end(A) || *target2 >= *target1)  // did not find the exact value (*target1-1)
			target2 = lower_bound(start, end(A), *(target2 - 1)); // search for first of value target2
		iter_swap(target1, target2);
	}
	return A;
}
#include "catch.hpp"
TEST_CASE("1041 robot bounded", "[MATH]")
{
	CHECK(isRobotBounded("LLGLR") == true);
}

TEST_CASE("pre permutation", "[MATH]")
{
	CHECK(prevPermOpt1(vector<int>{1,2,3}) == vector<int>{1,2,3});
	CHECK(prevPermOpt1(vector<int>{3, 2, 1}) == vector<int>{3, 1,2});
	CHECK(prevPermOpt1(vector<int>{3, 3,1}) == vector<int>{3, 1, 3});  // 
	CHECK(prevPermOpt1(vector<int>{3,1,1,3}) == vector<int>{1,3,1,3});
	CHECK(prevPermOpt1(vector<int>{1,9,4,6,7}) == vector<int>{1,7,4,6,9});
}