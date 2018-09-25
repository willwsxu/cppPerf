#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
using namespace std;


class Stacking {
public:
	// 739. Daily Temperatures, tells you how many days you would have to wait until a warmer temperature
	vector<int> dailyTemperatures(vector<int>& temperatures) {  // beat 99%
		return nextGreater(temperatures, false, true);   // reuse code from project 503, slower, beat 80%, slow down more after add 3rd param
		/*
		stack<int> pending;  // temp of day index if it cannot determine warm day right away, just store index
		int n = temperatures.size();
		vector<int> warm(n, 0);
		for (int i = 1; i < n; i++) {
			if (temperatures[i - 1] < temperatures[i]) { // find warmer temp
				warm[i - 1] = 1; // next day is warmer, process immediately
				while (!pending.empty()) { // examine there is days on stack are getting warm to current day
					int t = pending.top();
					if (temperatures[t] >= temperatures[i])
						break;
					warm[t] = i - t;
					pending.pop();
				}
			}
			else
				pending.push(i - 1);  // not warmer, add previous slot to stack
		}
		return warm;
		*/
	}

	unordered_map<int, int> nextGreater(vector<int>& temperatures) {  // beat 94% comparing to use dailyTemperatures
		stack<int> pending;  // temp of day index if it cannot determine warm day right away, just store index
		int n = temperatures.size();
		unordered_map<int, int> warm;
		for (int i = 1; i < n; i++) {
			int prev = temperatures[i - 1];
			if (prev < temperatures[i]) { // find warmer temp
				warm[prev] = i; // next day is warmer, process immediately
				while (!pending.empty()) { // examine there is days on stack are getting warm to current day
					int t = pending.top();
					if (temperatures[t] >= temperatures[i])
						break;
					warm[temperatures[t]] = i;
					pending.pop();
				}
			}
			else
				pending.push(i - 1);  // not warmer, add previous slot to stack
		}
		while (!pending.empty()) {
			warm[temperatures[pending.top()]] = 0;
			pending.pop();
		}
		return warm;
	}

	// 496. Next Greater Element I
	// given two arrays (without duplicates) nums1 and nums2 where nums1’s elements are subset of nums2. 
	// Find all the next greater numbers for nums1's elements in the corresponding places of nums2
	vector<int> nextGreaterElement(vector<int>& findNums, vector<int>& nums) { // beat 100%
		vector<int> nxGreater = nextGreater(nums, false);   // reuse code from project 503
		unordered_map<int, int> index;  // map value to index of next greater
		for (size_t k = 0; k < nxGreater.size(); k++)
			index[nums[k]] = nxGreater[k];
		transform(begin(findNums), end(findNums), begin(findNums), [&nums, &index](int n) { return index[n]>=0 ? nums[index[n]] : -1; });
		return findNums;
	}


	vector<int> nextGreaterElement_slow(vector<int>& findNums, vector<int>& nums) { // slower after bypass vector and use map directly, beat only 76%
		unordered_map<int, int> index = nextGreater(nums);  // map value to index of next greater
		transform(begin(findNums), end(findNums), begin(findNums), [&nums, &index](int n) { return index[n] ? nums[index[n]] : -1; });
		return findNums;
	}

	vector<int> nextGreater(vector<int>& nums, bool bCircular, bool delta=false) {  // add delta slow down program a lot
		stack<int> pending;  // store index of number which is not followed immediately by a larger number
		int n = nums.size();
		vector<int> greater(n, delta?0:-1);
		auto processStack = [&pending, &nums, &greater, delta](int i) {
			while (!pending.empty()) { // process all numbers on stack which is smaller than current number
				int t = pending.top();
				if (nums[t] >= nums[i])
					break;
				greater[t] = delta?i-t:i;
				pending.pop();
			}
		};
		for (int i = 1; i < n; i++) {
			if (nums[i - 1] < nums[i]) {   // number is larger than previous
				greater[i - 1] = delta?1:i;        // process immediately
				processStack(i);
			}
			else
				pending.push(i - 1);  // not warmer, add previous slot to stack
		}
		if (bCircular) { // circle back to beginning of the array
			pending.push(n - 1);  // add last element
			for (int i = 0; i < n - 1 && !pending.empty(); i++)
				processStack(i); // not necessary to check if stack top is more than one circle as staled numbers will be at bottom of the stack
		}
		return greater;
	}

