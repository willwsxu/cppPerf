#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h
#include <bitset>
using namespace std;

#include "..\library\myalgo.h"
#include "mathlib.h"

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
	// 219. Contains Duplicate II
	// contain nums[i]==nums[j], j-i<=k
	bool containsNearbyDuplicate(vector<int>& nums, int k) { // beat 17%
		unordered_set<int> s;  // use to track k consecutive numbers
		s.reserve(k * 2);
		k = min<int>(nums.size(), k);
		for (int i = 0; i < k; i++) {
			if (s.count(nums[i]))
				return true;
			s.insert(nums[i]);
		}
		for (size_t i = k; i < nums.size(); i++) {// sliding window
			if (s.count(nums[i]))
				return true;
			s.insert(nums[i]);
			s.erase(nums[i - k]);  // remove first number that is out of window
		}
		return false;
	}

	// 220. Contains Duplicate III
	// contain nums[i]==nums[j], j-i<=k, nums[i]-num[j]<=t
	bool containsNearbyAlmostDuplicate(vector<int>& nums, int k, int t) {// beat 79%
		set<int> s;  // to sort k+1 values
		k = min<int>(nums.size(), k);
		auto almostDup = [t, &s](int64_t n) {
			int64_t lb = max<int64_t>(n - t, INT_MIN);  // take care of underflow, overflow
			auto found = s.lower_bound(static_cast<int>(lb)); // look for nums[i]-t, nums[i]+t
			if (found != s.end() && *found <= n + t)
				return true;
			return false;
		};
		for (int i = 0; i < k; i++) {
			if (almostDup(nums[i]))
				return true;
			s.insert(nums[i]);
		}
		for (size_t i = k; i < nums.size(); i++) {// sliding window
			if (almostDup(nums[i]))
				return true;
			s.insert(nums[i]);
			s.erase(nums[i - k]);  // remove first number that is out of window
		}
		return false;
	}
};

TEST_CASE("219. Contains Duplicate II", "[DUP]")
{
	CHECK(Duplicate().containsNearbyDuplicate(vector<int>{1,2,3,1}, 3));
	CHECK(Duplicate().containsNearbyDuplicate(vector<int>{1, 2, 3, 1, 2, 3}, 2)==false);
	CHECK(Duplicate().containsNearbyDuplicate(vector<int>{1}, 1)==false);
	CHECK(Duplicate().containsNearbyDuplicate(vector<int>{1}, 2) == false);
	CHECK(Duplicate().containsNearbyDuplicate(vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 15) == false);
}

TEST_CASE("220. Contains Duplicate III", "[HASH]")
{
	CHECK(Duplicate().containsNearbyAlmostDuplicate(vector<int>{1, -2147483646}, 1, 2147483647) == true);
	CHECK(Duplicate().containsNearbyAlmostDuplicate(vector<int>{0, 2147483647}, 1, 2147483647) == true);
	CHECK(Duplicate().containsNearbyAlmostDuplicate(vector<int>{1, 2, 3, 1}, 3, 0));
	CHECK(Duplicate().containsNearbyAlmostDuplicate(vector<int>{1, 2, 3, 1, 2, 3}, 2, 0) == false);
	CHECK(Duplicate().containsNearbyAlmostDuplicate(vector<int>{1, 5, 3, 1, 2, 4}, 2, 1) == true);
	CHECK(Duplicate().containsNearbyAlmostDuplicate(vector<int>{1}, 1,0) == false);
	CHECK(Duplicate().containsNearbyAlmostDuplicate(vector<int>{1}, 2,0) == false);
	CHECK(Duplicate().containsNearbyAlmostDuplicate(vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 15, 0) == false);
}

class MapStuff {
public:
	// 846. Hand of Straights
	// 0 <= hand[i] <= 10^9, 1 <= hand.length <= 10000
	bool isNStraightHand(vector<int>& hand, int W) {  // beat 91%
		map<int, int> count;  // count cards of same value
		for (int h : hand)
			count[h]++;
		for (const auto& card : count) {  // from lowest card
			if (card.second) { // start of a new sequence
				int start = card.second;
				for (int i = card.first; i < card.first + W; i++) // iterate W sequential cards
				{
					auto& found = count[i];
					if (found < start)  // card in middle of sequence is not sufficient
						return false;
					found -= start;
				}
			}
		}
		return true;
	}

public:
	//771. Jewels and Stones, (string S)letters represent stones, (string J) some of these are Jewels
	int numJewelsInStones(string J, string S) {  // beat 98%
		map<char, int> jewels;
		transform(J.begin(), J.end(), inserter(jewels, jewels.begin()), [](char c)->pair<char, int> { return{ c,0 }; });  // string to map
		return count_if(S.begin(), S.end(), [&jewels](char c) { return jewels.count(c); });
	}

