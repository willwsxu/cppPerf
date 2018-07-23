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
		for (char c : s) {
			switch (c) {
			case '+':
			case'-':
			case '*':
			case '/':
			case' ':
				if (number) {
					tokens.emplace_back(num);  // parse operand
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
			tokens.emplace_back(num);
		if (tokens.empty())
			return 0;
		deque<Token> eval;
		eval.push_back(tokens[0]);
		auto t = begin(tokens) + 1;
		while (t != end(tokens)) {
			// t must be operator;
			switch (t->val) {
			case '*':
			{
				int left = eval.back().val;
				eval.pop_back();
				eval.push_back(Token(left* (++t)->val));
				break;
			}
			case '/':
			{
				int left = eval.back().val;
				eval.pop_back();
				eval.push_back(Token(left / (++t)->val));
				break;
			}
			default:  // + -
				eval.push_back(*(t));
				eval.push_back(*(++t));
			}
			++t;  // move to next operator
		}
		// second pass
		int ans = eval.front().val;
		auto e = begin(eval) + 1;
		while (e != end(eval)) {
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
