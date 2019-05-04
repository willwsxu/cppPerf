#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <set>

using namespace std;

string RSP(vector<string> others)
{
	int C = (int)others[0].size();
	string result;
	result.reserve(C);
	for (int c = 0; c < C; c++) {
		map<char,int> rsp;
		for (int i = 0; i < (int)others.size(); i++) {
			rsp[(others[i][c])]++;
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
			//if (rsp[result.back()] == others.size() - 1) {
			//	auto eliminate = remove_if(begin(others), end(others), [winner = result.back(), c](const string& s) { return s[c] != winner; });
			//	others.erase(eliminate, end(others));
			//}
		}
	}
	int A = (int)others.size() + 1;
	auto lose_match = [](const string s1, const string& s2) {  // s1 lose to s2
		if (s1 == s2)
			return false;
		for (int i = 0; i < (int)s1.size(); i++) {
			if (s1[i] == 'R' && s2[i] == 'P')
				return true;
			if (s1[i] == 'S' && s2[i] == 'R')
				return true;
			if (s1[i] == 'P' && s2[i] == 'S')
				return true;
		}
		return false;
	};
	auto lose_all = [lose_match](const string s, vector<string>& team) {
		for (const string& s2 : team)
			if (!lose_match(s, s2))
				return false;
		return true;
	};
	while (A > 1) {
		A /= 2;
		// eliminate losers
		set<string> eliminate;
		for (const string s : others)
			if (lose_all(s, others) && lose_match(s, result))
				eliminate.insert(s);
		if (eliminate.empty())
			break;
		others.erase(remove_if(begin(others), end(others), [&eliminate](const string& s) { return eliminate.count(s) > 0; }), end(others));
	}
	for (const string s : others)
		if (!lose_match(s, result))
			return "IMPOSSIBLE";
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


int main(int argc, char *argv[])
{
	online1();
	return 0;
}

/*
#include "catch.hpp"
TEST_CASE("jam2019 1C #1", "[1C1]")
{
	CHECK(RSP(vector<string>{"RSPR", "SSPR", "RPRR"}) == "RSPP");
	CHECK(RSP(vector<string>{"RSPS", "SSPS", "RPRS"}) == "RSPR");
	CHECK(RSP(vector<string>{"RSPP", "SSPP", "RPRP"}) == "RSPS");
	CHECK(RSP(vector<string>{"R","S","P"}) == "IMPOSSIBLE");
	CHECK(RSP(vector<string>{"RSP", "SSP", "RPR"}) == "IMPOSSIBLE");
}
*/