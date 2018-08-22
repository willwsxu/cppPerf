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


TEST_CASE("Pyramid Transition Matrix", "[HT]")
{
	CHECK(BitHashMap().pyramidTransition("DB", vector<string>{"BDD", "ACA", "CBA", "BDC", "AAC", "DCB", "ABC", "DDA", "CCB"}) == false);
}

//535. Encode and Decode TinyURL
class TinyUrl {
	unordered_map<string, string> tinyUrlMap;
	string tinyRoot = "http://tinyurl.com/";
public:
	string encode62(size_t h) {
		char map[] = "abcdefghijklmnopqrstuvwxyzABCDEF"
			"GHIJKLMNOPQRSTUVWXYZ0123456789";
		string url;
		url.reserve(8);
		while (h) {
			url.append(1, map[h % 62]);
			h /= 62;
		}
		reverse(url.begin(), url.end());
		return url;
	}
	// Encodes a URL to a shortened URL. http://tinyurl.com/4e9iAk
	string encode(string longUrl) {
		size_t h = hash<string>{}(longUrl);
		string tiny = encode62(h);
		tinyUrlMap[tiny] = longUrl;
		return tinyRoot + tiny;
	}

	// Decodes a shortened URL to its original URL.
	string decode(string shortUrl) {
		string tiny = shortUrl.substr(tinyRoot.size());
		return tinyUrlMap[tiny];
	}
};


TEST_CASE("tiny URL decode", "[HT]")
{
	string url= "https://leetcode.com/problems/design-tinyurl";
	TinyUrl t;
	CHECK(t.decode(t.encode(url)) == url);
}

// 781. Rabbits in Forest
class MapCounter
{

public:
	// a subset of rabbits are asked how many others they saw having same color as his or hers
	// find minimum total rabbits in a forest
	// idea: two rabbits provide different answers must not be same color
	//       rabbits with same answer could have different colors. if answer is a, then at most a+1 
	// could be of same color.
	// count same answer, n, ceiling(n/a+1) is # of colors, total=ceiling(n/a+1)*(a+1)
	int numRabbits(vector<int>& answers) {// beat 89%
		unordered_map<int, int> count;
		//int count[1000] = { 0 };  could be faster using array instead of map
		for (int a : answers)
			count[a]++;
		return accumulate(count.begin(), count.end(), 0, [](int prev, const auto& p) {
			return (p.second + p.first) / (p.first + 1)*(p.first + 1)+prev;
		});
	}

	// 554. Brick Wall, each row represent brick of various len
	// draw a vertical line and cross least bricks, return # of crossed bricks
	// line in between bricks is not considered crossed
	// # of bricks in each row and rows are in range [1,10,000], total bricks won't exceed 20000
	int leastBricks(vector<vector<int>>& wall) {  // beat 12%
		unordered_map<int, int> count;  // count position of all bricks from left
		int maxAlign = 0; // bricks in alignment
		for (const auto& row : wall) {
			for (size_t i = 0, pos = 0; i < row.size() - 1; i++) {  // skip last brick as we can not draw on edge
				pos += row[i];
				maxAlign = max(maxAlign, ++count[pos]);
			}
		}
		//auto result = max_element(count.begin(), count.end(), [](const auto&p1, const auto&p2) {
		//	return p1.second < p2.second;
		//});
		return wall.size() - maxAlign;  // draw line where bricks align the most
	}
};

class Duplicate {
public:
	// 217. Contains Duplicate
	bool containsDuplicate(vector<int>& nums) {
		unordered_set<int> s;
		s.reserve(nums.size()*2);  // beat 14% after reserve
		for (int n : nums) {
			if (s.count(n))
				return true;
			s.insert(n);
		}
		return false;
	}
};