	// 290. Word Pattern, Given a pattern and a string str, find if str follows the same pattern.
	bool wordPattern(string pattern, string str) {  // 2 hash maps, beat 100%
		stringstream strm(str);
		auto p = pattern.begin();
		unordered_map<string, char> str_p;
		unordered_map<char, string> p_str;
		for (auto x = istream_iterator<string>(strm); x != istream_iterator<string>(); ++x)
		{
			if (p == pattern.end())  // make sure pattern is not too short
				return false;
			auto found1 = str_p.find(*x);
			auto found2 = p_str.find(*p);
			if (found1 == str_p.end() && found2 == p_str.end()) {  // new pattern
				str_p[*x] = *p;        // add pattern
				p_str[*p] = *x;
			}
			else if (found1 == str_p.end() || found2 == p_str.end())  // found old pattern not match
				return false;
			else if (found1->second != *p || found2->second != *x)
				return false;
			++p;
		}
		return p == pattern.end();  // make sure pattern is not too long
	}

	// Given n points in the plane that are all pairwise distinct, a "boomerang" is a tuple of points (i, j, k) 
	// such that the distance between i and j equals the distance between i and k
	// n<=500, points [-10000, 10000]
	int numberOfBoomerangs(vector<pair<int, int>>& points) {
		return accumulate(points.begin(), points.end(), 0, [&points](int init, const pair<int, int>& p1) {
			map<int, int> dist_count;  // count points of same distance from p1
			for_each(points.begin(), points.end(), [&dist_count, &p1](const pair<int, int>& p2) {
				auto dist = [](int dx, int dy) { return dx*dx + dy*dy; };
				dist_count[dist(p1.first - p2.first, p1.second - p2.second)]++;
			});
			return init + accumulate(dist_count.begin(), dist_count.end(), 0, [](int init, const pair<int, int>& c) {
				return init + c.second*(c.second - 1);  // permutation 2 of n
			});
		});
	}

