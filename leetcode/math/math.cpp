#include <string>
#include <map>

using namespace std;
//1041 robot started at origin, facing N. Given instructions
// G move a step, L turn left, R turn right
// if the instruction is repeated, is robot bounded
bool isRobotBounded(string instructions) {
	int L = 0, R = 0, U = 0, D = 0;
	int x = 0, y = 0;
	char dir = 'N';
	map<char, char> turns_L{ {'N','W'},{'W','S'},{'S','E'},{'E','N'} };
	map<char, char> turns_R{ {'N','E'},{'W','N'},{'S','W'},{'E','S'} };
	for (int r = 0; r < 20; r++) {
		bool no_change = true;
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

#include "catch.hpp"
TEST_CASE("1041 robot bounded", "[MATH]")
{
	CHECK(isRobotBounded("LLGLR") == true);
}