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
};

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
	vector<string> removeComments(vector<string>& source) {
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

TEST_CASE("remove c++ comment", "[NEW]")
{
	CHECK(CppComments().removeComments(vector<string>{ "struct Node{",
		"    /*/ declare members;/**/",
		"    int size;",
		"    /**/int val;",
		"};" }) == vector<string>{"struct Node{", "    ", "    int size;", "    int val;", "};"});

	CHECK(CppComments().removeComments(vector<string>{"a/*comment", "line", "more_comment*/b"}) == vector<string>{"ab"});
}
