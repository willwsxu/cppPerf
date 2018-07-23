#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
using namespace std;

struct Token
{
	const int val;
	bool op;    // operator
	Token(string s) : val(atoi(s.c_str())), op(false) { }
	Token(char c) : val(c), op(true) { }
	Token(int i) : val(i), op(false) { }
};

class Calculator {
public:
	int calculate(string s) {  // improve from 20ms to 12ms after atoi
		vector<Token> tokens;
		bool number = false;
		int num = 0;
		auto multi_divide = [&tokens](int num) { // do multiply and division as token is parsed
			if (!tokens.empty()) {
				switch (tokens.back().val) {
				case '*':
					tokens.pop_back();
					num *= tokens.back().val;
					tokens.pop_back();
					break;
				case '/':
					tokens.pop_back();
					num = tokens.back().val / num;
					tokens.pop_back();
					break;
				}
			}
			tokens.emplace_back(num);  // push operand or result of multiplication or division
		};
		for (char c : s) {
			switch (c) {
			case '+':
			case'-':
			case '*':
			case '/':
			case' ':
				if (number) {
					multi_divide(num);
					number = false;
					num = 0;
				}
				if (c != ' ')
					tokens.emplace_back(c);  // add operator
				break;
			default:  //  digits
				number = true;
				num = num * 10 + c - '0';
				break;
			}
		}
		if (number) 
			multi_divide(num);
		if (tokens.empty())
			return 0;
		int ans = tokens.front().val;
		auto e = begin(tokens) + 1;
		while (e != end(tokens)) {
			switch (e->val) {
			case '+':
				ans += (++e)->val;
				break;
			case '-':
				ans -= (++e)->val;
				break;
			}
			++e;
		}
		return ans;
	}
};


TEST_CASE("calculator", "[NEW]")
{
	Calculator c;
	CHECK(c.calculate(" 3+5 / 2 ") == 5);
	CHECK(c.calculate(" 3-5 / 2 ") == 1);
	CHECK(c.calculate("3+2*2") == 7);
}
