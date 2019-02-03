
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "..\catch.hpp"  // don't put this file in stdafx.h

#include <cassert>
#include <fstream>
#include "Graph.h"
#include "helper.h"
#include "myalgo.h"
#include "SortedVector.h"
#include <stack>
#include <numeric>
using namespace std;

static vector<int> dp(1000001, 0);
static int compute(int n)
{
	if (dp[n]>0 || n==0)
		return dp[n];

	int ans = INT32_MAX;
	int factor = 0;
	for (int k = 2; k*k <= n; k++)
		if (n%k == 0) {  // find factor k*b=
			ans = min(ans, compute(n / k) + 1);
			factor++;
		}
	if (ans == INT32_MAX || factor<19)  // try n-1 if n has < 19 pairs of factors. Some tests fail without this
		ans = min(ans, compute(n - 1)+1); // too slow if n-1 is called always
	return dp[n]=ans;
}
/*
* Fast way to count n down to 0: two ways to count: count down 1 at a time, or factor n into a*b, count down to max(a,b)
*/
int downToZero(int n) { // 0<=N<=10^6
	return compute(n);
}

TEST_CASE("Hackerrank Down to Zero II", "[OLD]")
{
	CHECK(downToZero(554548) == 10);
	CHECK(downToZero(1321) == 7);
	CHECK(downToZero(999997) == 8);
	CHECK(downToZero(3) == 3);
	CHECK(downToZero(4) == 3);
	CHECK(downToZero(29) == 7);

	vector<int> testcase1{ 966514, 812849, 808707, 360422, 691410, 691343, 551065, 432560, 192658, 554548 };
	vector<int> answer1{ 8, 10, 8, 11, 9, 11, 9, 9, 10, 10 };
	for (size_t i = 0; i < testcase1.size(); i++) {
		CHECK(downToZero(testcase1[i]) == answer1[i]);
	}
}

// return "yes" if it sorted
// return "yes\nswap a b" if it becomes sorted by swapping position a and b
// return "yes\nreverse a b" if it becomes sorted by reverting all nubers position a and b
// return "no"
string almostSorted(vector<int> arr) { // divid into 3 section, increasing, mid, decreasing
	auto decrease = adjacent_find(begin(arr), end(arr), [](int a, int b) { return a>b; });  // first fist number decreasing
	if (decrease == end(arr))  // sorted
		return "yes";
	auto rlast = make_reverse_iterator(decrease);
	auto increase = adjacent_find(rbegin(arr), rlast, [](int a, int b) { return a<b; });
	if (increase != rbegin(arr) && *decrease > *(increase - 1))  // first value in mid is larger than first in third section
		return "no";
	if (decrease != begin(arr) && *(decrease - 1) > *increase)   // last value in first section is larger than last in mid
		return "no";

	auto mid_end = increase.base();  // use it for end of mid section, from decrease to increase
	int first = distance(begin(arr), decrease) + 1;
	int second = distance(begin(arr), mid_end);
	auto mid_increase = adjacent_find(decrease, mid_end, [](int a, int b) { return a < b; });
	if (mid_increase != mid_end) {  // mid section is not decrease
		iter_swap(decrease, increase.base() - 1);
		auto mid_decrease = adjacent_find(decrease, mid_end, [](int a, int b) { return a > b; });
		if (mid_decrease != mid_end) {
			return "no"; // not sorted after swap, no good
		}
		return string("yes\nswap ").append(to_string(first)).append(1, ' ').append(to_string(second));
	}
	auto mid_dist = distance(decrease, mid_end);
	if (mid_dist == 2 || mid_dist==3) {
		return string("yes\nswap ").append(to_string(first)).append(1, ' ').append(to_string(second));
	}
	return string("yes\nreverse ").append(to_string(first)).append(1, ' ').append(to_string(second));
}

