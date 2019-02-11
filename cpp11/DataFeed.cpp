#include <string>
#include <vector>
#include <sstream>
#include <map>
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

#include "..\catch.hpp"
TEST_CASE("Peak6 data feed test", "[TEST]")
{
	RegionalBook book;
	book.update_message("Q|NYS|IBM|99|105|");
	book.update_message("Q|NAS|IBM|98|102|");
	book.update_message("Q|ARC|IBM|101|103|");
	book.display();
}