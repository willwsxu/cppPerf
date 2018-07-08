#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

#include <complex>
#include <regex>
class String {
public:
	// 791. Custom Sort String, sort t same as letter order in s
	string customSortString(string S, string T) {  // beat 100%
		size_t order[128];
		for (size_t i = 0; i < S.size(); i++)
			order[S[i]] = i;
		sort(begin(T), end(T), [&order](char a, char b) { return order[a] < order[b]; });
		return T;
	}

	// 537. Complex Number Multiplication, in form of a+bi, no extra blank
	// Input: "1+-1i", "1+-1i"
	// Output: "0+-2i"
	vector<int> parseComplex(string s) {
		regex clx("[\\+i]+");
		vector<string> ord;
		copy(sregex_token_iterator(s.begin(), s.end(), clx, -1), sregex_token_iterator(), back_inserter(ord));
		return{ atoi(ord[0].c_str()),atoi(ord[1].c_str()) };
	}
	string complexNumberMultiply(string a, string b) {  // beat 59%
		auto c1 = parseComplex(a);
		auto c2 = parseComplex(b);
		auto r = c1[0] * c2[0] - c1[1] * c2[1];
		auto i = c1[0] * c2[1] + c1[1] * c2[0];
		return to_string(r).append(1, '+').append(to_string(i)).append(1, 'i');
	}

	// 553. Optimal Division, maximize result, add / and parenthesis only, no redundant parenthesis
	// size [1,10], nums val [2,1000]
	// simple algorithm to maximize division of integers:
	// use first int as dividend, rest as divisor. divide in sequential order to minimize divisor
	string optimalDivision(vector<int>& nums) { // tricky question, but easy. beat 100%
		if (nums.empty())
			return "";
		string str;
		str.reserve(50);
		str.append(to_string(nums[0]));
		int n = nums.size();
		if (n == 1)
			return str;
		str.append(1, '/');
		if (n > 2)
			str.append(1, '(');
		for (int i = 1; i < n; i++) {
			str.append(to_string(nums[i]));
			if (i < n - 1)
				str.append(1, '/');
		}
		if (n > 2)
			str.append(1, ')');
		return str;
	}

	// 856. Score of Parentheses
	//() 1, ()() 1+1=2, (()) 2*1=2
	size_t pos=0;
	int scoreOfParenthesesHelp(const string& S) {
		int res = 0;
		while (pos < S.size()) {
			char c = S[pos++];
			if (c == '(') {
				if (S[pos] == ')') {
					res++;
					pos++;
				}
				else
					res += 2 * scoreOfParenthesesHelp(S);
			}
			else
				return res;
		}
		return res;
	}

	int scoreOfParentheses(const string& S) {
		pos = 0;
		return scoreOfParenthesesHelp(S);
	}

};


TEST_CASE("Complex multiply", "[COMPLEX]")
{
	String s;
	CHECK(s.complexNumberMultiply("1+1i", "1+1i") == "0+2i");
}


TEST_CASE("parenthesis score", "[paren]")
{
	String s;
	CHECK(s.scoreOfParentheses("((()())())") == 10);
	CHECK(s.scoreOfParentheses("()") == 1);
	CHECK(s.scoreOfParentheses("()()") == 2);
	CHECK(s.scoreOfParentheses("(())") == 2);
	CHECK(s.scoreOfParentheses("(()(()))") == 6);
}