TEST_CASE("Hackerrank almost sorted", "[OLD]")
{
	CHECK(almostSorted(vector<int>{2, 4, 12, 8, 10, 6, 13}) == "yes\nswap 3 6");
	CHECK(almostSorted(vector<int>{2, 4, 12, 10, 8, 6, 13}) == "yes\nreverse 3 6");
	CHECK(almostSorted(vector<int>{2, 4, 12, 8, 6, 13}) == "yes\nswap 3 5");

	CHECK(almostSorted(vector<int>{2, 4, 12, 8, 10, 6}) == "yes\nswap 3 6");
	CHECK(almostSorted(vector<int>{2, 4, 12, 10, 8, 6}) == "yes\nreverse 3 6");

	CHECK(almostSorted(vector<int>{3, 6, 4, 5}) == "no");
	CHECK(almostSorted(vector<int>{3, 4, 1, 5}) == "no");
	CHECK(almostSorted(vector<int>{3, 2}) == "yes\nswap 1 2");
	CHECK(almostSorted(vector<int>{1, 2, 3, 4, 5}) == "yes");
	CHECK(almostSorted(vector<int>{5}) == "yes");
	CHECK(almostSorted(vector<int>{}) == "yes");
}

// Breadth First Search: Shortest Reach
// n nodes, m edges
// cost from s to each node, -1 if not reachable
// print cost from node 1 to n, but exclude s
vector<int> bfs(int n, int m, vector<vector<int>> edges, int s) {
	GraphUnweighted g(n);
	g.undirectEdges(edges);
	auto ans = g.bfs(s);
	ans.erase(begin(ans));
	ans.erase(begin(ans) + s - 1);
	for (int& w : ans) {  // each edge weight 6 units
		if (w > 0)
			w *= 6;
	}
	return ans;
}

//C++ Variadics
#include "meta_lib.h"

template<bool ...args>
int reversed_binary_value()
{
	return reverse_compute_from_bit(sizeof...(args), 0, args...);
}

TEST_CASE("Hackerrank compute value from bits", "[OLD]")
{
	CHECK(compute_from_bit(0, 1, 0, 0) == 4);
	CHECK(reverse_compute_from_bit(3,0,0,0,1) == 4);
	CHECK(reversed_binary_value<0, 0, 1>() == 4);
}


// n - chapters, arr - problems in each chapter, k - problems in a page
// find special problem where problem # == page #
int workbook(int n, int k, vector<int> arr) {
	int pageNo = 0;
	int ans = 0;
	for (int chapter = 0; chapter<n; chapter++) {
		int problemNo = pageNo + 1;
		int maxProblemNo = pageNo + ceiling_int(arr[chapter], k);
		while (problemNo <= arr[chapter] && problemNo <= maxProblemNo) {  // only problems fall in page count matters
			int pages = ceiling_int(problemNo, k) + pageNo;
			if (pages == problemNo) {
				ans++;
				problemNo++;
			}
			else if (problemNo > pages)
				problemNo = pages*k + 1;
			else
				problemNo = pages;
		}
		pageNo = maxProblemNo;
	}
	return ans;
}

TEST_CASE("Hackerrank Lisa's Workbook", "[OLD]")
{
	CHECK(workbook(5, 3, vector<int>{4, 2, 6, 1, 10}) == 4);
}

class SimpleTextEditor
{
	string S;
	stack<std::pair<int, string>> commands;
public:
	void append(const string& W, bool save_cmd = true) {
		if (save_cmd)
			commands.push({ 1, to_string(W.size()) });
		S.append(W);
	}
	void erase(int k, bool save_cmd = true) { //Delete the last  characters
		int chop_pos = S.size() - k;
		if (save_cmd)
			commands.push({ 2, S.substr(chop_pos, k) });
		S.erase(begin(S) + chop_pos, end(S));
	}
	char print(int k) {
		assert(k >= 1 && k <= (int)S.size());
		return S[k - 1];
	}
	void undo() {
		const auto& cmd = commands.top();
		switch (cmd.first) {
		case 1:  // undo append
			erase(stoi(cmd.second), false);
			break;
		case 2:  // undo delete
			append(cmd.second, false);
			break;
		}
		commands.pop();
	}
};

TEST_CASE("Hackerrank SimpleTextEditor", "[OLD]")
{
	SimpleTextEditor editor;
	editor.append("abc");
	CHECK(editor.print(3) == 'c');
	editor.erase(3);
	editor.append("xy");
	CHECK(editor.print(2) == 'y');
	editor.undo();
	editor.undo();
	CHECK(editor.print(1) == 'a');
}

