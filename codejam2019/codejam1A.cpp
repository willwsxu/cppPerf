#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
using namespace std;

int pair_rhyme(vector<string>& words) {
	size_t pos = 1;
	map<string, int> suffix_count;
	map<string, set<string>> rhymes;
	for (string& w : words) {
		rhymes[w.substr(w.size() - 1)].insert(move(w));
	}
	for (auto iter = begin(rhymes); iter != end(rhymes); ) {  // no rhymes at all
		if (iter->second.size() < 2)
			rhymes.erase(iter++);
		else
			++iter;
	}
	while(!rhymes.empty()) {
		map<string, set<string>> rhymes2;
		pos++;
		for (auto& p : rhymes) {
			int no_advance = 0;  // word cannot make longer suffix
			for (const string& w : p.second) {
				if (w.size() >= pos)
					rhymes2[ w.substr(w.size() - pos) ].insert(w);
				else
					no_advance++;
			}
			for (auto iter = begin(rhymes2); iter != end(rhymes2); ) {  // no rhymes at all
				if (iter->second.size() < 2) {
					rhymes2.erase(iter++);
					no_advance++;
				}
				else
					++iter;
			}
			suffix_count[p.first] = no_advance;
		}
		rhymes.swap(rhymes2);
	}
	vector<string> suffixes;
	for (const auto& p : suffix_count) {
		suffixes.push_back(p.first);
	}
	sort(begin(suffixes), end(suffixes), [](const string&s1, const string& s2) { return s1.size() > s2.size(); });
	int pairs = 0;
	for (const string& suf : suffixes) {
		auto found = suffix_count.find(suf);
		if (found == end(suffix_count))
			continue;
		int count = found->second;
		if (count > 1) {
			pairs++;
			count -= 2;
			suffix_count.erase(suf);
		}
		for (int i=1; i<suf.size(); i++) {
			string s = suf.substr(i);
			auto found = suffix_count.find(s);
			if (found != end(suffix_count)) {
				count += found->second;
				suffix_count.erase(s);
			}
			if (count > 1) {
				pairs++;
				count -= 2;
			}
		}
	}
	return pairs*2;
}

void online()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N;
		cin >> N;
		cin.ignore();
		vector<string> words;
		words.reserve(N);
		for (int i = 0; i < N; i++) {
			string s;
			getline(cin, s);
			words.push_back(move(s));
		}
		cout << "Case #" << t;
		cout << ": " << pair_rhyme(words) << "\n";
	}
}
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "..\catch.hpp"

TEST_CASE("jam2019 1A", "[R1A]")
{
	online();
	CHECK(pair_rhyme(vector<string>{"TARPOL","PROL"}) == 2);
	CHECK(pair_rhyme(vector<string>{"TARPOR", "PROL", "TARPRO"}) == 0);
	CHECK(pair_rhyme(vector<string>{"CODEJAM", "JAM","HAM","NALAM","HUM","NOLOM"}) == 6);
	CHECK(pair_rhyme(vector<string>{"PI", "HI","WI","FI"}) == 2);
	CHECK(pair_rhyme(vector<string>{"CODEJAM", "JAM", "HAM", "HUM"}) == 4);
}