	//503. Next Greater Element II, treat array as circular
	vector<int> nextGreaterElements(vector<int>& nums) {  // beat 92%
		vector<int> ans = nextGreater(nums, true);  // get index of next greater value
		for (size_t i = 0; i < ans.size(); i++) {
			if (ans[i] >= 0)
				ans[i] = nums[ans[i]];  // in place repace index with value
		}
		return ans;
	}

	// 556. Next Greater Element III
	// Given a positive 32-bit integer n, you need to find the smallest 32-bit integer which has exactly the same digits existing in the integer n and is greater in value than n
	int nextGreaterElement(int n) {  // beat 100%
		string s = to_string(n);
		auto piv = adjacent_find(s.rbegin(), s.rend(), greater<int>()); // find first # that is not in reverse order from back
		if (piv == s.rend())  // all digits are in reverse order
			return -1;
		auto p = piv.base() - 1;   // reverse iterator points to element that is left of its base.
		reverse(p, s.end());   // sort digits in ascending order, so number will be least
		auto next = upper_bound(p, s.end(), *(p - 1));
		iter_swap(p - 1, next);     // p-1 points to first digit that is not in revers order from back, swap it with the least number
		if (s.size() == 10 && s > "2147483647")  // check overflow
			return -1;
		return atoi(s.c_str());
	}
};

TEST_CASE("Daily Temperatures find warmer day ahead", "[GRE]")
{
	CHECK(Stacking().dailyTemperatures(vector<int>{73, 74, 75, 71, 69, 72, 76, 73}) == vector<int>{1, 1, 4, 2, 1, 1, 0, 0});
	CHECK(Stacking().dailyTemperatures(vector<int>{}) == vector<int>{});
	CHECK(Stacking().dailyTemperatures(vector<int>{73}) == vector<int>{0});
}

TEST_CASE("Next Greater Element I, II, III", "[GRE]")
{
	CHECK(Stacking().nextGreaterElement(vector<int>{4, 1, 2}, vector<int>{1, 3, 4, 2}) == vector<int>{-1, 3, -1});

	CHECK(Stacking().nextGreaterElements(vector<int>{1, 2, 1}) == vector<int>{2, -1, 2});

	CHECK(Stacking().nextGreaterElement(4365) == 4536);
	CHECK(Stacking().nextGreaterElement(230241) == 230412);
	CHECK(Stacking().nextGreaterElement(230242) == 230422);  // need to find upper bound
	CHECK(Stacking().nextGreaterElement(1999999999) == -1);
}


class CallStack {
public:
	// 636. Exclusive Time of Functions
	// wierd time convention, start time and end time are inclusive. start from 0 end 0 took 1 time interval
	tuple<int, bool, int> parse(const string&s)
	{  // string parsing is much faster than stringstream extraction, beat 99% vs 39%, 16ms vs 24 ms
		int n = s.find_first_of(':');
		int f = atoi(s.substr(0, n).c_str());
		bool start = s[n + 1] == 's';
		n += start ? 7 : 5;
		int e = atoi(s.substr(n).c_str());
		return{ f, start, e };
	}
	vector<int> exclusiveTime(int n, vector<string>& logs) {
		vector<int> time(n, 0);
		stack<int> calls;
		int prevTime = 0;  // process stack so only prev time is needed for computing
		for (const string& log : logs) {
			int func, stamp;
			bool start;
			tie(func, start, stamp) = parse(log);
			if (start) {
				if (!calls.empty()) {
					time[calls.top()] += stamp - prevTime;
				}
				prevTime = stamp;
				calls.emplace(func);
			}
			else {
				time[func] += stamp - prevTime + 1;
				prevTime = stamp + 1;
				calls.pop();
			}
		}
		return time;
	}
};