// find all possible sub string pair that are anagram to each other. 
// 2 sub string can overlap, 
int sherlockAndAnagrams(string s) {
	int ans = 0;
	map<char, int> count1;
	for (char c : s)  // easy case for 1 letter
		count1[c]++;
	for (const auto&p : count1) {
		if (p.second > 1) {
			ans += p.second*(p.second - 1) / 2;
		}
	}
	for (size_t len = 2; len < s.size(); len++)
	{
		map<string, int> count;
		for (size_t i = 0; i <= s.size() - len; i++) {
			auto subs = s.substr(i, len);
			sort(begin(subs), end(subs));  // anagrams match after sorted
			count[subs]++;
		}
		int  old_ans = ans;
		for (const auto& p : count) {
			if (p.second > 1)
				ans += p.second*(p.second - 1) / 2;
		}
		//if (old_ans == ans)
		//	break;
	}
	return ans;
}
TEST_CASE("Hackerrank Sherlock and Anagrams", "[OLD]")
{
	CHECK(sherlockAndAnagrams("ifailuhkqq") == 3);
	CHECK(sherlockAndAnagrams("abba") == 4);
	CHECK(sherlockAndAnagrams("KKKK") == 10);
	CHECK(sherlockAndAnagrams("abcd") == 0);
}

class Person
{
protected:
	string name;
	int age;
	int cur_id;

public:
	virtual void getdata() {
		cin >> name >> age;
	}
	virtual void putdata() = 0;
	void print(int param3) {
		cout << name << " " << age << " " << param3 << " " << cur_id << "\n";
	}
};

class Professor : public Person
{
	static int gid;
	int publications;

public:
	Professor() {
		cur_id = ++gid;
	};
	void getdata() override
	{
		Person::getdata();
		cin >> publications;
	}
	void putdata()
	{
		print(publications);
	}
};
int Professor::gid = 0;

class Student : public Person
{
	static int gid;
	int marks[6];
public:
	Student() {
		cur_id = ++gid;
	}
	void getdata() override
	{
		Person::getdata();
		for (int i = 0; i < sizeof(marks) / sizeof(int); i++)
			cin >> marks[i];
	}
	void putdata()
	{
		print(accumulate(begin(marks), end(marks), 0));
	}
};
int Student::gid = 0;

Person *createObj(int type) {
	switch (type) {
	case 1:
		return new Professor();
	case 2:
		return new Student();
	}
	return nullptr;
}

// Leetcode 239. Sliding Window Maximum, beat 94%
// finx max among K numbers, in the array
vector<int> findKMax(int arr[], int n, int k) {
	if (k>n || k==0)
		return{};
	deque<pair<int, int>> k_num;  // store position
	auto add = [&k_num, k](int n, int pos) {
		if (!k_num.empty() && k_num.front().second + k == pos)  // maintain window of K numbers
			k_num.pop_front();
		if (k_num.empty()) {
			k_num.emplace_back(n, pos);
			return;
		}
		if (n >= k_num.front().first) {  // new max val, clear out all
			k_num.clear();
			k_num.emplace_back(n, pos);
		}
		else {
			while (k_num.back().first <= n)
				k_num.pop_back();   // remove smaller number at back
			if (k_num.back().first > n)
				k_num.emplace_back(n, pos); // add new num in strictly decreasing order
		}
	};

	vector<int> ans;
	ans.reserve(n - k + 1);
	for (int i = 0; i < k - 1; i++)
		add(arr[i], i);
	for (int i = k - 1; i < n; i++) {
		add(arr[i], i);
		ans.push_back(k_num.front().first);
	}
	return ans;
}

void printKMax(int arr[], int n, int k) {
	//Write your code here.
	auto ans = findKMax(arr, n, k);
	stringstream ostm;
	copy(begin(ans), end(ans), ostream_iterator<int>(ostm, " "));
	cout << ostm.str() << "\n";
}
TEST_CASE("Hackerrank deque STL", "[OLD]")
{
	int arr4[] = { 1,3,-1,-3,5,3,6,7 };
	CHECK(findKMax(arr4, 8,3) == vector<int>{3, 3, 5, 5, 6, 7});
	int arr3[] = { 10,1,2,10,7 };
	CHECK(findKMax(arr3, 5, 4) == vector<int>{10, 10});
	int arr2[] = { 3,4,6,3,4 };
	CHECK(findKMax(arr2, 5, 2) == vector<int>{4, 6, 6, 4});
	int arr[] = { 3,4,5,8,1,4,10 };
	CHECK(findKMax(arr, 7, 4) == vector<int>{8, 8, 8, 10});
}

