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
};


TEST_CASE("Complex multiply", "[NEW]")
{
	String s;
	CHECK(s.complexNumberMultiply("1+1i", "1+1i") == "0+2i");
}