	vector<string> split(const string&s, char sep)
	{
		auto next = s.find_first_of(sep);
		if (next != string::npos) {
			return{ s.substr(0,next), s.substr(next + 1) };
		}
		return{};
	}
	// 811. Subdomain Visit Count
	vector<string> subdomainVisits(vector<string>& cpdomains) { // beat 54%
		map<string, int> count;
		for (const string& d : cpdomains) {
			auto tokens = split(d, ' ');
			if (!tokens.empty()) {  // 0 or 2
				int c = stoi(tokens[0]);
				while (!tokens.empty()) {
					count[tokens[1]] += c;
					tokens = split(tokens[1], '.');
				}
			}
		}
		vector<string> ans;
		transform(count.begin(), count.end(), back_inserter(ans), [](const auto&p) { return to_string(p.second).append(1, ' ').append(p.first);	});
		return ans;
	}
	// 599. Minimum Index Sum of Two Lists, return all ties
	vector<string> findRestaurant(vector<string>& list1, vector<string>& list2) {
		map<string, int> index_map;
		int i = 0;
		for (const string &s2 : list2)
			index_map[s2] = i++;
		int index_sum=INT32_MAX;		
		vector <string> ans;
		i = 0;
		for (const string& s1 : list1) {
			auto x = index_map.find(s1);
			if (x != index_map.end()) { // find string match
				if (ans.empty()) {
					ans.push_back(x->first);
					index_sum = i + x->second; // sum up index
				}
				else if (index_sum > i + x->second) {  // new low
					ans.clear();
					index_sum = i + x->second;
					ans.push_back(x->first);
				}
				else if (index_sum == i + x->second)
					ans.push_back(x->first);
			}
			i++;
		}
		return ans;
	}
	// 957. Prison Cells After N Days
	vector<int> prisonAfterNDays(vector<int>& cells, int N) {
		int size = cells.size();
		if (size < 3)
			return cells;
		map<vector<int>, int>  pattern_recurring;
		deque<vector<int>> patterns;
		size--;
		for (int n = 1; n <= N; n++) {
			vector<int> next(size + 1, 0); // cell config next day
			for (int k = 1; k < size; k++) {
				if (cells[k - 1] == cells[k + 1]) // flip to 1 if both neighbor are 1 or 0
					next[k] = 1;
			}
			copy(begin(next), end(next), begin(cells));
			auto found = pattern_recurring.find(next);
			if (found == end(pattern_recurring)) {
				pattern_recurring[next] = n;
				patterns.push_back(move(next));
			}
			else {
				N -= (found->second - 1);  // repeating pattern from found->second to n
				N %= (n - found->second);
				if (N == 0)
					return patterns.back();  // N-1, index from 0
				return patterns[N-1 + found->second - 1];  // found->second - 1 cells not in patter
			}
		}
		return patterns.back();
	}
	// 966. Vowel Spellchecker
	vector<string> spellchecker(vector<string>& wordlist, vector<string>& queries) {
		map<string, string> lower_cases, replace_vowels;
		set<string> exact(begin(wordlist), end(wordlist));
		set<char> vowels{ 'a','e','i','o','u','A','E','I','O','U' };
		auto replace_vower = [&vowels](string s) {
			for (char& c : s) {
				if (vowels.count(c))
					c = '_';
				else
					c = tolower(c);
			}
			return s;
		};
		for (const string& s : wordlist) {
			auto& l_c = lower_cases[to_lower(s)];
			if (l_c.empty())
				l_c = s;
			auto& r_v = replace_vowels[replace_vower(s)];
			if (r_v.empty())
				r_v = s;
		}
		vector<string> ans;
		ans.reserve(queries.size());
		for (const string& q : queries) {  // 3 step spelling check
			if (exact.count(q)) {  // return same word if exact match
				ans.push_back(q);
				continue;
			}
			auto found = lower_cases.find(to_lower(q));
			if (found != end(lower_cases)) {  // return first math if match on condition of case insensitive
				ans.push_back(found->second);
				continue;
			}
			auto found2 = replace_vowels.find(replace_vower(q)); // return first match if vowels are replaced
			if (end(replace_vowels) != found2) {
				ans.push_back(found2->second);
				continue;
			}
			ans.push_back("");
		}
		return ans;
	}
};
TEST_CASE("957. Prison Cells After N Days", "[HASH]")
{
	CHECK(MapStuff().prisonAfterNDays(vector<int>{1, 0, 0, 1, 0, 0, 1, 0}, 1000000000) == vector<int>{0, 0, 1, 1, 1, 1, 1, 0});
	CHECK(MapStuff().prisonAfterNDays(vector<int>{0, 1, 0, 1, 1, 0, 0, 1}, 7) == vector<int>{0, 0, 1, 1, 0, 0, 0, 0});
}

TEST_CASE("290. Word Pattern", "[HASH]")
{
	CHECK(MapStuff().wordPattern("ab", "dog cat dog cat") == false);
	CHECK(MapStuff().wordPattern("abba", "dog cat dog cat") == false);
	CHECK(MapStuff().wordPattern("abba", "dog cat cat dog") == true);
	CHECK(MapStuff().wordPattern("abba", "dog dog dog dog") == false);
	CHECK(MapStuff().wordPattern("abba", "dog cat cat fish") == false);
	CHECK(MapStuff().wordPattern("aaaa", "dog cat cat dog") == false);
	CHECK(MapStuff().wordPattern("aaaa", "dog cat cat dog") == false);
	CHECK(MapStuff().wordPattern("jquery", "jquery") == false);
}
TEST_CASE("447. Number of Boomerangs", "[HASH]")
{
	vector<pair<int, int>> x{ { 0, 0 },{ 1,0 },{ 2, 0 } };
	CHECK(MapStuff().numberOfBoomerangs(x) == 2);
}

class SetStuff
{
	set<string> str_to_set(string& A, set<string>& dup)
	{
		stringstream astr(A);
		set<string> a;
		//copy(istream_iterator<string>(astr), istream_iterator<string>(), inserter(a, a.begin()));
		for_each(istream_iterator<string>(astr), istream_iterator<string>(), [&a, &dup](const string&s) {
			if (a.count(s))
				dup.insert(s);
			else
				a.insert(s);
		});
		for (const string&s : dup)
			a.erase(s);
		return a;
	}
public:
	// A word is uncommon if it appears exactly once in one of the sentences, and does not appear in the other sentence
	vector<string> uncommonFromSentences(string A, string B) {  // beat 40%
		set<string> dup;
		set<string> a = str_to_set(A, dup);
		set<string> b = str_to_set(B, dup);
		vector<string> ans;
		std::set_difference(a.begin(), a.end(), b.begin(), b.end(), back_inserter(ans));
		std::set_difference(b.begin(), b.end(), a.begin(), a.end(), back_inserter(ans));
		if (!dup.empty()) {
			vector<string> ans2;
			sort(ans.begin(), ans.end());
			std::set_difference(ans.begin(), ans.end(), dup.begin(), dup.end(), back_inserter(ans2));  // ans-dup
			return ans2;
		}
		return ans;
	}