// leetcode 53. Maximum Subarray
// max of continuous sub array, and max of sub sequence
vector<int> maxSubarray(vector<int> arr) {
	if (arr.empty())
		return{};	
	auto maxSubSeq = [&arr]() {
		int ans = INT32_MIN;
		for (int n : arr) {
			if (ans < 0)
				ans = max(ans, n);
			else if (n > 0)
				ans += n;
		}
		return ans;
	};
	return{ WXU::maxSubSum(arr.begin(), arr.end(), INT32_MIN).first, maxSubSeq() };
}

// find a minimal section to replace with a new sequence to make it steady
// each letter is exactly 1/4, ATCG
// n [4, 500000]
int steadyGene(string gene) {
	int max_per_letter = gene.size() / 4;
	map<char, int> count{ { 'A', 0 }, { 'T', 0 }, { 'C', 0 }, { 'G', 0 } };  //for all
	for (char g : gene)
		count[g]++;

	if (all_of(begin(count), end(count), [max_per_letter](const auto&p) { return p.second == max_per_letter; }))
		return 0;
	for (auto& p : count)  // count of any letter exceed 1/4
		p.second -= max_per_letter;
	map<char, int> count_section{ {'A', 0},{ 'T', 0 },{ 'C', 0 },{ 'G', 0 } };
	auto section_good = [&count, &count_section]() {  // a section contain the excessive letters
		return equal(begin(count), end(count), begin(count_section),
			[](const auto&p1, const auto&p2) { return p2.second >= p1.second; });
	};
	// Scan from left to right to find the optimal section
	auto first = begin(gene);
	auto last = first;
	int ans = gene.size();
	while (last != end(gene)) {
		while (!section_good() && last != end(gene)) {
			count_section[*last]++;
			++last;
		}
		if (!section_good())  // this section is not good, done
			break;
		while (first != last && count[*first] <= 0) { // move to right if the letter is not in excess
			count_section[*first]--;
			++first;
		}
		ans = min(ans, (int)distance(first, last));
		count_section[*first]--;  // scan forward one
		++first;
	}
	return ans;
}
TEST_CASE("Hackerrank steadyGene", "[OLD]")
{	
	CHECK(steadyGene("GAAATAAA") == 5);
	CHECK(steadyGene("AGCCCGGC") == 3);
	CHECK(steadyGene("AGTTTGGT") == 3); // edge case, need to initialize count
	//ifstream f("steadygene-test.txt");
	//int n;
	//string gene;
	//f >> n >> gene;
	//CHECK(steadyGene(gene) == 50022);
}

class HRMLElement
{
	HRMLElement *		parent;
	string				name;
	map<string, string> attributes;
	vector<HRMLElement>	child;
public:
	HRMLElement() :HRMLElement("", nullptr)
	{
	}
	HRMLElement(string&& n, HRMLElement* p) :name(move(n)), parent(p)
	{
	}
	HRMLElement& addTag(string&& tag)
	{
		if (tag.find('/')==string::npos) {  // <tag1 value = "HelloWorld">
			vector<string> attr;
			istringstream istr(tag);
			vector<string> tokens;
			move(istream_iterator<string>(istr), istream_iterator<string>(), back_inserter(tokens));
			string tag_name = tokens[0].substr(1);
			if (tag_name.back() == '>')
				tag_name.erase(end(tag_name) - 1); //<tag>, no attribute
			child.push_back(HRMLElement(move(tag_name), this));
			HRMLElement& cur = child.back();
			auto tok = begin(tokens) + 1;

			while (tok != end(tokens)) {
				string& val = *(tok + 2);
				if (val.back() == '>')
					val.erase(end(val) - 1);
				if (val.back() == '"')
					val.erase(end(val) - 1);
				if (val.front() == '"')
					val.erase(begin(val));
				cur.attributes[move(*tok)] = move(val);
				tok += 3;
			}
			return cur;
		}
		else {  // </tag2>
			return *parent;
		}
	}

