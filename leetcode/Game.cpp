#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std; 

class Game {
public:
	// each turn can pick 1 or 2 or 3 stones
	bool canWinNim(int n) { // beat 100%
		return n % 4 != 0;
	}
};

int pick = 6;
int guess(int num)
{
	return num==pick?0:pick>num?1:-1;
}

class GuessNumber {
	int guessNumber(int low, int high) {  // binary search
		if (low >= high)
			return low;  // should not happen
		int mid = low/2+high/2;  // !!avoid overflow when low+high>MAX_INT
		int hint = guess(mid);
		if (hint == 0)
			return mid;
		else if (hint > 0)
			return guessNumber(mid + 1, high);
		else
			return guessNumber(low, mid - 1);
	}
public:
	// 374. Guess Number Higher or Lower, [1,n]
	int guessNumber(int n) {  // beat 100%
		return guessNumber(1, n);
	}
};

TEST_CASE("guess a number", "[NEW]")
{	
	pick = 6;
	CHECK(GuessNumber().guessNumber(10)== pick);
	pick = 1702766719;
	CHECK(GuessNumber().guessNumber(2126753390) == pick);
}