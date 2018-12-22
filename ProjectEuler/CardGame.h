#pragma once
#include <string>
#include <vector>
#include <algorithm>

class Card
{
	enum Suit{CLUB, DIAMOND, HEART, SPADE} suit;
	int card_val;
	std::string name;
	friend class Hand;
public:
	Card(std::string c):name(move(c)) {
		switch (name[0]) {
		case 'T':	card_val = 10;	break;
		case 'J':	card_val = 11;	break;
		case 'Q':	card_val = 12;	break;
		case 'K':	card_val = 13;	break;
		case 'A':	card_val = 14;	break;
		default:	card_val = name[0] - '0';	break;
		}
		switch (name[1]) {
		case 'C':	suit = CLUB;	break;
		case 'D':	suit = DIAMOND;	break;
		case 'H':	suit = HEART;	break;
		case 'S':	suit = SPADE;	break;
		}
	}
};
class Hand {
	std::vector<Card>  hand;  // 5 cards in a hand
	enum Rank{High_CARD, ONE_PAIR, TWO_PAIR, THREE, STRAIGHT, FLUSH, FULL_HOUSE, FOUR, STRAIGHT_FLUSH} rank;
	void eval_straight() {

	}
	std::vector<int> high_cards;
	friend bool operator<(const Hand& h1, const Hand& h2);
public:
	Hand(std::vector<std::string>&& h) {
		for (const auto& s : h)
			hand.push_back(Card(s));
	}
private:
	void eval()
	{
		std::sort(begin(hand), end(hand), [](const auto& c1, const auto&c2) { return c1.card_val < c2.card_val; });
		auto straight = std::adjacent_find(begin(hand), end(hand),
			[](const auto& c1, const auto&c2) { return c2.card_val - c1.card_val != 1; });
		bool bFlush = std::adjacent_find(begin(hand), end(hand),
			[](const auto& c1, const auto&c2) { return c2.suit != c1.suit; }) == end(hand);
		if (straight == end(hand)) {
			rank = STRAIGHT;
			high_cards.push_back(hand.back().card_val);
		}
		else if ((++straight)->card_val == 14 && begin(hand)->card_val == 2) {  // treat A as 1
			rank = STRAIGHT;
			high_cards.push_back(5);
		}
		if (rank == STRAIGHT && bFlush) {
			rank = STRAIGHT_FLUSH;
			return;
		}
		if (bFlush) {
			rank = FLUSH;
			high_cards.reserve(hand.size());
			for (const auto& c : hand)  // use all cards as kicker
				high_cards.push_back(c.card_val);
			reverse(begin(high_cards), end(high_cards));
			return;
		}
		std::vector<std::pair<int, int>> same_kind;  // scan for same kind of cards
		hand.push_back(Card("1C")); // insert sentinel to simplify scanning of card
		int same = 0;
		int prev = 0;
		for (const auto& c : hand) {
			if (c.card_val == prev)
				same++;
			else {
				if (same > 1) {
					same_kind.emplace_back(same, prev);  // 2 or 3 or 4 a kind
				}
				else if (same==1) {
					high_cards.push_back(prev);  // kicker
				}
				same = 1;
				prev = c.card_val;
			}
		}
		switch (same_kind.size()) {
		case 0:		rank = High_CARD;	reverse(begin(high_cards), end(high_cards));	break;
		case 1:
			switch (same_kind.front().first) {
			case 2:	rank = ONE_PAIR;	break;
			case 3:	rank = THREE;		break;
			case 4: rank = FOUR;		break;
			}
			high_cards.push_back(same_kind.front().second);
			reverse(begin(high_cards), end(high_cards));
			break;
		case 2:
			std::sort(begin(same_kind), end(same_kind), [](const auto&p1, const auto&p2) {
				if (p1.first == p2.first)  
					return p1.second > p2.second; // 2 pairs, sort high to low value
				return p1.first > p2.first;  // sort three a kind first
			});
			if (same_kind.front().first == 3)
				rank = FULL_HOUSE;
			else
				rank = TWO_PAIR;
			high_cards.push_back(same_kind.back().second);
			high_cards.push_back(same_kind.front().second);
			reverse(begin(high_cards), end(high_cards));
		}
	}
};
bool operator<(const Hand& h1, const Hand& h2) {
	if (h1.rank != h2.rank)
		return h1.rank < h2.rank;
	return h1.high_cards < h2.high_cards;
}