TEST_CASE("exclusive execution time", "[STACK]")
{
	CHECK(CallStack().exclusiveTime(2, vector<string>{"0:start:0", "1:start:2", "1:end:5", "0:end:6"}) == vector<int>{3, 4});
	CHECK(CallStack().exclusiveTime(1, vector<string>{"0:start:0", "0:start:2", "0:end:5", "0:start:6", "0:end:6", "0:end:7"}) == vector<int>{8});
}
class GeneralStack
{
public:
	// 735. Asteroid Collision, absolute value represents its size, and the sign represents its direction (positive meaning right, negative meaning left). Each asteroid moves at the same speed.
	//  If two asteroids meet, the smaller one will explode. If both are the same size, both will explode
	// Ideas: negative on the left are safe, store positive on stack until it meets some left traveling asteroid
	vector<int> asteroidCollision(vector<int>& asteroids) {  // messed up requirement at first, beat 99%
		if (asteroids.empty())
			return{};
		vector<int> ans;
		ans.reserve(asteroids.size() / 2);
		deque<int> rightward; // asteroids rightward
		for (int a : asteroids) {
			if (rightward.empty()) {
				if (a < 0)
					ans.push_back(a);
				else
					rightward.push_back(a);
			}
			else {
				if (a>0)
					rightward.push_back(a);
				else {
					int size2 = -a;
					while (!rightward.empty()) {
						if (rightward.back() > size2)  // new asteroid explode
							break;
						else if (rightward.back() == size2) { // both explode
							rightward.pop_back();
							break;
						}
						else {
							rightward.pop_back();  // old one explode
							if (rightward.empty()) {
								ans.push_back(a);  // no more collission
								break;
							}
						}
					}
				}
			}
		}
		if (rightward.empty())
			return ans;
		copy(begin(rightward), end(rightward), back_inserter(ans));
		return ans;
	}

	// 394. Decode String, k[encode], repeat encode k times, k is positive int
	// 3[2[a]] -> aaaaaa
	// 3a, 2[4] are invalid
	tuple<int, int> getCount(const string& s, int start) {  // start from [
		int count = 0;
		int mult = 1;
		while (--start >= 0 && isdigit(s[start])) {  // calculate from right to left
			count += (s[start] - '0')*mult;
			mult *= 10;
		}
		return{ start + 1, count };
	}
	string decodeString(string s) {
		stack<int> ob; // open brackets
		int n = s.size();
		for (int i = 0; i < n; i++) {
			switch (s[i]) {
			case '[':	ob.push(i);		break;  // these positions won't be impacted by decode from right side
			case ']':  // decode inner most []
			{
				int start = ob.top() + 1; // start of encode string
				int ins_pos, count;
				tie(ins_pos, count) = getCount(s, ob.top()); // compute count and insertion point
				ob.pop();
				int replaced = i - ins_pos + 1;  // chars to replace, #[encode]
				string encode = s.substr(start, i - start); //[encode]
				int encode_len = i - start;
				s.replace(ins_pos, replaced, encode);  // replace #[encode] with encode once
				for (int i = 0; i < count - 1; i++) {  // insert encode string count -1 times more
					ins_pos += encode_len;
					s.insert(ins_pos, encode);
				}
				int delta = encode_len*count - replaced;  // compute net chars changes to string
				i += delta;
				n += delta;
			}
			break;
			}
		}
		return s;
	}