	// 575. Distribute Candies, evenly to 2 sibling, number represent kind of candies
	// array size, or total candies [2, 10,000]
	// array value, or candy kind,  [-100,000, 100,000]
	int distributeCandies(vector<int>& candies) {
		//return min(unordered_set<int>(begin(candies), end(candies)).size(), candies.size() / 2);  // beat 29%
		bitset<200001> exist;  // beat 31%
		size_t count = 0;
		for (int c : candies) {
			if (!exist.test(c + 100000)) {  // much faster to use test and set, than [], beat 89%
				exist.set(c + 100000, 1);
				count++;
			}
		}
		return min(count, candies.size()/2); // array is faster than bitset
	}
	// 202. Happy Number, repeat sum of square of digits, till ==1
	bool isHappy(int n) {  // beat 100%
		set<int> numbers;
		numbers.insert(n);
		while (n > 1) {
			int total = 0;
			while (n > 0) {
				total += (n % 10)*(n % 10);
				n /= 10;
			}
			if (numbers.count(total)) // number repeated
				return false;
			numbers.insert(total);
			n = total;
		}
		return n == 1;
	}
	// 955. Delete Columns to Make Sorted II
	int minDeletionSize(vector<string>& A) {  // greedy check ordering column by column
		int ans = 0;
		set<int> good;  // save row who has column value larger than previous
		for (size_t c = 0; c<A[0].size(); c++) {
			vector<int> row_larger;
			for (size_t r = 1; r<A.size(); r++) {
				if (good.count(r)>0)  // no need to compare next column
					continue;
				else if (A[r - 1][c] > A[r][c]) {
					ans++;
					row_larger.clear();
					break;
				}
				else if (A[r - 1][c] < A[r][c])  // potential candidate as good row
					row_larger.push_back(r);
			}
			good.insert(begin(row_larger), end(row_larger));  // store good rows with larger values
		}
		return ans;
	}	
	// 963. Minimum Area Rectangle II, 3 loops to find 3 points of a rectangle, check if forth is in the set
	double minAreaFreeRect(vector<vector<int>>& points) {
		set < vector<int>> point_set(points.begin(), points.end());
		int size = points.size();
		double area = numeric_limits<double>::max();
		for (int i = 0; i < size; i++) {  // p1
			for (int j = 0; j < size; j++) {  // P2
				if (j == i)
					continue;
				for (int k = 0; k < size; k++) { // P4
					if (k == i || k == j)
						continue;
					int leg1 = euclid_distance_square(points[i], points[j]);
					int leg2 = euclid_distance_square(points[i], points[k]);
					int diagonal = euclid_distance_square(points[k], points[j]);
					if (leg1 + leg2 != diagonal)
						continue;  // P1 P2 P4 form right triangle
								   // geometry, compute p3
					int x3 = points[j][0] + points[k][0] - points[i][0];
					int y3 = points[j][1] + points[k][1] - points[i][1];
					if (point_set.find(vector<int>{x3, y3}) != end(point_set))
						area = min(area, sqrt(leg1)*sqrt(leg2));
				}
			}
		}
		return area == numeric_limits<double>::max() ? 0 : area;
	}
	// 939. Minimum Area Rectangle, side parallel to axis
	// step 1: iterate all possible diagonal points of a rectangle
	// step 2: compute other 2 points of the rectangle
	// step 3: find if they exist, find in set
	// step 4: if rectangle is good, compute area
	int minAreaRect(vector<vector<int>>& points) {
		unordered_set < pair<int,int>> point_set;
		for (const auto& p : points)
			point_set.emplace(p[0],p[1]);
		//sort(begin(points), end(points), [](const auto&p1, const auto&p2) { return p1.first < p2.first; });
		// could use sort to eliminate points that has no others on its horizontal or vertical lines
		int area = INT32_MAX;
		for (const auto& diag1 : points) {
			for (const auto& diag2 : points) {
				if (diag1[0] == diag2[0] || diag1[1] == diag2[1]) // cannot be on a vertical or horizontal line
					continue;
				if (point_set.find({ diag1[0], diag2[1] }) == end(point_set))
					continue;
				if (point_set.find({ diag2[0], diag1[1] }) == end(point_set))
					continue;
				area = min(area, abs(diag2[0] - diag1[0])*abs(diag1[1] - diag2[1]));
			}
		}
		return area == INT32_MAX ? 0 : area;
	}
};
TEST_CASE("939. Minimum Area Rectangle", "[NEW]")
{
	CHECK(SetStuff().minAreaRect(vector<vector<int>> { {1, 1}, { 1,3 }, { 3,1 }, { 3,3 }, { 2,2 }}) == 4);
	CHECK(SetStuff().minAreaRect(vector<vector<int>> { {1, 2}, { 1, 3 }, { 3, 3 }, { 4, 4 }, { 2, 1 }, { 1, 4 }, { 2, 2 }, { 1, 0 }, { 0, 2 }}) == 0);
}
// 955. Delete Columns to Make Sorted II
TEST_CASE("955. Delete Columns to Make Sorted II", "[HASH]")
{
	CHECK(SetStuff().minDeletionSize(vector<string>{"xc", "yb", "za"}) == 0);
}
TEST_CASE("202. Happy Number", "[HASH]")
{
	CHECK(SetStuff().isHappy(358) == false);
}
TEST_CASE("884. Uncommon Words from Two Sentences", "[HASH]")
{
	CHECK(SetStuff().uncommonFromSentences("abcd def abcd xyz", "ijk def ijk") == vector<string>{"xyz"});
	CHECK(SetStuff().uncommonFromSentences("s z z z s", "s z ejt") == vector<string>{"ejt"});
	CHECK(SetStuff().uncommonFromSentences("apple apple", "banana") == vector<string>{"banana"});
	CHECK(SetStuff().uncommonFromSentences("this apple is sweet", "this apple is sour") == vector<string>{"sweet", "sour"});
}

