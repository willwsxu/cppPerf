#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "helper.h"

using namespace std;

class BackTracking {
	// 842. Split Array into Fibonacci Sequence, a least 3 numbers
	// number cannot start with 0, unless it is 0
	// 0 <= F[i] <= 2^31 - 1
	vector<int> ans;
	bool splitIntoFibonacci(const string& S, int start) {
		int len = S.size();
		if (start == len)
			return true;
		int n = ans.size();
		int sum = ans[n - 1] + ans[n - 2];
		for (int j = start; j < len; j++) {
			int next = to_int(S, start, j);
			if (next < 0)
				return false;
			if (sum == next) {
				ans.push_back(next);
				if (splitIntoFibonacci(S, j + 1))
					return true;
				ans.pop_back();
			}
			else if (next > sum)  // pruning, no need to check further
				return false;
			if (S[start] == '0')  // no leading 0 unless it is 0
				break;
		}
		return false;
	}

public:
	vector<int> splitIntoFibonacci(string S) {
		int n = S.size();
		for (int i = 0; i < n - 2; i++) {
			int next = to_int(S, 0, i);
			if (next < 0)  // overflow
				break;
			ans.push_back(next);
			for (int j = i + 1; j < n - 1; j++) {
				int next = to_int(S, i + 1, j);
				if (next < 0)  // overflow
					break;
				ans.push_back(next);
				if (splitIntoFibonacci(S, j + 1))
					return ans;
				ans.pop_back();
				if (S[i + 1] == '0')  // no leading 0 unless it is 0
					break;
			}
			ans.pop_back();
			if (S[0] == '0')  // no leading 0 unless it is 0
				break;
		}
		return{};
	}
	void grayCode2(int n, int& prefix, int pos, vector<int>& ans) {
		if (pos == n) {
			ans.push_back(prefix);
			return;
		}
		grayCode2(n, prefix, pos + 1, ans);
		prefix ^= (1 << pos);  // flip the bit
		grayCode2(n, prefix, pos + 1, ans);
	}
	// 89. Gray Code, two successive values differ in only one bit
	vector<int> grayCode(int n) {  // beat 74%
		vector<int> ans;
		ans.reserve(1 << n);
		int code = 0;
		grayCode2(n, code, 0, ans);
		return ans;
	}
	// 967. Numbers of digit With Same Consecutive Differences
	void numsSameConsecDiff(int N, int K, vector<int>&digits, vector<int>& result)
	{
		if (digits.back() < 0 || digits.back() > 9)
			return;
		if (N == digits.size()) {
			int ans = 0;
			for (int i : digits)
				ans = ans * 10 + i;
			result.push_back(ans);
			return;
		}
		int last = digits.back();
		digits.push_back(last + K);
		numsSameConsecDiff(N, K, digits, result);
		if (K > 0) {
			digits.back() = last - K;
			numsSameConsecDiff(N, K, digits, result);
		}
		digits.pop_back();
	}
	vector<int> numsSameConsecDiff(int N, int K) {
		vector<int> result;
		if (N == 1)
			result.push_back(0);
		for (int i = 1; i < 10; i++) {
			vector<int> digits{ i };
			numsSameConsecDiff(N, K, digits, result);
		}
		return result;
	}