	// 150. Evaluate Reverse Polish Notation
	int evalRPN(vector<string>& tokens) {  // simple, beat 100%
		stack<int> operands;
		for (const string & s : tokens) {
			if (s.size() == 1 && !isdigit(s[0])) {  // operator
				int n2 = operands.top();				operands.pop();
				int n1 = operands.top();				operands.pop();
				// compute and put it back on stack
				switch (s[0]) {
				case '+':	operands.push(n1 + n2);	break;
				case '-':	operands.push(n1 - n2);	break;
				case '*':	operands.push(n1 * n2);	break;
				case '/':	operands.push(n1 / n2);	break;
				}
			}
			else
				operands.push(atoi(s.c_str()));
		}
		return operands.top();
	}

	// 331. Verify Preorder Serialization of a Binary Tree
	// idea: each node must has two children
	bool isValidSerialization(string preorder) { // beat 100%, watch out for special case
		if (preorder.empty())
			return false;
		if (preorder == "#")   //special case
			return true;
		stack<int> childCount;
		bool digit = false;  // keep track of end of digit token
		for (size_t i = 0; i < preorder.size(); i++) {
			switch (preorder[i]) {
			case '#':	// leaf node
				if (childCount.empty())
					return false;
				if (++childCount.top() > 2)
					return false;
				i++;
				break;
			case ',':
				digit = false;
				break;
			default:
				if (digit)
					break;
				digit = true;
				if (!childCount.empty())
					childCount.top()++;
				childCount.push(0);
				break;
			}
			while (!childCount.empty() && childCount.top() == 2)  // backtrack to processed parents with 2 children
				childCount.pop();
			if (childCount.empty() && i < preorder.size() - 2)  // special case, tree is complete but there are  more nodes
				return false;
		}
		return childCount.empty();  //
	}
	// 682. Baseball, C: last scrore invalid, D: double last, +: add last two valid
	int calPoints(vector<string>& ops) { // beat 98%
		vector<int> scores;
		for (const string& s : ops) {
			switch (s[0]) {
			case 'C':	scores.pop_back();	break;
			case 'D':	scores.push_back(scores.back() * 2);	break;
			case '+':	scores.push_back(scores.back() + scores[scores.size() - 2]);	break;
			default:	scores.push_back(stoi(s));
			}
		}
		return accumulate(begin(scores), end(scores), 0);
	}
};

TEST_CASE("asteroidCollision", "[AST]")
{
	CHECK(GeneralStack().asteroidCollision(vector<int>{5, 10, -5, -15, 5, -5}) == vector<int>{-15});
	CHECK(GeneralStack().asteroidCollision(vector<int>{-2, -1, 1, 2}) == vector<int>{-2, -1, 1, 2});
}

TEST_CASE("decode string", "[DECODE]")
{
	CHECK(GeneralStack().decodeString("3[a]2[bc]") == "aaabcbc");
	CHECK(GeneralStack().decodeString("3[a2[c]]") == "accaccacc");
	CHECK(GeneralStack().decodeString("2[abc]3[cd]ef") == "abcabccdcdcdef");
}

TEST_CASE("preorder tree valid", "[STACK]")
{
	CHECK(GeneralStack().isValidSerialization("9,3,4,#,#,1,#,#,#,2,#,6,#,#") == false);
	CHECK(GeneralStack().isValidSerialization("#") == true);
}


class NoStack  // conceptual stack
{
public:
	// 853. Car Fleet, faster car behind cannot past slow car in front so they join as one fleet
	// idea: calculate how much time it took to get to target
	// sort by position in reverse (critical)
	// process from car closer to target, faster cars cannot pass so just discard, slower car will become a new fleet
	int carFleet(int target, vector<int>& position, vector<int>& speed) {
		if (position.empty())
			return 0;
		vector<pair<int, double>> tempo;
		for (size_t i = 0; i < position.size(); i++) {
			tempo.emplace_back(position[i], (double)(target - position[i]) / speed[i]);
		}
		sort(tempo.begin(), tempo.end(), [](const auto&a, const auto&b) { return a.first > b.first; });
		double front_tempo = tempo[0].second;
		int ans = 1;
		for (const auto&p : tempo) {
			if (front_tempo < p.second) {  // front car is faster, start a new fleet
				ans++;
				front_tempo = p.second;
			}
		}
		return ans;
	}
};


