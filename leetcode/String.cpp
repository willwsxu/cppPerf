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
	float maximize(const vector<int>& nums, int start, int last)
	{
		int n = last - start;
		if (n < 1)
			return 0;
		if (n == 1)
			return (float)nums[start];
		float div2 = (float)nums[start] / nums[start + 1];
		if (n == 2)
			return div2;
		float v1 = (float)nums[start] / minimize(nums, start + 1, last);
		float v2 = div2/ minimize(nums, start + 2, last);
		return max(v1, v2);
	}
	float minimize(const vector<int>& nums, int start, int last)
	{
		int n = last - start;
		if (n < 1)
			return 0;
		if (n == 1)
			return (float)nums[start];
		float div2 = (float)nums[start] / nums[start + 1];
		if (n == 2)
			return div2;
		float v1 = (float)nums[start] / maximize(nums, start + 1, last);
		float v2 = div2 / maximize(nums, start + 2, last);
		return min(v1, v2);
	}
	string optimalDivision(vector<int>& nums) {
		return "";
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


TEST_CASE("parenthesis score", "[NEW]")
{
	String s;
	CHECK(s.scoreOfParentheses("((()())())") == 10);
	CHECK(s.scoreOfParentheses("()") == 1);
	CHECK(s.scoreOfParentheses("()()") == 2);
	CHECK(s.scoreOfParentheses("(())") == 2);
	CHECK(s.scoreOfParentheses("(()(()))") == 6);
}