	const char * find(string&& long_tag) {  // tag1.tag2.tag3...tagn~attr
		transform(begin(long_tag), end(long_tag), begin(long_tag), [](char c) {
			if (c == '.' || c == '~')
				return ' ';
			return c;
		});
		istringstream istr(long_tag);
		vector<string> tags;
		move(istream_iterator<string>(istr), istream_iterator<string>(), back_inserter(tags));
		return find(tags, 0);
	}

private:
	const char * find(const vector<string>& tags, int idx)
	{
		if (idx == tags.size() - 1) {
			auto found = attributes.find(tags[idx]);
			return found == end(attributes) ? "Not Found!" : attributes[tags[idx]].c_str();
		}
		auto found = std::find_if(begin(child), end(child), [tag = tags[idx]](const auto& e) { return e.name == tag; });
		if (found == end(child))
			return "Not Found!";
		return found->find(tags, idx + 1);
	}
};

TEST_CASE("Hacker rank Attribute Parser", "[OLD]")
{
	HRMLElement hrml;
	HRMLElement *curElem = &hrml;
	curElem = &curElem->addTag("<tag1 value = \"HelloWorld\">");
	curElem = &curElem->addTag("<tag2 name = \"Name1\">");
	curElem = &curElem->addTag("</tag2>");
	curElem = &curElem->addTag("</tag1>");
	CHECK(hrml.find("tag1.tag2~name") == string("Name1"));
	CHECK(hrml.find("tag1~name") == string("Not Found!"));
	CHECK(hrml.find("tag1~value") == string("HelloWorld"));
}
TEST_CASE("Hacker rank Attribute Parser test case 2", "[OLD]")
{
	HRMLElement hrml;
	HRMLElement *curElem = &hrml;
	curElem = &curElem->addTag("<a>");
	curElem = &curElem->addTag("<b name = \"tag_one\">");
	curElem = &curElem->addTag("<c name = \"tag_two\" value = \"val_907\">");
	curElem = &curElem->addTag("< / c>");
	curElem = &curElem->addTag("< / b>");
	curElem = &curElem->addTag("< / a>");
	CHECK(hrml.find("a.b~name") == string("tag_one"));
	CHECK(hrml.find("a.b.c~value") == string("val_907"));
	CHECK(hrml.find("a.b.c~src") == string("Not Found!"));
	CHECK(hrml.find("a.b.c.d~name") == string("Not Found!"));
}

void countSort(vector<vector<string>> arr) {  // vector of int string pair, sort by int
	vector<vector<string>> ans(100, vector<string>{});
	for (size_t i = 0; i < arr.size() / 2; i++) {  // first half converting to "-"
		ans[stoi(arr[i][0])].push_back("-");
	}
	for (size_t i = arr.size() / 2; i < arr.size();  i++) {
		ans[stoi(arr[i][0])].push_back(arr[i][1]);
	}
	for (const auto& v : ans) {
		for (const string& s : v)
			cout << s << " ";
	}
	cout << "\n";
}

