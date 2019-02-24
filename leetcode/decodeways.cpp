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
	//639. Decode Ways II, add wild card * which can be [1,9]
	int numDecodings2(string s) {
		static const int MOD = 1000000007;
		auto scanner = rbegin(s);  // scan backwards
		auto get_count = [](char c) {  // one digit
			switch (c) {
			case '0': 				return 0;
			case '*': 				return 9;
			default: 				return 1;
			}
		};
		long long current = get_count(*scanner);  // '0' not valid
		long long prev = 1, prev_prev = 0;  // base case is important
		auto get_count_wild = [&prev_prev, &prev](int c1, int c2) {  // c1 is [1,9]
			auto get_count2 = [](char c) {  // c is first digit, second is *
				switch (c) {
				case '1': 				return 9;
				case '2': 				return 6;
				default: 				return 0;
				}
			};
			long long ways = 0;
			if (c2 == '*') {
				ways =(long long) get_count2(c1) * prev_prev;
			}
			else {
				int val_2digit = c1 - '0';
				val_2digit = val_2digit * 10 + c2 - '0';
				if (val_2digit < 27) // two ways to decode if first digit is 1 or 2
					ways = prev_prev;
			}
			ways += prev;
			return ways%MOD;
		};
		while (++scanner != rend(s)) {
			prev_prev = prev;
			prev = current;
			current = 0;
			switch (*scanner) {
			case '0':	current = 0;	break;
			case '*':
				for (int i = 1; i < 10; i++)
					current += get_count_wild(i + '0', *(scanner - 1));
			default:
				current += get_count_wild(*scanner, *(scanner - 1));
			}
			current %= MOD;
		}
		return static_cast<int>(current);
	}
};

#include "..\catch.hpp"  // don't put this file in stdafx.h
TEST_CASE("decode digit to letter with wild card test", "[NEW]")
{
	CHECK(DP().numDecodings2("**") == 96);
	CHECK(DP().numDecodings2("1*") == 18);
	CHECK(DP().numDecodings2("2*") == 15);
	CHECK(DP().numDecodings2("3*") == 9);
}

TEST_CASE("facebook interview test", "[NEW]")
{
	CHECK(DP().numDecodings("12") == 2);
	CHECK(DP().numDecodings("10") == 1);
	CHECK(DP().numDecodings("1010") == 1);
	CHECK(DP().numDecodings("0") == 0);
	CHECK(DP().numDecodings("100") == 0);
}