class ArrayMap  // use array to map
{
public:
	vector<string> findWords(vector<string>& words) { // beat 100%
		static const int rows[] = { 1,2,2,1,0,1,1,1,0,1,1,1,2,2,0,0,0,0,1,0,0,2,0,2,0,2 };  // row of alphabet on keyboard
		auto same_row = [](const string&s) {  // each letter is at same row of keyboard
			if (s.empty())
				return true;
			return all_of(s.begin(), s.end(), [target = rows[toupper(s[0]) - 'A']](char c){return rows[toupper(c) - 'A'] == target; });
		};
		vector<string> ans;
		copy_if(words.begin(), words.end(), back_inserter(ans), [same_row](const string&s) { return same_row(s); });
		return ans;
	}
	// 205. Isomorphic Strings, apply letter mapping so s can be transformed to t
	// assume both s and t have the same length.
	bool isIsomorphic(string s, string t) { // beat 99%
		vector<int> map_s_t(128,0), map_t_s(128,0);
		for (size_t i = 0; i < s.size(); i++) {
			if (map_s_t[s[i]] == 0 && map_t_s[t[i]] == 0) {  // map letter between t ans s
				map_s_t[s[i]] = t[i];
				map_t_s[t[i]] = s[i];
			}
			else if (map_s_t[s[i]] == 0 || map_t_s[t[i]] == 0)  // inconsistant mapping
				return false;
			if (map_s_t[s[i]] != t[i])
				return false;
		}
		return true;
	}
	// 890. Find and Replace Pattern, reuse func #205
	vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
		vector<string> ans;
		copy_if(begin(words), end(words), back_inserter(ans), [this, &pattern](string& s) { return isIsomorphic(s, pattern); });
		return ans;
	}
	// 916. Word Subsets, find a in A when every b in B is subset of a
	// Each word is a string of lowercase letters
	// clever idea: find super set of B 
	vector<string> wordSubsets(vector<string>& A, vector<string>& B) {
		vector<int> superset_b_count(26, 0);
		for (const string& b : B) {
			vector<int> count_b = count_letter(b, 'a');
			for (int i = 0; i < 26; i++)
				superset_b_count[i] = max(superset_b_count[i], count_b[i]);
		}
		auto universal = [&superset_b_count](const string& s) {
			vector<int> count = count_letter(s, 'a');
			for (int i = 0; i < 26; i++)
				if (count[i] < superset_b_count[i])
					return false;
			return true;
		};
		vector<string> ans;
		copy_if(begin(A), end(A), back_inserter(ans), universal);
		return ans;
	}
	// 914. X of a Kind in a Deck of Cards
	// separate into groups of X same cards, X>=2
	// 0 <= deck[i] < 10000
	bool hasGroupsSizeX(vector<int>& deck) {
		if (deck.size() < 2)
			return false;
		vector<int> count(10000, 0);
		for (int d : deck)
			count[d]++;
		count.erase(remove(count.begin(), count.end(), 0), count.end());
		sort(count.begin(), count.end());
		if (count[0] < 2)
			return false;
		for (int factor = 2; factor <= count[0]; factor++) {
			if (all_of(count.begin(), count.end(), [&count, factor](int i) { return i%factor == 0; }))
				return true;
		}
		return false;
	}
	// 594. Longest Harmonious Subsequence, exactly max-min=1
	int findLHS(vector<int>& nums) { // beat 54%
		map<int, int> num_count;
		for (int n : nums)
			num_count[n]++;
		int max_count = 0;
		for (const auto& c : num_count) {
			auto x = num_count.find(c.first + 1);
			if (x == num_count.end())
				continue;
			max_count = max(max_count, c.second + x->second);
		}
		return max_count;
	}
};
TEST_CASE("914. X of a Kind in a Deck of Cards", "[HASH]")
{
	CHECK(ArrayMap().hasGroupsSizeX(vector<int>{1, 1, 1, 1, 2, 2, 2, 2, 2, 2}) == true);
}
TEST_CASE("500. Keyboard Row", "[HASH]")
{
	CHECK(ArrayMap().findWords(vector<string>{"Hello", "Alaska", "Dad", "Peace"}) == vector<string>{"Alaska", "Dad"});
}
class Greedy
{
public:
	// 874. Walking Robot Simulation, start from (0,0) in x-y plane, face north. commands
	// -2: turn left 90 degrees
	//	- 1 : turn right 90 degrees
	//	1 <= x <= 9 : move forward x units
	// 0 <= commands.length <= 10000, 0 <= obstacles.length <= 10000, obstacle[i,j]
	int robotSim(vector<int>& commands, vector<vector<int>>& obstacles) {  // beat 79%
		map<char, pair<char, char>> direction_change{ {'N',{'W','E'}},{'S',{'E','W'}},{'W',{'S','N'}},{'E',{'N','S'}} };
		char dir = 'N';
		int x = 0;
		int y = 0;
		set<pair<int, int>> obst{};
		for (const auto&v : obstacles) {
			obst.emplace(v[0],v[1]);
		}
		auto find_obst = [&obst](int x1, int y1)
		{
			return obst.count({ x1,y1 })>0;
		};
		int result = 0;
		for (int cmd : commands) {
			if (cmd < 0) {
				const auto& change = direction_change[dir];
				dir = cmd == -2 ? change.first : change.second;
				continue;
			}
			int step = 1;
			if (dir == 'S' || dir == 'W')
				step = -1;
			switch (dir) {
			case 'N':
			case 'S':
				for (int j = 1; j <= cmd; j++) {
					if (find_obst(x, y + step))
						break;
					y += step;
				}
				break;
			case 'W':
			case 'E':
				for (int j = 1; j <= cmd; j++) {
					if (find_obst(x+step, y))
						break;
					x += step;
				}
				break;
			}
			result = max(result, x*x + y*y);  // find max of euclidean distance
		}
		return result;
	}
};


TEST_CASE("874. Walking Robot Simulation", "[HASH]")
{
	CHECK(Greedy().robotSim(vector<int>{4, -1, 4, -1, 4, -1, 3}, vector<vector<int>>{ {2, 4}}) == 17);
	CHECK(Greedy().robotSim(vector<int>{4, -1, 4, -2, 4}, vector<vector<int>>{ {2,4}})==65);
}

// 933. Number of Recent Calls
class RecentCounter {
	set<int> pings;  // ping time, increasing
public:
	RecentCounter() {	}

	int ping(int t) { // return number of pings between [t-3000, t]
		pings.insert(end(pings), t);
		return distance(pings.lower_bound(t - 3000), end(pings));
	}
};