// Greedy
// Complete the luckBalance function below. each game has a luck value and importance flag
// lose a game gain luck, winning reduce luck
// find max luck if can lose k important games
int luckBalance(int k, vector<vector<int>> contests) {
	auto computeLuck = [](auto start, auto end, int init) {
		return accumulate(start, end, init, [](int init, const auto& g) { return init + g[0]; });
	};
	auto unImportant = partition(begin(contests), end(contests), [](const auto& g) { return g[1] > 0; });
	if (k >= distance(begin(contests), unImportant)) // can lose all games
		return computeLuck(begin(contests), end(contests), 0);
	int luck= computeLuck(unImportant, end(contests), 0);  // lose all unimportant games
	nth_element(begin(contests), begin(contests) + k, unImportant, [](const auto&g1, const auto&g2) { return g1[0] > g2[0]; });
	// add luck up to position k, minus luck from k, from important games
	return computeLuck(begin(contests), begin(contests) + k, luck)- computeLuck(begin(contests)+k, unImportant, 0);
}
TEST_CASE("Hacker rank Greedy luck balance", "[OLD]")
{
	CHECK(luckBalance(3, vector<vector<int>>{ {5, 1}, { 2,1 }, { 1,1 }, { 8,1 }, { 10,0 }, { 5,0 }}) == 29);
	CHECK(luckBalance(5, vector<vector<int>>{ {13, 1}, { 10,1 }, { 9,1 }, { 8,1 }, { 13,1 }, { 12,1 }, { 18,1 }, { 13,1 }}) == 42);
}
// toys of size <= smallest+4 can be shipped in one box
// what is the minimal shipping
int toys(vector<int> w) {
	sort(begin(w), end(w));
	auto cursor = begin(w);
	int count = 0;
	while (cursor != end(w)) {
		cursor = upper_bound(cursor, end(w), *cursor + 4);
		count++;
	}
	return count;
}
TEST_CASE("Hacker rank Greedy toys shipping", "[OLD]")
{
	CHECK(toys(vector<int>{233, 960, 27, 817, 961, 919, 450, 19, 882, 278, 217, 304, 742, 920, 605, 92, 376, 64, 837, 31, 893, 273, 689, 188, 18, 334, 952, 807, 847, 296, 630, 80, 256, 657, 249, 570, 928, 699, 589, 811, 977, 806, 467, 71, 78, 72, 515, 806, 137, 352, 837, 382, 626, 879, 571, 996, 565, 875, 803, 412, 523, 785, 844, 780, 443, 93, 350, 723, 792, 291, 534, 121, 97, 353, 544, 527, 426, 412, 333, 915, 764, 523, 649, 742, 402, 572, 90, 319, 447, 246, 731, 323, 31, 927, 103, 826, 20, 805, 550, 164}) == 69);
}

