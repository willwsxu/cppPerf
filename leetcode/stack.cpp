#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
#include "helper.h"
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
	// 921. Minimum Add to Make Parentheses Valid
	int minAddToMakeValid(string S) {
		int count_bad = 0;
		stack<char> paren;
		for (char c : S) {
			if (c == '(')
				paren.push(c);
			else {
				if (paren.empty())
					count_bad++;
				else
					paren.pop();
			}
		}
		return count_bad + paren.size();
	}
	int minAddToMakeValid_borrowed(string S) {
		int open = 0, close=0;
		for (char c : S) {
			if (c == '(')
				open++;
			else if (open > 0)
				open--;
			else
				close++;
		}
		return open + close;
	}
	// 316. Remove Duplicate Letters, every letter appear once and only once
	// result is the smallest in lexicographical order among all possible results
	string removeDuplicateLetters(string s) {
		vector<int> count = count_letter(s, 'a'); // use counter to remove previous added letters which are larger than current
		deque<char> letters;
		for (char letter: s) {
			if (find(begin(letters), end(letters), letter) == end(letters)) {  // add to stack only if it is new
				while (!letters.empty()) {
					char back = letters.back();
					if (letter < back && count[back - 'a'] > 1) {  // maintain increast order if possible
						letters.pop_back();  // remove previously larger letter
						count[back - 'a']--;
					}
					else
						break;
				}
				letters.push_back(letter);
			}
			else
				count[letter - 'a']--;
		}
		return{ begin(letters), end(letters) };
	}
	// 32. Longest Valid Parentheses
	int longestValidParentheses(string s) {
		deque<int> _stack;
		for (size_t i = 0; i < s.size(); i++) {
			if (s[i] == '(') {
				_stack.push_back(i);
			}
			else {
				if (_stack.empty() || s[_stack.back()]==')')
					_stack.push_back(i);
				else
					_stack.pop_back();
			}
		}
		int prev_pos = -1;
		int maxParen = 0;
		for (int pos : _stack) {
			maxParen = max(maxParen, pos - prev_pos-1);
			prev_pos = pos;
		}
		return max(maxParen, (int)s.size()- prev_pos-1);
	}
};

TEST_CASE("32. Longest Valid Parentheses", "[NEW]")
{
	string paren = ")(()(()(((())(((((()()))((((()()(()()())())())()))()()()())(())()()(((()))))()((()))(((())()((()()())((())))(())))())((()())()()((()((())))))((()(((((()((()))(()()(())))((()))()))())";
	CHECK(Stacking().longestValidParentheses(paren) == 132);
	CHECK(Stacking().longestValidParentheses("(())()(()((") == 6);
	CHECK(Stacking().longestValidParentheses(")(())(()()))(") == 10);
	CHECK(Stacking().longestValidParentheses("(()()") == 4);
	CHECK(Stacking().longestValidParentheses("(()(((()") == 2);
	CHECK(Stacking().longestValidParentheses(")()(((())))(") == 10);
	CHECK(Stacking().longestValidParentheses(")()())") == 4);
	CHECK(Stacking().longestValidParentheses("()(()") == 2);
}

TEST_CASE("316. Remove Duplicate Letters", "[NEW]")
{
	CHECK(Stacking().removeDuplicateLetters("abacb") == "abc");

	CHECK(Stacking().removeDuplicateLetters("bcabc") == "abc");
	CHECK(Stacking().removeDuplicateLetters("cbacdcbc") == "acdb");
}

TEST_CASE("Daily Temperatures find warmer day ahead", "[GRE]")
{
	CHECK(Stacking().dailyTemperatures(vector<int>{73, 74, 75, 71, 69, 72, 76, 73}) == vector<int>{1, 1, 4, 2, 1, 1, 0, 0});
	CHECK(Stacking().dailyTemperatures(vector<int>{}) == vector<int>{});
	CHECK(Stacking().dailyTemperatures(vector<int>{73}) == vector<int>{0});
}

