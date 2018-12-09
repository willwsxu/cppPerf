#include "..\catch.hpp"  // don't put this file in stdafx.h
#include <vector>
#include "helper.h"
#include <numeric>
#include <stack>
using namespace std;


// plates from stack Ai-1, move to stack Bi if divisible by prime[i], or A[i]
vector<int> waiter(vector<int> number, int q) {
	vector<int> primes = make_primes(10000);
	vector<int> ans;
	ans.reserve(number.size());
	for (int i = 0; i < q; i++) {
		auto B= stable_partition(begin(number), end(number), [p = primes[i]](int n) { return n%p != 0; });
		copy(B, end(number), back_inserter(ans));
		if (B == begin(number))
			return ans;
		number.erase(B, end(number));
		reverse(begin(number), end(number));
	}
	copy(rbegin(number), rend(number), back_inserter(ans));
	return ans;
}

TEST_CASE("Hacker rank stack Waiter test primes", "[NEW]")
{
	vector<int> primes = make_primes(10000);
	CHECK(primes[0]== 2);
	CHECK(primes[1] == 3);
	CHECK(primes[2] == 5);
	CHECK(primes[3] == 7);
	vector<int> primes2 = make_primes(200000);
	CHECK( primes2.size() ==17984);
}

TEST_CASE("Hacker rank stack Waiter", "[NEW]")
{
	CHECK(waiter(vector<int>{3,3,4,4,9}, 2) == vector<int>{4,4,9,3,3});
	CHECK(waiter(vector<int>{3, 4, 7, 6, 5}, 1) == vector<int>{4, 6, 3, 7, 5});
}

// idea: use monotonous decreasing stack to max position, then next max, etc
// allow duplciate max value
// buy all stock between prev max and current max, sell at current max
int stockmax(vector<int> prices) {
	if (prices.empty())
		return 0;
	deque<int> decrease_stack{ 0 };  // max value of all previous, up to previous max
	for (size_t i = 1; i<prices.size(); i++) {
		while (!decrease_stack.empty() && prices[decrease_stack.back()]<prices[i])
			decrease_stack.pop_back();
		decrease_stack.push_back(i);
	}
	int start = 0;
	int profit = 0;
	for (int max_p : decrease_stack) {
		if (max_p>start) {
			profit += prices[max_p] * (max_p - start) - accumulate(begin(prices) + start, begin(prices) + max_p, 0);
		}
		start = max_p + 1;
	}
	return profit;
}
TEST_CASE("Hacker rank Greedy Stock Maximize", "[NEW]")
{
	CHECK(stockmax(vector<int>{5, 4, 3, 4, 5}) == 4);
	CHECK(stockmax(vector<int>{5, 3, 2}) == 0);
	CHECK(stockmax(vector<int>{1, 2, 100}) == 197);
}


string isBalanced(string s) {  // medium, actually easy. watch out for edge case
	stack<char> open_brackets;
	auto pop = [&open_brackets](const char c) {
		if (open_brackets.empty() || open_brackets.top() != c)
			return false;
		open_brackets.pop();
		return true;
	};
	for (const char c : s) {
		switch (c) {
		case '(':
		case '[':
		case '{':
			open_brackets.push(c);
			break;
		case ')':
			if (!pop('('))
				return "NO";
			break;
		case ']':
			if (!pop('['))
				return "NO";
			break;
		case '}':
			if (!pop('{'))
				return "NO";
			break;
		}
	}
	return open_brackets.empty() ? "YES" : "NO";
}
TEST_CASE("Hacker rank stack Balanced Brackets", "[NEW]")
{
	CHECK(isBalanced("}][}}(}][))]") == "NO");  // check empty before pop
	CHECK(isBalanced("{[()]}") == "YES");
}

long largestRectangle(vector<int> h) {
	vector<int> left_lower(h.size(), -1);
	deque<int> mono_stack_inc;
	auto stack_op = [&mono_stack_inc, &h](vector<int>&lower_range, int i, auto comp) {
		while (!mono_stack_inc.empty() && comp(h[i], h[mono_stack_inc.back()]))
			mono_stack_inc.pop_back();
		if (!mono_stack_inc.empty())
			lower_range[i] = mono_stack_inc.back();
		mono_stack_inc.push_back(i);
	};
	for (int i = 0; i<(int)h.size(); i++) {
		stack_op(left_lower, i, less_equal<int>());
	}
	mono_stack_inc.clear();
	vector<int> right_lower(h.size(), h.size());
	for (int i = (int)h.size() - 1; i >= 0; i--) {
		stack_op(right_lower, i, less_equal<int>());
	}
	long maxArea = 0;
	for (int i = 0; i<(int)h.size(); i++) {
		maxArea = max(maxArea, (long)h[i] * (right_lower[i] - left_lower[i] - 1));
	}
	return maxArea;
}
TEST_CASE("Hacker rank stack Largest Rectangle", "[NEW]")
{
	CHECK(largestRectangle(vector<int>{1, 3, 5, 9, 11}) == 18);
}
