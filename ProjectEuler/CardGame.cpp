#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "CardGame.h"
using namespace std;
TEST_CASE("Project Euler #54: Poker hands", "[NEW]")
{
	SECTION("Test Rank") {
		CHECK(Hand(vector<string>{"2D","9C","AS","AH","AC"}) < Hand(vector<string>{"5H","5C","6S","7S","KD"}));
	}
}