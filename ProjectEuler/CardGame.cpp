#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "CardGame.h"
using namespace std;
TEST_CASE("Project Euler #54: Poker hands", "[NEW]")
{
	SECTION("Test Rank") {
		CHECK(Hand(vector<string>{"5D","8C","9S","JS","AC"}).get()==High_CARD);
		CHECK(Hand(vector<string>{"5H", "5C", "6S", "7S", "KD"}).get()==ONE_PAIR);
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "7S", "3D"}).get()==TWO_PAIR);
		CHECK(Hand(vector<string>{"2D", "9C", "AS", "AH", "AC"}).get()==THREE);
		CHECK(Hand(vector<string>{"2D", "4H", "5C", "3S", "AH"}).get() == STRAIGHT);
		CHECK(Hand(vector<string>{"2D", "4D", "5D", "8D", "AD"}).get() == FLUSH);
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "5S", "3D"}).get() == FULL_HOUSE);
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "5S", "5D"}).get() == FOUR);
		CHECK(Hand(vector<string>{"JD", "TD", "QD", "KD", "AD"}).get() == STRAIGHT_FLUSH);
	}
	SECTION("Test High card") {
		CHECK(Hand(vector<string>{"5D", "7C", "9S", "JS", "AC"}) < Hand(vector<string>{"5D", "8C", "9S", "JS", "AC"}));
	}
	SECTION("Test two pairs") {
		CHECK(Hand(vector<string>{"4H", "4C", "3S", "7S", "3D"}) < Hand(vector<string>{"6H", "6C", "2S", "7S", "2D"}));
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "7S", "3D"}) < Hand(vector<string>{"5H", "5C", "4S", "7S", "4D"}));
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "7S", "3D"}) < Hand(vector<string>{"5H", "5C", "3S", "8S", "3D"}));
	}
	SECTION("Hacker rank test cases") {
		CHECK(Hand(vector<string>{"5H", "5C", "6S", "7S", "KD"}) < Hand(vector<string>{"2C","3S","8S","8D","TD"}));
	}
}