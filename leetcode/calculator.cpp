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
	int calculate(string s) {  // beat 27%
		vector<Token> tokens;
		int start = 0;
		int cur = 0;
		for (char c : s) {
			switch (c) {
			case '+':
			case'-':
			case '*':
			case '/':
			case' ':
				if (cur > start)
					tokens.emplace_back(s.substr(start, cur - start));
				if (c != ' ')
					tokens.emplace_back(s[cur]);
				start = ++cur;
				break;
			default:  //  digits
				cur++;
				break;
			}
		}
		if (cur > start)
			tokens.emplace_back(s.substr(start, cur - start));
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


TEST_CASE("calculator", "[CALC]")
{
	Calculator c;
	CHECK(c.calculate(" 3+5 / 2 ") == 5);
	CHECK(c.calculate(" 3-5 / 2 ") == 1);
	CHECK(c.calculate("3+2*2") == 7);
}