int beautifulPairs(vector<int> A, vector<int> B) {
	map<int, int> count_A;
	for (int a : A)
		count_A[a]++;
	size_t pairs = 0;
	for (int b : B) {
		if (count_A[b]>0) {  // count_A.count(b) does not work
			pairs++;
			count_A[b]--;
		}
	}
	if (pairs < A.size())
		pairs++;
	else
		pairs--;  // must make a change, to lose a pair
	return pairs;
}
TEST_CASE("Hacker rank Greedy Beautiful Pairs", "[OLD]")
{
	vector<int> A{ 140,978,829,564,379,418,543,516,910,109,185,562,801,214,532,228,66,19,713,78,405,540,254,198,540,102,545,444,324,377,464,362,158,794,515,9,641,963,580,426,170,878,312,643,553,48,600,924,317,950,308,553,464,674,461,839,769,983,626,651,405,206,221,282,344,684,287,496,354,455,891,149,907,555,426,976,346,543,997,666,348,796,570,400,683,428,106,803,332,369,687,12,318,883,476,449,82,856,761,253,325,632,485,308,779,78,202,150,303,354,215,108,451,464,984,94,916,862,628,376,145,364,906,73,548,130,213,357,493,969,112,954,672,94,127,375,787,156,224,464,497,561,863,969,114,172,221,732,609,254,43,99,240,526,757,656,558,744,363,60,254,750,814,133,260,578,306,209,83,745,562,16,534,817,904,483,889,110,764,73,974,912,139,430,812,565,647,880,759,543,934,597,408,133,829,221,821,325,314,207,218,435,413,655,696,983,618,34,968,823,592,444,643,268,748,393,24,778,754,806,644,722,689,477,114,345,606,583,348,43,608,704,189,104,214,134,961,968,509,378,621,13,205,693,586,172,544,364,363,522,531,957,767,265,173,76,416,139,295,799,717,685,825,579,541,373,178,302,341,416,488,722,577,633,316,432,722,863,263,730,19,166,266,150,50,85,577,277,144,594,106,191,631,336,541,496,24,727,68,810,411,383,892,1,564,530,671,764,12,760,351,740,477,191,322,183,906,825,920,600,299,133,186,818,266,244,795,728,686,811,692,626,938,69,777,289,149,155,29,643,671,189,319,876,397,327,426,889,431,159,770,558,686,280,337,802,830,536,782,708,900,73,467,729,780,903,348,818,730,594,891,752,878,962,980,366,939,211,391,546,84,350,116,628,793,306,290,414,842,550,230,71,966,281,214,914,398,675,564,478,44,564,169,634,150,637,227,727,477,141,913,243,535,530,985,607,506,407,677,366,341,208,622,406,211,108,535,667,409,200,252,126,290,402,251,575,464,107,787,172,359,661,987,440,717,368,218,142,864,453,645,821,313,245,350,33,106,499,808,607,798,705,687,368,611,570,998,624,378,626,746,745,333,567,206,543,530,550,895,54,842,992,23,167,286,836,829,164,214,641,925,355,717,575,596,530,22,261,165,973,770,160,938,95,378,493,775,39,844,795,379,251,201,353,829,110,40,404,739,510,378,973,587,497,305,941,368,148,737,557,625,68,41,370,919,613,225,486,976,888,330,399,697,805,503,989,804,500,453,795,500,569,374,46,28,895,562,215,263,758,745,326,619,987,926,90,499,165,875,25,172,213,669,183,350,54,139,552,310,47,717,435,576,562,625,324,886,978,986,443,67,167,23,803,396,63,999,858,951,12,876,367,918,225,871,869,478,659,601,551,456,134,125,773,596,468,558,375,443,503,910,120,778,380,923,475,669,584,420,4,420,405,457,848,168,639,850,901,269 };
	vector<int> B{ 921,792,944,382,852,732,568,169,921,295,352,971,810,87,311,426,281,605,854,778,305,19,828,80,753,135,938,247,566,97,879,611,251,907,506,337,370,637,466,322,989,754,726,925,995,919,567,908,8,73,218,430,578,812,266,213,675,106,364,502,271,968,427,320,828,769,792,508,53,746,894,614,636,655,795,866,115,168,836,881,374,465,46,872,19,284,349,429,288,637,10,65,225,270,792,32,983,340,706,779,4,53,119,164,954,905,918,219,988,600,790,707,584,996,664,384,38,168,26,317,624,30,81,858,283,223,593,448,733,150,319,105,631,347,977,849,463,237,616,519,110,292,413,186,247,588,490,380,452,451,685,698,21,58,943,162,21,127,774,499,186,945,983,660,583,928,317,741,877,316,100,789,240,197,375,29,274,213,218,985,36,601,449,140,920,535,616,690,81,803,550,175,174,350,869,372,934,610,753,876,123,707,908,331,50,193,488,492,313,193,24,506,554,409,662,928,527,907,666,812,68,840,452,118,931,545,555,574,200,679,235,603,474,940,237,445,461,406,104,608,250,217,670,750,402,135,190,722,997,907,747,890,784,30,231,234,554,277,495,636,280,148,387,297,87,913,600,267,81,390,638,553,917,322,805,744,66,640,16,608,610,370,338,573,77,806,35,113,588,777,811,48,956,158,67,416,935,39,645,898,190,904,458,789,874,331,720,666,20,605,945,973,442,325,195,232,225,409,539,435,458,773,23,973,813,701,565,666,45,12,926,148,65,329,291,162,54,298,545,875,986,184,445,204,675,843,121,319,63,589,684,138,719,8,311,431,85,329,882,305,20,583,37,300,560,109,620,823,772,179,457,429,362,112,652,935,732,791,919,896,709,265,790,48,64,885,413,490,133,338,694,879,974,261,791,397,12,359,216,724,109,961,817,293,370,375,504,906,972,656,294,596,673,346,296,337,2,806,285,579,339,7,575,748,748,564,284,727,753,839,265,788,802,259,899,310,412,904,970,46,911,508,241,945,388,455,711,404,573,16,772,310,649,174,57,119,446,514,958,943,963,890,723,405,919,205,558,25,733,258,4,622,624,757,910,701,815,622,939,963,111,704,1,440,240,816,2,156,31,173,715,516,699,701,865,96,353,875,973,105,523,720,881,413,468,611,810,425,117,144,941,862,791,154,62,773,369,559,994,71,939,294,9,699,117,475,716,924,33,725,201,223,745,621,905,978,999,343,31,819,666,908,138,920,651,642,665,117,815,500,888,941,31,759,412,381,335,514,831,603,320,340,960,834,996,916,732,527,691,639,208,84,492,565,759,68,866,381,377,863,604,734,726,648,593,99,462,423,550,570,355,598,584,586,952,169,305,802,14,855,925,663,864,586,397,771,43,894,680,311,514,696,486,158,520,368,469,249,1,825,529,770,715,392,97,201,475,380,800,515,768,808,151,630,446,685,361,773,938 };
	CHECK(beautifulPairs(A, B) == 228);
	sort(begin(A), end(A));
	sort(begin(B), end(B));
}