TEST_CASE("Next Greater Element I, II, III", "[NEW]")
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

	// 880. Decoded String at Index
	string decodeAtIndex(string S, int K) {
		stack<char> decode;
		long long count = 0;
		for (char c : S) {
			if (isdigit(c))
				count *= c - '0';  // multiple of all previous letter
			else
				count++;
			decode.push(c);
			if (count >= K)
				break;
		}
		auto update_count = [&count, &K](int multi) {
			count /= multi;
			K = K%count;  // adjust K to [1, count]
			if (K == 0)
				K = (int)count; 
		};
		while (count > K) {
			if (isdigit(decode.top()))
				update_count(decode.top() - '0');
			else
				count--;
			decode.pop();
		}
		while (isdigit(decode.top())) {
			update_count(decode.top() - '0');
			decode.pop();
		}
		return string(1, decode.top());
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
	// 946. Validate Stack Sequences
	// Given two sequences pushed and popped with distinct values, return true if and only if 
	// this could have been the result of a sequence of push and pop operations on an initially empty stack.
	bool validateStackSequences(vector<int>& pushed, vector<int>& popped) {
		if (pushed.size() != popped.size())
			return false;
		stack<int> stk;
		auto pushed_cur = begin(pushed);
		auto popped_cur = begin(popped);
		auto popping = [&]() {
			while (!stk.empty() && popped_cur != end(popped)) {  // pop from stack if it matches popped sequence
				if (stk.top() == *popped_cur) {
					stk.pop();
					++popped_cur;
				}
				else
					break;
			}
		};
		while (pushed_cur != end(pushed)) {
			popping();
			while (*pushed_cur != *popped_cur) {  // keep push to stack until it match popped
				stk.push(*pushed_cur);
				if (++pushed_cur == end(pushed))
					return false;
			}
			++pushed_cur;
			++popped_cur;
		}
		popping();
		return stk.empty();
	}
};

TEST_CASE("880. Decoded String at Index", "[STACK]")
{
	CHECK(GeneralStack().decodeAtIndex("ha22", 5) == "h");
	CHECK(GeneralStack().decodeAtIndex("leet2code3", 10) == "o");
	CHECK(GeneralStack().decodeAtIndex("a2345678999999999999999", 1) == "a");
	CHECK(GeneralStack().decodeAtIndex("a2345678999999999999999", 1000000) == "a");
}
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

class MonotoneStack
{
public:
	template <typename T, typename Comp>
	vector<T> computePrevQueue(vector<T>& arr, Comp comp) {
		vector<T> prev_pos(arr.size(), -1); //find  elem > current, on the left side, from left
		deque<T> plp{ 0 };  // monotonous stack
		for (size_t i = 1; i < arr.size(); i++) {
			if (plp.empty() || comp(arr[i], arr[plp.back()]))
				plp.push_back(i);
			else {
				auto greater = upper_bound(begin(plp), end(plp), i, [&arr](int i, int j) { return arr[i] < arr[j]; });
				if (greater != end(plp))
					prev_pos[i] = *greater;
			}
		}
		return prev_pos;
	}
	vector<int> nextGreaterElementsII(vector<int>& nums) {  // implement with monotone stack, no circular, beat 96%
		vector<int> next_greater_pos = computeNextLess(nums, greater_equal<int>());// from R to L, maintain decreasing stack
																				   // circular array means left side of current, scan from left to right
		vector<int> prev_greater_pos = computePrevQueue(nums, greater<int>());
		vector<int> ans(nums.size(), -1);
		for (size_t i = 0; i < nums.size(); i++) {
			if (next_greater_pos[i] != nums.size()) {
				ans[i] = nums[next_greater_pos[i]];
			}
			else if (prev_greater_pos[i] != -1)
				ans[i] = nums[prev_greater_pos[i]];
		}
		return ans;
	}
	// 907. Sum of Subarray Minimums, sum of min of all sub array, n [1,30000]
	int sumSubarrayMins(vector<int>& A) {
		vector<int> prev_less_pos = computePrevLess(A, less<int>());// from L to R, maintain increasing stack, allow equal
		vector<int> next_less_pos = computeNextLess(A, less_equal<int>());// from R to L, maintain increasing stack
		int MOD = 1000000007;
		long long sum = 0;
		for (size_t i = 0; i < A.size(); i++) {
			// treat i as local min, find left and right boundary
			sum += (i - prev_less_pos[i])*(next_less_pos[i] - i)*A[i];
			sum %= MOD;
		}
		return (int)sum;
	}
	template <typename T, typename Comp>
	vector<T> computePrevLess(vector<T>& arr, Comp comp) {
		vector<T> prev_less_pos(arr.size(), -1); //find  elem < current, on the left side
		deque<T> plp{ 0 };  // monotonous stack
		for (size_t i = 1; i < arr.size(); i++) {
			while (!plp.empty() && comp(arr[i], arr[plp.back()]))  // don't cross equal value
				plp.pop_back();
			if (!plp.empty())
				prev_less_pos[i] = plp.back();
			plp.push_back(i);
		}
		return prev_less_pos;
	}
	template <typename T, typename Comp>
	vector<T> computeNextLess(vector<T>& arr, Comp comp) {
		deque<T> plp{};  // monotonous stack
		vector<T> next_less_pos(arr.size(), arr.size()); // find elem < current, on the right side
		for (int i = (int)arr.size() - 1; i >= 0; i--) {  // i must be signed
			while (!plp.empty() && comp(arr[i], arr[plp.back()]))  // cross equal value
				plp.pop_back();
			if (!plp.empty())
				next_less_pos[i] = plp.back();
			plp.push_back(i);
		}
		return next_less_pos;
	}
	// 84. Largest Rectangle in Histogram, similar to #907
	// at each bar, find the left and rigth edge which are shorter than current
	int largestRectangleArea(vector<int>& heights) {
		vector<int> prev_less_pos = computePrevLess(heights, less_equal<int>());// from L to R, maintain increasing stack
		vector<int> next_less_pos = computeNextLess(heights, less_equal<int>());// from R to L, maintain increasing stack
		int max_area = 0;
		for (size_t i = 0; i < heights.size(); i++) { // height of current bar will be height of the rentangle
			max_area = max(max_area, heights[i] * (next_less_pos[i] - prev_less_pos[i] - 1));
		}
		return max_area;
	}
	// 42. Trapping Rain Water
	int trap(vector<int>& height) {
		// scan from R to L, use stack to store elevaion in decreasing order
		deque<int> elevation;  // keep same order as vector height
		int area = 0;
		for (int i = height.size() - 1; i >= 0; i--) {
			if (elevation.empty())
				elevation.push_back(i);
			else if (height[i] >= height[elevation.back()]) {  // compute water trapped between current and the highest
				area += (elevation.back() - i - 1)*height[elevation.back()];
				area -= accumulate(begin(height) + i + 1, begin(height) + elevation.back(), 0); // subtract area occupied by elevation in between
				elevation.clear();  // sub system to store water is done because Left side is higher
				elevation.push_back(i);
			}
			else {
				while (height[i] >= height[elevation.front()]) // maintain increasing order from left to right
					elevation.pop_front();
				elevation.push_front(i);
			}
		}
		// check each reservoir in increasing order
		if (elevation.size() > 1) {
			int left = elevation.front();
			for (int right : elevation) {
				if (right - left > 1) {
					int A = (right - left - 1)*height[left];
					int noneWater = accumulate(begin(height) + left + 1, begin(height) + right, 0);
					if (A > noneWater)
						area += (A - noneWater);
				}
				left = right;
			}
		}
		return area;
	}

