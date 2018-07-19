#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
using namespace std;


class Stacking {
public:
	// 739. Daily Temperatures, tells you how many days you would have to wait until a warmer temperature
	vector<int> dailyTemperatures(vector<int>& temperatures) {  // beat 99%
		vector<int> nxGreater = nextGreater(temperatures, false);   // reuse code from project 503, slower, beat 80%
		for (int i = 0; i < nxGreater.size(); i++) {	
			if (nxGreater[i] < 0)
				nxGreater[i] = 0;
			else
				nxGreater[i] -= i;
		}
		return nxGreater;
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
		return warm;*/
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

	vector<int> nextGreater(vector<int>& nums, bool bCircular) {
		stack<int> pending;  // store index of number which is not followed immediately by a larger number
		int n = nums.size();
		vector<int> greater(n, -1);
		auto processStack = [&pending, &nums, &greater](int i) {
			while (!pending.empty()) { // process all numbers on stack which is smaller than current number
				int t = pending.top();
				if (nums[t] >= nums[i])
					break;
				greater[t] = i;
				pending.pop();
			}
		};
		for (int i = 1; i < n; i++) {
			if (nums[i - 1] < nums[i]) {   // number is larger than previous
				greater[i - 1] = i;        // process immediately
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

TEST_CASE("Daily Temperatures find warmer day ahead", "[NEW]")
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
