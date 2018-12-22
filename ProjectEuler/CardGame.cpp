#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "CardGame.h"
using namespace std;
TEST_CASE("Project Euler #54: Poker hands", "[NEW]")
{
	SECTION("Test Rank") {
		CHECK(Hand(vector<string>{"5D","8C","9S","JS","AC"}).get()==High_CARD);
		CHECK(Hand(vector<string>{"5D", "8C", "9S", "JS", "AC"}).get_kicker() == vector<int>{14,11,9,8,5});
		CHECK(Hand(vector<string>{"5H", "5C", "6S", "7S", "KD"}).get()==ONE_PAIR);
		CHECK(Hand(vector<string>{"5H", "5C", "6S", "7S", "KD"}).get_kicker() == vector<int>{5, 13, 7,6});
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "7S", "3D"}).get()==TWO_PAIR);
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "7S", "3D"}).get_kicker() == vector<int>{5,3,7});
		CHECK(Hand(vector<string>{"2D", "9C", "AS", "AH", "AC"}).get()==THREE);
		CHECK(Hand(vector<string>{"2D", "9C", "AS", "AH", "AC"}).get_kicker() == vector<int>{14, 9, 2});
		CHECK(Hand(vector<string>{"2D", "4H", "5C", "3S", "AH"}).get() == STRAIGHT);
		CHECK(Hand(vector<string>{"2D", "4H", "5C", "3S", "AH"}).get_kicker() == vector<int>{5});
		CHECK(Hand(vector<string>{"2D", "4D", "5D", "8D", "AD"}).get() == FLUSH);
		CHECK(Hand(vector<string>{"2D", "4D", "5D", "8D", "AD"}).get_kicker() == vector<int>{14,8,5,4,2});
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "5S", "3D"}).get() == FULL_HOUSE);
		CHECK(Hand(vector<string>{"5H", "5C", "QS", "5S", "QD"}).get_kicker() == vector<int>{5,12});
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "5S", "5D"}).get() == FOUR);
		CHECK(Hand(vector<string>{"5H", "5C", "TS", "5S", "5D"}).get_kicker() == vector<int>{5,10});
		CHECK(Hand(vector<string>{"JD", "TD", "QD", "KD", "AD"}).get() == STRAIGHT_FLUSH);
		CHECK(Hand(vector<string>{"JD", "TD", "QD", "KD", "AD"}).get_kicker() == vector<int>{14});
	}
	SECTION("Test High card") {
		CHECK(Hand(vector<string>{"6D", "8C", "9S", "QS", "KC"}) < Hand(vector<string>{"5D", "7C", "8S", "JS", "AC"}));
		CHECK(Hand(vector<string>{"6D", "8C", "9S", "QS", "AC"}) < Hand(vector<string>{"5D", "7C", "8S", "KS", "AC"}));
		CHECK(Hand(vector<string>{"6D", "8C", "9S", "QS", "AC"}) < Hand(vector<string>{"5D", "7C", "TS", "QS", "AC"}));
		CHECK(Hand(vector<string>{"6D", "7C", "9S", "QS", "AC"}) < Hand(vector<string>{"5D", "8C", "9S", "QS", "AC"}));
		CHECK(Hand(vector<string>{"6D", "8C", "9S", "QS", "AC"}) < Hand(vector<string>{"7D", "8C", "9S", "QS", "AC"}));
	}
	SECTION("Test flush") {
		CHECK(Hand(vector<string>{"2D", "4D", "5D", "QD", "KD"}) < Hand(vector<string>{"2D", "4D", "5D", "8D", "AD"}));
		CHECK(Hand(vector<string>{"2D", "4D", "6D", "8D", "AD"}) < Hand(vector<string>{"2D", "4D", "5D", "9D", "AD"}));
		CHECK(Hand(vector<string>{"2D", "5D", "6D", "8D", "AD"}) < Hand(vector<string>{"2D", "4D", "7D", "8D", "AD"}));
		CHECK(Hand(vector<string>{"3D", "4D", "6D", "8D", "AD"}) < Hand(vector<string>{"2D", "5D", "6D", "8D", "AD"}));
		CHECK(Hand(vector<string>{"2D", "4D", "5D", "8D", "AD"}) < Hand(vector<string>{"3D", "4D", "5D", "8D", "AD"}));
	}
	SECTION("Test straight") {
		CHECK(Hand(vector<string>{"2D", "4H", "5C", "3S", "AH"}) < Hand(vector<string>{"2D", "4H", "5C", "3S", "6H"}));
		CHECK(Hand(vector<string>{"2D", "4H", "5C", "3S", "AH"}) < Hand(vector<string>{"TD", "QH", "KC", "JS", "AH"}));
	}
	SECTION("Test straight flush") {
		CHECK(Hand(vector<string>{"JD", "TD", "QD", "KD", "9D"}) < Hand(vector<string>{"JD", "TD", "QD", "KD", "AD"}));
	}
	SECTION("Test one pairs") {
		CHECK(Hand(vector<string>{"5H", "5C", "6S", "7S", "AD"}) < Hand(vector<string>{"8H", "8C", "6S", "7S", "KD"}));
		CHECK(Hand(vector<string>{"5H", "5C", "6S", "8S", "KD"}) < Hand(vector<string>{"5H", "5C", "6S", "7S", "AD"}));
		CHECK(Hand(vector<string>{"5H", "5C", "6S", "7S", "KD"}) < Hand(vector<string>{"5H", "5C", "4S", "8S", "KD"}));
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "7S", "KD"}) < Hand(vector<string>{"5H", "5C", "6S", "7S", "KD"}));
	}
	SECTION("Test two pairs") {
		CHECK(Hand(vector<string>{"4H", "4C", "3S", "8S", "3D"}) < Hand(vector<string>{"6H", "6C", "2S", "7S", "2D"}));
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "8S", "3D"}) < Hand(vector<string>{"5H", "5C", "4S", "7S", "4D"}));
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "7S", "3D"}) < Hand(vector<string>{"5H", "5C", "3S", "8S", "3D"}));
	}
	SECTION("Test three of a kind") {
		CHECK(Hand(vector<string>{"2D", "TC", "3S", "3H", "3C"}) < Hand(vector<string>{"2D", "9C", "AS", "AH", "AC"}));
		CHECK(Hand(vector<string>{"3D", "9C", "AS", "AH", "AC"}) < Hand(vector<string>{"2D", "TC", "AS", "AH", "AC"}));
		CHECK(Hand(vector<string>{"2D", "9C", "AS", "AH", "AC"}) < Hand(vector<string>{"3D", "9C", "AS", "AH", "AC"}));
	}
	SECTION("Test four of a kind") {
		CHECK(Hand(vector<string>{"5H", "5C", "4S", "5S", "5D"}) < Hand(vector<string>{"AH", "AC", "3S", "AS", "AD"}) );
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "5S", "5D"}) < Hand(vector<string>{"5H", "5C", "4S", "5S", "5D"}));
	}
	SECTION("Test full house") {
		CHECK(Hand(vector<string>{"5H", "5C", "8S", "5S", "8D"}) < Hand(vector<string>{"6H", "6C", "3S", "6S", "3D"}));
		CHECK(Hand(vector<string>{"5H", "5C", "3S", "5S", "3D"}) < Hand(vector<string>{"5H", "5C", "4S", "5S", "4D"}));
	}
	SECTION("Hacker rank test cases") {
		CHECK(Hand(vector<string>{"5H", "5C", "6S", "7S", "KD"}) < Hand(vector<string>{"2C","3S","8S","8D","TD"}));
		CHECK(Hand(vector<string>{"2C","5C","7D","8S","QH"}) < Hand(vector<string>{"5D", "8C", "9S", "JS", "AC"}));
		CHECK(Hand(vector<string>{"2D", "9C", "AS", "AH", "AC"}) < Hand(vector<string>{"2D", "4D", "5D", "8D", "AD"}));
		CHECK(Hand(vector<string>{"3D", "3S", "3H", "8D", "8C"}) < Hand(vector<string>{"2D", "2H", "4C", "4D", "4S"}));
	}
}