TEST_CASE("car fleet", "[STACK]")
{
	CHECK(NoStack().carFleet(10, vector<int>{0, 4, 2}, vector<int>{2, 1, 3}) == 1);
	CHECK(NoStack().carFleet(12, vector<int>{10, 8, 0, 5, 3}, vector<int>{2, 4, 1, 1, 3}) == 3);
}

class Pattern132 {
	/*int top() { return mid.top(); }    // slower than use stack as local varialbe in function, 20 ms vs 16ms
	bool empty() { return mid.empty(); }
	void pop() { mid.pop(); }
	void push(int i) { mid.push(i); }
	vector<int> *stack;
	int top_, full_;
	int top() { return (*stack)[top_]; }
	bool empty() { return top_==full_; }
	void pop() { top_++; }
	void push(int i) { (*stack)[--top_]=i; }*/

public:
	// 456. 132 Pattern, subsequence ai, aj, ak such that i < j < k and ai < ak < aj
	// n will be less than 15,000
	// borrowed idea: look for first number from right to left, 
	// add to number to stack if it is not solution, but pop out smaller numbers first so stack top is the mid number
	// update third # as max of numbers popped out of the stack
	bool find132pattern(vector<int>& nums) {  // beat 98%
		/*stack = &nums;
		top_ = nums.size();
		full_ = top_;*/

		stack<int>   mid;  // mid numbers
		int a3 = INT_MIN;  // third number in 132 pattern
		for (auto next = nums.rbegin(); next != nums.rend(); ++next) { // from right to left
			if (*next<a3 && *next<mid.top() )  // no need to check stack full after a3 becomes a valid number
				return true;
			while (!mid.empty() && *next > mid.top()) {  // pop out any smaller number on stack
				a3 = max(a3, mid.top());  // update a3 to be the greater
				mid.pop();
			}
			mid.push(*next);
		}
		return false;
	}

};

TEST_CASE("132 pattern", "[STACK]")
{
	CHECK(Pattern132().find132pattern(vector<int>{1, 0, 1, -4, -3}) == false);
	CHECK(Pattern132().find132pattern(vector<int>{3, 5, 0, 3, 4}) == true);
	CHECK(Pattern132().find132pattern(vector<int>{3, 5, 0, 2, 1}) == true);
	CHECK(Pattern132().find132pattern(vector<int>{8, 5, 7, 4, 8, 7}) == true);
	CHECK(Pattern132().find132pattern(vector<int>{24, 26, 25, 30, 20}) == true);
}

// 155. Min Stack, supports push, pop, top, and retrieving the minimum element in constant time
// idea, when there is a new min, push old min on stack
class MinStack {  // beat 98%
	vector<int>  stack;
	int min_val = INT32_MAX;
public:
	/** initialize your data structure here. */
	MinStack() {
	}

	void push(int x) {
		if (x <= min_val) {  // push old min on to stack, borrowed idea
			stack.push_back(min_val);
			min_val = x;
		}
		stack.push_back(x);
	}

	void pop() {
		int last = top();
		stack.pop_back();
		if (last == min_val) { // retrieve new min
			min_val = top();
			stack.pop_back();
		}
	}

	int top() {
		return stack.back();
	}

	int getMin() {
		return min_val;
	}
};

TEST_CASE("155. Min Stack", "[NEW]")
{
	MinStack ms;
	ms.push(0);
	ms.push(1);
	ms.push(0);
	CHECK(ms.getMin() == 0);
	ms.pop();
	CHECK(ms.getMin() == 0);
}