	// 996. Number of Squareful Arrays (backtracking + Graph)
	// similar to XR project Brick By Brick
	// sum of 2 numbers form a square, generate all edges to use this requirement
	// start from any number, try all possible ways to fill next numbers so all neighbors form a square sum
	bool is_square(int s) {
		int r = static_cast<int>(sqrt(s) + 0.1);
		return r*r == s;
	}
	int numSquarefulPerms(int remaining, int prev, map<int, int>& count, map<int, set<int>>&  square_pairs) {
		if (remaining == 0)  // permutation complete
			return 1;
		int ans = 0;
		for (int next : square_pairs[prev]) {  // iterator all numbers that forms square with prev
			if (count[next] == 0)  // number is used up
				continue;
			count[next]--;
			ans += numSquarefulPerms(remaining - 1, next, count, square_pairs);
			count[next]++;
		}
		return ans;
	}
public:
	int numSquarefulPerms(vector<int>& A) {
		if (A.size() == 1)  // special case?
			return 1;
		map<int, int> count;  // count of unique number
		for (int a : A)
			count[a]++;
		map<int, set<int>>  square_pairs;  // all numbers can form a square with another number
		for (auto a = begin(count); a != end(count); ++a) {
			if (a->second>1 && is_square(a->first * 2))
			{
				square_pairs[a->first].insert(a->first);  // e.g. 2+2 is a square
			}
			auto b = a;
			for (++b; b != end(count); ++b) {
				if (is_square(a->first + b->first)) {  // 2+7 is a square
					square_pairs[a->first].insert(b->first);
					square_pairs[b->first].insert(a->first);
				}
			}
		}
		if (count.size() != square_pairs.size())  // at least one number cannot form a square with any
			return 0;
		int total = 0;
		for (auto& c : count) {
			if (c.second == 0)  // number is used up
				continue;
			c.second--;
			total += numSquarefulPerms(A.size() - 1, c.first, count, square_pairs);
			c.second++;  // backtracking
		}
		return total;
	}
};
TEST_CASE("967. Numbers With Same Consecutive Differences", "[NEW]")
{
	CHECK(BackTracking().numsSameConsecDiff(2, 0) == vector<int>{11, 22, 33, 44, 55, 66, 77, 88, 99});
	CHECK(BackTracking().numsSameConsecDiff(1, 7) == vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
	CHECK(BackTracking().numsSameConsecDiff(3, 7) == vector<int>{181, 292, 707, 818, 929});
	CHECK(BackTracking().numsSameConsecDiff(2, 1) == vector<int>{12, 10, 23, 21, 34, 32, 45, 43, 56, 54, 67, 65, 78, 76, 89, 87, 98});
}

TEST_CASE("89. Gray Code", "[NEW]")
{
	CHECK(BackTracking().grayCode(2) == vector<int>{0, 2, 3, 1});
}
TEST_CASE("splitIntoFibonacci", "[FIB]")
{
	BackTracking s;
	CHECK(s.splitIntoFibonacci("214748364721474836422147483641") == vector<int>{});
	CHECK(s.splitIntoFibonacci("0123") == vector<int>{});
	CHECK(s.splitIntoFibonacci("1023") == vector<int>{});
	CHECK(s.splitIntoFibonacci("1203") == vector<int>{});
	CHECK(s.splitIntoFibonacci("123456579") == vector<int>{123, 456, 579});
}

class CppComments
{
public:
	/*
	// max 100 lines, 80 char per line*/
	// The first effective comment takes precedence over others: 
	// if the string // occurs in a block comment, it is ignored. 
	// Similarly, if the string /* occurs in a line or block comment, it is also ignored
	//  but */ after // is effective
	std::pair<bool, size_t> findOpen(const string& line, int start)
	{
		auto single = line.find("//", start);
		auto block = line.find("/*", start);
		if (single == string::npos && block == string::npos)  // find none
			return{ false, string::npos };
		else if (single == string::npos)  // block comment
			return{ true, block };
		else if (block == string::npos)   // single line
			return{ false, single };
		if (single < block)
			return{ false, single };
		return{ true, block };
	}
	pair<string, bool> processLine(const string& line, int start, bool openBlock)
	{
		if (start >= (int)line.size())
			return{ "", openBlock };
		if (openBlock) {
			auto multi = line.find("*/", start);
			if (multi == string::npos)
				return{ "", openBlock };
			else {
				return processLine(line, multi + 2, false);
			}
		}
		else {
			auto found = findOpen(line, start);
			if (found.second == string::npos)
				return{ line.substr(start), false };
			if (!found.first)
				return{ line.substr(start, found.second - start), false };
			auto next = processLine(line, found.second + 2, true);
			return{ line.substr(start, found.second - start) + next.first, next.second };
		}
	}
	vector<string> removeComments(vector<string>& source) { // beat 100%
		bool openBlock = false;
		vector<string> ans;
		for (const string& line : source) {
			auto x = processLine(line, 0, openBlock);
			if (!x.first.empty()) {
				if (openBlock)  // implicit newline characters can be deleted by block comments
					ans.back().append(x.first);
				else
					ans.push_back(x.first);
			}
			openBlock = x.second;
		}
		return ans;
	}
};

TEST_CASE("remove c++ comment", "[BT]")
{
	CHECK(CppComments().removeComments(vector<string>{ "struct Node{",
		"    /*/ declare members;/**/",
		"    int size;",
		"    /**/int val;",
		"};" }) == vector<string>{"struct Node{", "    ", "    int size;", "    int val;", "};"});

	CHECK(CppComments().removeComments(vector<string>{"a/*comment", "line", "more_comment*/b"}) == vector<string>{"ab"});
}


class StringParenthesis
{
public:
	// 678. Valid Parenthesis String, (*)) is valid because * can be ( or ) or blank
	bool valid(const string& s, int start, int balance) {
		for (size_t pos = start; pos < s.size(); pos++)
		{
			switch (s[pos]) {
			case '(':
				balance++;
				break;
			case ')':
				if (balance > 0) {
					balance--;
					break;
				}
				return false;
			case '*':
				if (valid(s, pos + 1, balance))
					return true;
				if (valid(s, pos + 1, balance + 1))
					return true;
				if (balance > 0)
					return valid(s, pos + 1, balance - 1);
				return false;
			}
		}
		return balance == 0;
	}
	bool checkValidString(string s) {  // improve backtracking, speed up from 300 to 160 ms, but still only 16%
		return valid(s, 0, 0);
	}

	bool checkValidString2(string s) {  // borrow clever idea to count bounds, 0ms
		int lb=0, ub=0; // count ( as positive, ) as negative. keep track of upper and lower bound
		for (char c : s) {
			switch (c) {
			case '(':	lb++, ub++;	break;
			case ')':   lb--, ub--; break;
			case '*':   lb--, ub++; break;
			}
			if (ub < 0)
				return false;
			if (lb < 0)
				lb = 0;
		}
		return lb == 0;
	}

	//784. Letter Case Permutation, transform every letter individually to be lowercase or uppercase to create another string
	// Len <= 12
	void letterCasePermutation(const string& S, size_t pos, string target, vector<string>&result) {
		if (pos == S.size()) {
			result.push_back(target);
			return;
		}
		for (size_t i = pos; i < S.size(); i++) {
			if (!isdigit(S[i])) {
				string newString = target + S.substr(pos, i - pos + 1);
				if (i > pos)
					target.append(S.begin() + pos, S.begin() + i);
				if (isupper(S[i]))
					target.append(1, tolower(S[i]));
				else
					target.append(1, toupper(S[i]));
				letterCasePermutation(S, i + 1, newString, result);
				letterCasePermutation(S, i + 1, target, result);
				return;
			}
		}
		result.push_back(target.append(S.begin() + pos, S.end()));
	}

public:
	vector<string> letterCasePermutation(string S) {  // beat 100%, could revise to use S in place, eliminate 3rd param
		vector<string> result;
		letterCasePermutation(S, 0, "", result);
		return result;
	}
};

TEST_CASE("Valid Parenthesis", "[BT]")
{
	StringParenthesis p;
	CHECK(p.checkValidString2("(*()())") == true);
	CHECK(p.checkValidString2("(*))") == true);
}

class NQueens {
	class DiagMask
	{
	protected:
		vector<bool> mask;
		DiagMask(int n) :mask(2 * n - 1, false)
		{

		}
		bool get(int idx) {
			if (idx < (int)mask.size())
				return mask[idx];
			return false;
		}
		void set(int idx, bool v)
		{
			if (idx < (int)mask.size())
				mask[idx] = v;
		}
	};
	class ForwardDiagMask : public DiagMask
	{
	public:
		ForwardDiagMask(int n) :DiagMask(n)
		{
		}
		void set(int r, int c) {
			DiagMask::set(r + c, true);
		}
		void reset(int r, int c) {
			DiagMask::set(r + c, false);
		}
		bool get(int r, int c) {
			return DiagMask::get(r + c);
		}
	};
	class BackwardDiagMask : public DiagMask
	{
		int offset;
	public:
		BackwardDiagMask(int n) :DiagMask(n), offset(n - 1)
		{
		}
		void set(int r, int c) {
			DiagMask::set(offset + r - c, true);
		}
		void reset(int r, int c) {
			DiagMask::set(offset + r - c, false);
		}
		bool get(int r, int c) {
			return DiagMask::get(offset + r - c);
		}
	};
	void backtracking(vector<vector<string>>& ans, int n, vector<int>& rows, vector<bool>& mCol, ForwardDiagMask& fwd, BackwardDiagMask& bwd)
	{
		if (rows.size() == n) {
			vector<string> board;
			for (int c : rows) {
				string r(n, '.');     // default row config
				r[c] = 'Q';					// set Q position
				board.push_back(r);  // speed up to 6ms from 10ms after switch to use string, instead of vector to ostream 
			}
			ans.push_back(board);
			return;
		}
		int r = rows.size();
		for (int c = 0; c < n; c++) {  // try each col
			if (mCol[c] || fwd.get(r, c) || bwd.get(r, c))  // invalid
				continue;
			mCol[c] = true;  // set masks for row, and 2 diagonals
			fwd.set(r, c);
			bwd.set(r, c);
			rows.push_back(c); // add col value for this row
			backtracking(ans, n, rows, mCol, fwd, bwd);
			mCol[c] = false;   // reset all states
			fwd.reset(r, c);
			bwd.reset(r, c);
			rows.pop_back();
		}
	}

	int backtrackingCount(int r, vector<bool>& mCol, ForwardDiagMask& fwd, BackwardDiagMask& bwd)
	{
		if (r<0) {
			return 1;
		}
		int ans = 0;
		for (size_t c = 0; c < mCol.size(); c++) {  // try each col
			if (mCol[c] || fwd.get(r, c) || bwd.get(r, c))  // invalid
				continue;
			mCol[c] = true;  // set masks for row, and 2 diagonals
			fwd.set(r, c);
			bwd.set(r, c);
			ans += backtrackingCount(r - 1, mCol, fwd, bwd);
			mCol[c] = false;   // reset all states
			fwd.reset(r, c);
			bwd.reset(r, c);
		}
		return ans;
	}
public:
	vector<vector<string>> solveNQueens(int n) {  // beat 74%
		vector<int> rows;  // rows of the board, its value is the column at each row
		vector<bool> mCol(n, false);
		ForwardDiagMask fwd(n);
		BackwardDiagMask bwd(n);
		vector<vector<string>> ans;
		backtracking(ans, n, rows, mCol, fwd, bwd);
		return ans;
	}
	int totalNQueens(int n) {  // beat 88%
		vector<bool> mCol(n, false);
		ForwardDiagMask fwd(n);
		BackwardDiagMask bwd(n);
		return  backtrackingCount(n - 1, mCol, fwd, bwd);
	}
};


void testNQueens()
{
	NQueens nq;
	vector<vector<string>> ans = nq.solveNQueens(4);
	for (auto& v : ans) {
		copy(begin(v), end(v), std::ostream_iterator<string>(cout, "\n"));
		cout << endl;
	}
	cout << nq.totalNQueens(8) << endl;  // 92
}