	// 85. Maximal Rectangle, matrix of '0' and '1'
	// find max rectangle of '1's
	int maximalRectangle(vector<vector<char>>& matrix) {
		if (matrix.empty() || matrix[0].empty())
			return 0;
		int rows = matrix.size();
		int cols = matrix[0].size();
		vector<vector<int>> heights(rows, vector<int>(cols, 0));
		// count cumulative '1' from top to bottom, start over if there is '0'
		for (int j = 0; j < cols; j++) {
			int h = 0;
			for (int i = 0; i < rows; i++) {
				if (matrix[i][j] == '1')
					h++;
				else
					h = 0;
				heights[i][j] = h;
			}
		}
		int max_area = 0;
		// each row is the base of a histogram, like problem #84
		for (int r = 0; r < rows; r++) {
			vector<int> prev_less_pos = computePrevLess(heights[r], less_equal<int>());// from L to R, maintain increasing stack
			vector<int> next_less_pos = computeNextLess(heights[r], less_equal<int>());// from R to L, maintain increasing stack
			for (int c = 0; c < cols; c++)  // height of current bar will be height of the rentangle
				max_area = max(max_area, heights[r][c] * (next_less_pos[c] - prev_less_pos[c] - 1));
		}
		return max_area;
	}
	// 962. Maximum Width Ramp, find max j-i with A[j]>=A[i]
	int maxWidthRamp_NlogN(vector<int>& A) {
		deque<int> decreasing{ 0 };
		int max_ramp = 0;
		for (int i = 1; i < (int)A.size(); i++) {
			if (A[i] < A[decreasing.back()]) {
				decreasing.push_back(i);
			}
			else { // find the previous position > A[i]
				auto found = upper_bound(rbegin(decreasing), rend(decreasing), A[i], [&A](int target, int idx) { return target < A[idx]; });
				if (found == rend(decreasing))
					max_ramp = max(max_ramp, i);  // tricky here
				else {
					max_ramp = max(max_ramp, i - *--found); // tricky again
				}
			}
		}
		return max_ramp;
	}
	int maxWidthRamp(vector<int>& A) { // O(n) fast idea: generating decreasing stack from L to R, then scan from R to L
		deque<int> decreasing{ 0 };
		int max_ramp = 0;
		for (int i = 1; i < (int)A.size(); i++) {
			if (A[i] < A[decreasing.back()])
				decreasing.push_back(i);
		}
		if (decreasing.size() == A.size())  // A is in decreasing order
			return 0;
		// second phase, scan from R to L to find the first that is not smaller than stack top
		int fromR = A.size()-1;  // initialize to invalid
		while (!decreasing.empty()) {
			while (A[fromR] < A[decreasing.back()])
				fromR--;
			max_ramp = max(max_ramp, fromR - decreasing.back());
			decreasing.pop_back();
		}
		return max_ramp;
	}
};
TEST_CASE("962. Maximum Width Ramp", "[NEW]")
{
	CHECK(MonotoneStack().maxWidthRamp(vector<int>{3,4,1,2}) == 1);
	CHECK(MonotoneStack().maxWidthRamp(vector<int>{6, 0, 8, 2, 1, 5}) == 4);
	CHECK(MonotoneStack().maxWidthRamp(vector<int>{9, 8, 1, 0, 1, 9, 4, 0, 4, 1}) == 7);
}
TEST_CASE("84. Largest Rectangle in Histogram", "[NEW]")
{
	CHECK(MonotoneStack().largestRectangleArea(vector<int>{0, 9}) == 9);
	CHECK(MonotoneStack().largestRectangleArea(vector<int>{2, 1, 5, 6, 2, 3}) == 10);
}
TEST_CASE("907. Sum of Subarray Minimums", "[NEW]")
{
	CHECK(MonotoneStack().sumSubarrayMins(vector<int>{71, 55, 82, 55}) == 593);
	CHECK(MonotoneStack().sumSubarrayMins(vector<int>{3, 1, 2, 4}) == 17);
}
TEST_CASE("Next Greater Element II", "[NEW]")
{
	CHECK(MonotoneStack().nextGreaterElementsII(vector<int>{1, 2, 1}) == vector<int>{2, -1, 2});
	CHECK(MonotoneStack().nextGreaterElementsII(vector<int>{5, 4, 3, 2, 1}) == vector<int>{-1, 5, 5, 5, 5});
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

// 232. Implement Queue using Stacks
class MyQueue {  // beat 100%
	stack<int>	stack_push; // for push ops
	stack<int>	stack_pop;	// for pop ops
	void transfer_stack()
	{
		if (stack_pop.empty()) {
			while (!stack_push.empty()) { // move push stack to pop stack, reverse order
				stack_pop.push(stack_push.top());
				stack_push.pop();
			}
		}
	}
public:
	/** Initialize your data structure here. */
	MyQueue() {	}

	/** Push element x to the back of queue. */
	void push(int x) {
		stack_push.push(x);
	}

	/** Removes the element from in front of queue and returns that element. */
	int pop() {
		transfer_stack();
		int x = stack_pop.top();
		stack_pop.pop();
		return x;
	}

	/** Get the front element. */
	int peek() {
		transfer_stack();
		return stack_pop.top();
	}

	/** Returns whether the queue is empty. */
	bool empty() {
		return stack_pop.empty()&& stack_push.empty();
	}
};

// 225. Implement Stack using Queues
class MyStack {  // beat 100%
	queue<int>	q;  // push back, pop front
public:
	/** Initialize your data structure here. */
	MyStack() {	}

	/** Push element x onto stack. */
	void push(int x) {  // push is slow O(n), so pop and top are fast O(1)
		q.push(x);
		int n = q.size() - 1;
		while (n--) {  // move last to front
			q.push(q.front());
			q.pop();
		}
	}

	/** Removes the element on top of the stack and returns that element. */
	int pop() {
		int x = q.front();
		q.pop();
		return x;
	}

	/** Get the top element. */
	int top() {
		return q.front();
	}

	/** Returns whether the stack is empty. */
	bool empty() {
		return q.empty();
	}
};

// similar to 907. Sum of Subarray Minimums, 239. Sliding Window Maximum
// max # of consecutive days that none decreasing price till today
class StockSpanner {
	vector<pair<int, int>> prices_stack;  // decreasing, with count of deleted smaller #
public:
	StockSpanner() {}

	int next(int price) {
		int count = 1;// itself
		while (!prices_stack.empty() && prices_stack.back().first <= price) {
			count += prices_stack.back().second;
			prices_stack.pop_back();
		}
		prices_stack.emplace_back(price, count);
		return count;
	}
};
TEST_CASE("901. Online Stock Span", "[NEW]")
{
	StockSpanner span;
	CHECK(span.next(100) == 1);
	CHECK(span.next(80) == 1);
	CHECK(span.next(60) == 1);
	CHECK(span.next(70) == 2);
	CHECK(span.next(60) == 1);
	CHECK(span.next(75) == 4);
	CHECK(span.next(85) == 6);
	CHECK(span.next(85) == 7);
}
// 895. Maximum Frequency Stack
class FreqStack {
	unordered_map<int, int> freq; // overall frequency of each int
	map<int, stack<int>, greater<int>> freq_stack; // stack per frequency
public:
	FreqStack() {}

	void push(int x) {
		freq_stack[++freq[x]].push(x);
	}

	int pop() {
		int x = freq_stack.begin()->second.top();
		freq_stack.begin()->second.pop();
		freq[x]--;
		if (freq_stack.begin()->second.empty())
			freq_stack.erase(freq_stack.begin());
		return x;
	}
};