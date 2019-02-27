#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;
vector<string> get_tokens(string&& data)
{
	istringstream strm(move(data));
	vector<string> fields;
	for (string token; getline(strm, token, '|'); ) {
		fields.push_back(token);
	}
	return fields;
}

class RegionalBook
{
	struct Quote
	{
		int bid;
		int ask;
		Quote(int b=0, int a=0) : bid(b), ask(a) {}
	};
	map<string, map<string, Quote>> book;
public:
	void update_message(string&& msg) {
		auto parsed = get_tokens(move(msg));
		if (parsed[0] != "Q")
			return;
		book[parsed[2]][parsed[1]] = Quote(stoi(parsed[3]), stoi(parsed[4]));
	}
	void display()
	{
		for (const auto& sym : book) {
			cout << "Symbol: " << sym.first << "\n";
			map<int, string, greater<int>> bids;
			map<int, string> asks;
			for (const auto& ex : sym.second) {
				bids[ex.second.bid] = ex.first;
				asks[ex.second.ask] = ex.first;
			}
			istringstream two_sides;
			auto a = begin(asks);
			for (auto b = begin(bids); b != end(bids); ++b) {
				cout << b->second << " " << b->first << " " << a->first << " " << a->second << "\n";
			}
		}
	}
};

// support order operation add, modify, delete
// find inside quotes, with aggregated size
class OrderTiers {  // for one symbol
	struct OrderMsg {
		int64_t		price;
		int64_t		size;
		char		side; // B, S
	};
	unordered_map<int64_t, OrderMsg> order_book;
	map<int64_t, int64_t, greater<int64_t>>  bids;
	map<int64_t, int64_t>  asks;
public:
	void add_order(int64_t id, const OrderMsg& order)
	{
		order_book[id] = order;
		if (order.side == 'B')
			bids[order.price] += order.size;
		else
			asks[order.price] += order.size;
	}
	bool modify_order(int64_t id, const OrderMsg& order) {  // no side info in modify
		auto old = order_book.find(id);
		if (old == end(order_book))  // original order not found
			return false;
		auto update_inside = [](auto& quote, const OrderMsg& original, const OrderMsg& modify) {
			// easy but less efficient way
			auto& old_ref = quote[original.price];
			old_ref -= original.size;
			quote[modify.price] += modify.size;  // 2 map look up regard less price is changed or not
			if (old_ref == 0)
				quote.erase(original.price);
		};
		if (old->second.side == 'B')
			update_inside(bids, old->second, order);
		else
			update_inside(asks, old->second, order);
		old->second.price = order.price;
		old->second.size = order.size;
		return true;
	}
	bool delete_order(int64_t id)
	{
		auto old = order_book.find(id);
		if (old == end(order_book))  // original order not found
			return false;
		auto del = [](auto& quote, const OrderMsg& original) {
			// easy but less efficient way
			auto& old_ref = quote[original.price];
			old_ref -= original.size;
			if (old_ref == 0)
				quote.erase(original.price);
		};
		if (old->second.side == 'B')
			del(bids, old->second);
		else
			del(asks, old->second);
		order_book.erase(id);
		return true;
	}
	tuple<int64_t, int64_t, bool> get_best_bid() {
		if (bids.empty())
			return { 0,0,false };
		auto best = begin(bids);
		return { best->first, best->second, true };
	}
	tuple<int64_t, int64_t, bool> get_best_ask() {
		if (asks.empty())
			return { 0,0,false };
		auto best = begin(asks);
		return { best->first, best->second, true };
	}
};

#include "..\catch.hpp"
TEST_CASE("Peak6 data feed test", "[FEED]")
{
	RegionalBook book;
	book.update_message("Q|NYS|IBM|99|105|");
	book.update_message("Q|NAS|IBM|98|102|");
	book.update_message("Q|ARC|IBM|101|103|");
	book.display();
}

TEST_CASE("XR trading data feed test", "[NEW]")
{
	OrderTiers orders;
	orders.add_order(1, { 50, 100, 'B' });
	auto best_ask = orders.get_best_ask();
	CHECK(get<2>(best_ask) == false);
	orders.modify_order(1, { 50, 200, 0 });
	auto best_bid = orders.get_best_bid();
	CHECK(get<2>(best_bid) == true);
	CHECK(get<0>(best_bid) == 50);
	CHECK(get<1>(best_bid) == 200);

	orders.add_order(2, { 52, 300, 'B' });
	best_bid = orders.get_best_bid();
	CHECK(get<0>(best_bid) == 52);
	orders.modify_order(2, { 51, 400, 'B' });
	best_bid = orders.get_best_bid();
	CHECK(get<0>(best_bid) == 51);
	CHECK(get<1>(best_bid) == 400);
	orders.delete_order(2);
	best_bid = orders.get_best_bid();
	CHECK(get<0>(best_bid) == 50);
	CHECK(get<1>(best_bid) == 200);

	orders.delete_order(1);
	best_bid = orders.get_best_bid();
	CHECK(get<2>(best_bid) == false);

	orders.add_order(1, { 60, 100, 'S' });
	orders.add_order(2, { 65, 500, 'S' });
	orders.modify_order(2, { 59,200,0 });
	best_ask = orders.get_best_ask();
	CHECK(get<0>(best_ask) == 59);
	CHECK(get<1>(best_ask) == 200);
	orders.add_order(3, { 59, 100, 'S' });
	best_ask = orders.get_best_ask();
	CHECK(get<0>(best_ask) == 59);
	CHECK(get<1>(best_ask) == 300);
	orders.delete_order(2);
	best_ask = orders.get_best_ask();
	CHECK(get<1>(best_ask) == 100);
	orders.modify_order(1, { 59, 600, 0 });
	best_ask = orders.get_best_ask();
	CHECK(get<1>(best_ask) == 700);
}