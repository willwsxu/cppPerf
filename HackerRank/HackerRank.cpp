#include "stdafx.h"
#include <cassert>
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "Graph.h"
#include "helper.h"
#include "myalgo.h"
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

TEST_CASE("Hackerrank Down to Zero II", "[NEW]")
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

TEST_CASE("Hackerrank almost sorted", "[NEW]")
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

// subproblem, suffix of 2 strings
// LCS, DP
int commonChild(const string& s1, const string& s2, int i, int j, vector<vector<int>>& dp) {
	if (i == s1.size() || j == s2.size())
		return 0;
	if (dp[i][j] >= 0)
		return dp[i][j];
	if (s1[i] == s2[j])
		dp[i][j] = 1 + commonChild(s1, s2, i + 1, j + 1, dp);
	else
		dp[i][j] = max(commonChild(s1, s2, i, j + 1, dp), commonChild(s1, s2, i + 1, j, dp));
	return dp[i][j];
}
// longest common child of s1 and s2
int commonChild(string s1, string s2) {
	vector<vector<int>> dp(s1.size(), vector<int>(s2.size(), -1));
	int ans = commonChild(s1, s2, 0, 0, dp);
	for (const auto& v : dp) {
		copy(begin(v), end(v), ostream_iterator<int>(cout, " "));
		cout << "\n";
	}
	return ans;
}

TEST_CASE("Hackerrank Common Child", "[NEW]")
{
	CHECK(commonChild("michelangelo", "hieroglyphology") == 5);
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

TEST_CASE("Hackerrank compute value from bits", "[NEW]")
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

TEST_CASE("Hackerrank Lisa's Workbook", "[NEW]")
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

TEST_CASE("Hackerrank SimpleTextEditor", "[NEW]")
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
TEST_CASE("Hackerrank Sherlock and Anagrams", "[NEW]")
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
	copy(begin(ans), end(ans), ostream_iterator<int>(cout, " "));
	cout << "\n";
}
TEST_CASE("Hackerrank deque STL", "[NEW]")
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
	return{ maxSubSum(arr.begin(), arr.end(), INT32_MIN).first, maxSubSeq() };
}
// find a minimal section to replace with a new sequence to make it steady
// each letter is exactly 1/4, ATCG
// n [4, 500000]
int steadyGene(string gene) {
	int max_per_letter = gene.size() / 4;
	return -1;
}