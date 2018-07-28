#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;


struct BitHashAG
{
	size_t operator()(string const& s) const noexcept {  // hash first 2 char ([A..G])
		return (s[0] - 'A') + ((s[1] - 'A') << 3);
	}
};
class BitHashMap
{
	using myMap = unordered_map<string, string, BitHashAG>;
	vector<string> generateBottom(const vector<string>& bases, const myMap& base2top)
	{
		vector<string> newBottom[2];
		newBottom[0].push_back("");
		int from = 0;
		for (string const & b : bases) {
			const string& top = base2top.at(b);
			newBottom[1 - from].clear();
			for (char t: top) {
				for (string const& partial : newBottom[from])
					newBottom[1 - from].push_back(partial + t);
			}
			from = 1 - from;
		}
		return newBottom[from];
	}
	bool pyramidTransition(const string& bottom, const myMap& base2top)
	{
		int n = bottom.size() - 1;
		if (n == 0)
			return true;
		vector<string> bases;
		bases.reserve(n);
		for (int i = 0; i < n; i++) {
			bases.push_back(bottom.substr(i, 2));
			if (base2top.find(bases.back()) == base2top.end())  // no allowed triangle
				return false;
		}
		vector<string> newBottom = generateBottom(bases, base2top);
		for (const string & b : newBottom) {
			if (pyramidTransition(b, base2top))
				return true;
		}
		return false;
	}
public:
	// 756. Pyramid Transition Matrix, from pyramid given bottom line, and the allowed triplet where first two match the base of the triangle
	// allowed letters: A-G
	bool pyramidTransition(string bottom, vector<string>& allowed) { // beat 76%
		myMap base2top;  // allowed top letter from first 2 letter of the triplets
		for (const auto& s : allowed) { // no speed difference between using int bits or string for top letters  
			base2top[s.substr(0, 2)].append(1,s[2]); // third letter of the triplet is at top
		}
		return pyramidTransition(bottom, base2top);
	}
};


TEST_CASE("Pyramid Transition Matrix", "[NEW]")
{
	CHECK(BitHashMap().pyramidTransition("DB", vector<string>{"BDD", "ACA", "CBA", "BDC", "AAC", "DCB", "ABC", "DDA", "CCB"}) == false);
}

//535. Encode and Decode TinyURL
class TinyUrl {
public:

	// Encodes a URL to a shortened URL.
	string encode(string longUrl) {
		size_t h = hash<string>{}(longUrl);
	}

	// Decodes a shortened URL to its original URL.
	string decode(string shortUrl) {

	}
};