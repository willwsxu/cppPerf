#include <string>

using namespace std;
// 91. Decode Ways
// number 1 - 26 map to letter A-Z. Given a string to digits, find ways to decode into letters

class Solution {
public:
	int numDecodings(string s) {
		// scan backwards
		auto scanner = rbegin(s);
		int current = *scanner=='0'?0:1;  // '0' not valid
		int prev= 0, prev_prev=0;
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