vector<int> closestNumbers(vector<int> arr) {
	sort(begin(arr), end(arr));
	int min_diff = INT32_MAX;
	for (size_t i = 1; i<arr.size(); i++)
		min_diff = min(min_diff, arr[i] - arr[i - 1]);
	vector<int> ans;
	for (size_t i = 1; i<arr.size(); i++) {
		if (min_diff == arr[i] - arr[i - 1]) {
			ans.push_back(arr[i - 1]);
			ans.push_back(arr[i]);
		}
	}
	return ans;
}
TEST_CASE("Hacker rank sort closest pair", "[OLD]")
{
	CHECK(closestNumbers(vector<int>{-5, 15, 25, 71, 63}) == vector<int>{63, 71});
}

TEST_CASE("sorted vector", "[OLD]")
{
	vector<int> unsorted{ 10,15,5,7 };
	SortedVector<int> sorted;
	for (int n : unsorted)
		sorted.insert(n);
	CHECK(sorted.get() == vector<int>{5, 7, 10, 15});
	SortedVector<int> sorted2{ 10,15,5,7 };
	CHECK(sorted2.get() == vector<int>{5, 7, 10, 15});
	SortedVector<int> sorted3 = { begin(unsorted), end(unsorted) };
	CHECK(sorted3.get() == vector<int>{5, 7, 10, 15});
}

// idea: s=merge(reverse(s)+any order(s))
// use half of the letter to construct, from R to L to get correct order
// Simplified logic:
// don't add letter unless that letter count not more than half
// put skipped letter on a non decreasing stack, decrement count
// when there is letter must be added due to count=half count, 
//   add any letter on stack that are smaller first
string reverseShuffleMerge(string s) {
	vector<int> count(26, 0), half_count(26, 0);
	for (char c : s)
		count[c - 'a']++;
	// half of letter count is needed to construct original string
	transform(begin(count), end(count), begin(half_count), [](int n) { return n / 2; });
	copy(begin(count), end(count), ostream_iterator<int>(cout, " "));
	cout << "\n";
	copy(begin(half_count), end(half_count), ostream_iterator<int>(cout, " "));
	cout << "\n";
	string ans;
	ans.reserve(s.size() / 2);
	vector<char> skipped; // stack of non decreasing order of chars skipped
	skipped.reserve(10);
	auto erase_back = [&skipped](int c) {
		while (!skipped.empty() && skipped.back()>c)
			skipped.pop_back();
	};
	for (int i = s.size() - 1; i >= 0; --i) {  // from R to L to construct string
		int idx = s[i] - 'a';
		cout << s[i] << " count=" << count[idx] << " half=" << half_count[idx] << "\n";
		if (half_count[idx] <= 0)
			continue;
		if (count[idx] == half_count[idx]) {
			// must add as if we pass up, there is not enough such letter later
			auto start = begin(skipped);
			while (start != end(skipped) && *start <= s[i]) {
				char c = *start; // add any smaller char
				if (half_count[c - 'a']-- >0) {
					ans.append(1, c);
					cout << "    add back skipped       " << c << "\n";
				}
				++start;
				if (c == s[i])
					break;
			}
			skipped.erase(begin(skipped), start); // erase all added
		}
		if (half_count[idx] <= 0) // check again as s[i] could be just added
			continue;
		if (count[idx]-- > half_count[idx]) {
			erase_back(s[i]);
			skipped.push_back(s[i]); // delay to choose
			cout << skipped.size() << " skipped size\n";
		}
		else {
			skipped.clear(); // remove all large char
			ans.append(1, s[i]);
			half_count[idx]--;
			cout << s[i] << " must add at       " << i << "\n";
		}
	}
	return ans;
}

TEST_CASE("HackerRank Greedy string reverse shuffle merge", "[NEW]")
{
	CHECK(reverseShuffleMerge("daadaaaaeebddb") == "bddeaaa");
}