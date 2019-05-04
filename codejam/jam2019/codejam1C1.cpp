#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <set>

using namespace std;

// No assumption, not all string of same lenght
// wrap around if string is too short
// observation: tie will eventually be broken, with win or lose
// eliminate lost players if there are different hands (RS, SP or PR)
string RSP(vector<string> others)
{
	auto max_elem = max_element(begin(others), end(others), [](const string& s1, const string& s2) { return s1.size() < s2.size(); });
	string result;
	result.reserve(max_elem->size());
	int c = -1;
	while (!others.empty()) {
		c++;
		map<char,int> rsp;
		for (int i = 0; i < (int)others.size(); i++) {
			const string& s = others[i];
			rsp[(s[c%s.size()])]++;  // wrap around shorter string
		}
		if (rsp.size() > 2)
			return "IMPOSSIBLE";
		if (rsp.size() == 1) {
			char p = begin(rsp)->first;  // R -> S -> P -> R
			if (p=='R')
				result.push_back('P');
			else if (p=='S')
				result.push_back('R');
			else
				result.push_back('S');
			return result;
		}
		else {
			char first = begin(rsp)->first;
			char second = (++begin(rsp))->first;
			if (first == 'R' && second != 'P' || first == 'S' && second != 'R' || first == 'P'&& second != 'S')
				result.push_back(first);
			else
				result.push_back(second);
			auto eliminate = remove_if(begin(others), end(others), [winner = result.back(), c](const string& s) { return s[c%s.size()] != winner; });
			others.erase(eliminate, end(others));
		}
	}
	return result;
}
void online1() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int A;
		cin >> A;
		cin.ignore();
		vector<string> others;
		for (int i = 0; i < A; i++) {
			string s;
			getline(cin, s);
			others.push_back(move(s));
		}
		cout << "Case #" << t;
		auto ans = RSP(others);
		cout << ": " << ans << "\n";
	}
}

/*
int main(int argc, char *argv[])
{
	online1();
	return 0;
}
*/

#include "catch.hpp"
TEST_CASE("jam2019 1C #1", "[1C1]")
{
	CHECK(RSP(vector<string>{"RSPR", "SSPR", "RPRR"}) == "RSS");
	CHECK(RSP(vector<string>{"R","S","P"}) == "IMPOSSIBLE");
	CHECK(RSP(vector<string>{"RS","RSRSR","RSRSS","RSRPR","RSSR","RPP","S"}) == "RSRSRRR");
}
