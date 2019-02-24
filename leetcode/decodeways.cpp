#include <string>

using namespace std;
// 91. Decode Ways
// number 1 - 26 map to letter A-Z. Given a string to digits, find ways to decode into letters

class DP {
public:
	int numDecodings(string s) {  // bottom dp
		// scan backwards
		auto scanner = rbegin(s);
		int current = *scanner=='0'?0:1;  // '0' not valid
		int prev= 1, prev_prev=0;  // base case is important
		while (++scanner != rend(s)) {
			prev_prev = prev;
			prev = current;
			if (*scanner == '0')
				current = 0;
			else {
				int val_2digit = *scanner - '0';
				val_2digit = val_2digit * 10 + *(scanner - 1) - '0';
				current = prev;
				if (val_2digit < 27) // two ways to decode if first digit is 1 or 2
					current += prev_prev;
			}
		}
		return current;
	}
};

#include "..\catch.hpp"  // don't put this file in stdafx.h
TEST_CASE("facebook interview test", "[NEW]")
{
	CHECK(DP().numDecodings("12") == 2);
	CHECK(DP().numDecodings("10") == 1);
	CHECK(DP().numDecodings("1010") == 1);
	CHECK(DP().numDecodings("0") == 0);
	CHECK(DP().numDecodings("100") == 0);
}