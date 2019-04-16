#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
using namespace std;

template<typename T>
struct greater_size {
	bool operator()(const string&s1, const string& s2) { // sort by len, then lexico
		if (s1.size() == s2.size())
			return s1 < s2;  // so it allow set to store string of same length!!!
		return s1.size() > s2.size(); 
	};
};
// Given a list of unique words, find pair of words that rhyme (share same suffix), but not with any other pairs choosen
// e.g. ABC BBC CBC DBC are valid 2 pairs, one pair rhyme on BC, other pair rhyme on C
// ABC BBC CBC DBC EBC FBC only have 2 valid pairs. a third pair will conflict with other two pairs 
int pair_rhyme(vector<string>& words) {
	size_t pos = 1;
	map<string, int> suffix_count;
	map<string, set<string>> rhymes;
	for (string& w : words) {
		rhymes[w.substr(w.size() - 1)].insert(move(w)); // group by last letter
	}
	for (auto iter = begin(rhymes); iter != end(rhymes); ) {  //remove all word that share no suffix, no rhymes at all
		if (iter->second.size() < 2)
			rhymes.erase(iter++);
		else
			++iter;
	}
	while(!rhymes.empty()) {
		map<string, set<string>> rhymes2;
		pos++;  // increase suffix by one, each loop
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
			suffix_count[p.first] = no_advance;  // store suffix even if count is 0
		}
		rhymes.swap(rhymes2);
	}
	set<string, greater_size<string>> suffixes;  // sorted by len in reverse order
	for (const auto& p : suffix_count) {
		suffixes.insert(p.first);  // candidate to use multi index container
	}
	int pairs = 0;
	while (!suffixes.empty()) {
		// check longest suffix first, if count >=2, increment answer by 1, pass down left over values to suffix value of lesser len
		const string& longest_suf = *suffixes.begin();
		auto found = suffix_count.find(longest_suf);
		suffixes.erase(suffixes.begin());
		if (found == end(suffix_count))
			continue;
		const string& suf = found->first;
		int count = found->second;
		if (count > 1) {
			pairs++;  // a new pair at this long suffix
			count -= 2;
		}
		if (count>0 && suf.size()>1) {  // roll leftover value to next suffix of len-1, don't process
			string s = suf.substr(1);
			auto shorter = suffix_count.find(s);
			if (shorter != end(suffix_count)) {
				shorter->second += count;
			}
			else {  // not reachable due to all suffix len are added already
				suffix_count[s] = count;
				suffixes.insert(move(s)); // add the new suffix of this len
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
	//online();
	CHECK(pair_rhyme(vector<string>{"1YXA", "2YXA", "3YXA", "1ZXA", "2ZXA", "3ZXA","1WXA","2WXA","3WXA","A"}) == 10);
	CHECK(pair_rhyme(vector<string>{"1YXA", "2YXA", "3YXA", "1ZXA", "2ZXA", "3ZXA"}) == 6);
	CHECK(pair_rhyme(vector<string>{"CODEJAM", "JAM", "HAM", "HUM"}) == 4);
	CHECK(pair_rhyme(vector<string>{"TARPOL","PROL"}) == 2);
	CHECK(pair_rhyme(vector<string>{"TARPOR", "PROL", "TARPRO"}) == 0);
	CHECK(pair_rhyme(vector<string>{"CODEJAM", "JAM","HAM","NALAM","HUM","NOLOM"}) == 6);
	CHECK(pair_rhyme(vector<string>{"PI", "HI","WI","FI"}) == 2);
}