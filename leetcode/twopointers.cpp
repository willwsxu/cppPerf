#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
using namespace std;


class TwoPointers {
public:
	//567  return true if s2 contains the permutation of s1
	bool checkInclusion(string s1, string s2) {  // beat 99%
		int len1 = s1.length();
		int len2 = s2.length();
		if (len1 > len2)
			return false;
		int count[128] = { 0 };
		auto same = [&count]() {
			for (int i = 'a'; i <= 'z'; i++) {
				if (count[i])
					return false;
			}
			return true;
		};
		for (int i = 0; i < len1; i++) {
			count[s1[i]]++;   // add letter count from s1
			count[s2[i]]--;   // subtract letter count from s2
		}
		if (same())  // same when count is 0 for all letters
			return true;
		for (int i = len1; i < len2; i++) {  // sliding window of len1
			count[s2[i]]--;         // subtract next char
			count[s2[i - len1]]++;  // add back first
			if (same())
				return true;
		}
		return false;
	}

	// 713 number of (contiguous) subarrays where the product of all the elements in the subarray is less than k
	int numSubarrayProductLessThanK(vector<int>& nums, int k) {  // beat 62%
		int count = 0;
		int prod = 1;   // compute product between first and second pointer
		for (int second = 0, first = 0; second < (int)nums.size(); second++) {
			prod *= nums[second];     // multiply new val
			while (prod >= k && first <= second) { // divide from beginning of subarray, update pointer
				prod /= nums[first++];
			}
			if (first <= second)   // subarray is not empty
				count += (second - first + 1);  // add all subarrays from second pointer to first pointer
		}
		return count;
	}
	// 826. Most Profit Assigning Work, first 2 vectors are for job's difficulty and profit
	// 3rd vector is worker's ability, can only do job of same of less difficulty
	int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
		auto p = begin(profit);
		using pii = pair<int, int>;
		vector<pii> zip;
		zip.reserve(profit.size());
		for (auto d = begin(difficulty); d != end(difficulty) && p != end(profit); d++, p++) {
			zip.emplace_back(*d, *p);
		}
		// assume all vectors are not sorted
		sort(begin(zip), end(zip), [](auto&a, auto&b) { return a.first < b.first; });  // sort by difficult
		sort(begin(worker), end(worker));
		// work with higher ability should earn more, thus we only need to go through zip once;
		int maxP = 0;
		auto z = begin(zip);
		auto Max = [&z, &zip, &maxP](int acc, int a) {
			for (; z != end(zip); z++) {
				if (z->first > a)  // job too difficult for worker a
					break;
				maxP = max(maxP, z->second);
			}  // found max profit up to difficulty a
			return maxP + acc;
		};
		return accumulate(begin(worker), end(worker), 0, Max); // faster after using accumulate, 89ms vs 114ms
	}

	template<class RanIt>
	pair<RanIt, RanIt> mountain_range(RanIt first, RanIt last)
	{
		first = adjacent_find(first, last, less<int>());
		if (first != last) {
			auto top = first + 1;
			auto down = top;
			while (++down != last) {
				if (*down == *top)
					return mountain_range(down, last);
				else if (*down < *top)
					break;
				else
					++top;
			}
			if (down != last) {
				auto next = down;
				while (++next != last) {
					if (*next >= *down)
						break;
					else
						++down;
				}
				return{ first, down };
			}
		}
		return{ last,last };
	}
	// 845. Longest Mountain in Array, strictly up or down
	int longestMountain(vector<int>& A) {
		int longest = 0;
		auto res = mountain_range(begin(A), end(A));
		while (res.first != res.second) {
			longest = max(longest, (int)distance(res.first, res.second) + 1);
			res = mountain_range(res.second, end(A));
		}
		return longest;
	}
	// 923. 3Sum With Multiplicity, output total possible A[i]+A[j]+A[k]=target, modulo 1e9+7
	// 3 <= A.length <= 3000, 0 <= A[i] <= 100
	int threeSumMulti(vector<int>& A, int target) {  // math counting
		static const int MOD = 1000000007;
		vector<int> count(101, 0);
		for (int a : A)
			count[a]++;
		int size = A.size();
		int ans = 0;
		for (int Ai = 0; Ai < 101; Ai++) {
			if (count[Ai] == 0)
				continue;
			for (int Aj = 0; Aj < 101; Aj++) {
				if (count[Aj] == 0)
					continue;
				int Ak = target - Ai - Aj;
				if (Ak<0 || Ak>100 || !count[Ak]) // A[k] does not exist
					continue;
				if (Ai == Aj && Aj == Ak) // pick 3 out of A[i]
				{
					if (count[Ai] < 3)
						continue;
					int64_t choose3 = count[Ai] * (count[Ai] - 1);
					ans += static_cast<int>(choose3*(count[Ai] - 2) / 6 % MOD);
				}
				else if (Ai == Aj && Aj != Ak) {  // 2 2 2 3 target 7
					if (count[Ai] < 2)
						continue;
					int64_t choose2 = count[Ai] * (count[Ai] - 1) / 2;
					ans += static_cast<int>(choose2*count[Ak] % MOD);
				}
				else if (Ai < Aj && Aj < Ak) {
					ans += static_cast<int>((int64_t)count[Ai] * count[Aj] * count[Ak] % MOD);
				}
				ans %= MOD;
			}
		}
		return ans;
	}
};

TEST_CASE("923. 3Sum With Multiplicity", "[NEW]")
{
	CHECK(TwoPointers().threeSumMulti(vector<int>{16, 51, 36, 29, 84, 80, 46, 97, 84, 16}, 171) == 2);
	CHECK(TwoPointers().threeSumMulti(vector<int>{1, 1, 2, 2, 3, 3, 4, 4, 5, 5}, 8) == 20);
}
class TwoPointerEasy
{
public:
	// 283. Move Zeroes
	void moveZeroes(vector<int>& nums) { // beat 50%
		auto zero = find(nums.begin(), nums.end(), 0);
		if (zero == nums.end())
			return;
		auto nonezero = find_if(zero + 1, nums.end(), [](int i) { return i != 0; });
		while (zero != nums.end() && nonezero != nums.end()) {
			iter_swap(zero, nonezero);
			zero = find(zero + 1, nums.end(), 0);
			nonezero = find_if(nonezero + 1, nums.end(), [](int i) { return i != 0; });
		}
	}
	// 27. Remove Element
	int removeElement(vector<int>& nums, int val) {  // beat 98%, copy all good number from pos=0, seems fater than method in #283
		int i = 0;
		for (int n : nums) {
			if (n != val)
				nums[i++] = n;
		}
		return i;
	}
	// 844. Backspace String Compare, # means to erase previous char
	bool backspaceCompare(string S, string T) {  // beat 100%, tricky cases
		auto s = S.rbegin();
		auto t = T.rbegin();
		auto back_remove = [](auto x, auto end) {
			int del = 0;
			while (x != end && *x == '#') {
				auto start = x;
				while (++x != end && *x == '#') {}  // count # till end
				del += distance(start, x);  // for case like ab#c##
				while (x != end && *x != '#' && del-- > 0)  // stop when it end or seeing another backspace
					++x;
			}
			return x;
		};
		while (s != S.rend() && t != T.rend()) {  // compare char backwards
			if (*s == '#' || *t == '#') {
				s = back_remove(s, S.rend());
				t = back_remove(t, T.rend());
				continue;
			}
			if (*s++ != *t++)
				return false;
		}
		if (s != S.rend())   // don't forget to remove backspace after one string is done
			s = back_remove(s, S.rend());
		if (t != T.rend())
			t = back_remove(t, T.rend());
		return s == S.rend() && t == T.rend();
	}
	// 88. Merge Sorted Array, assume first array has enough room to store second
	void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {  // beat 99%
		int pos = m + n - 1;  // merge in reverse order
		while (n > 0 && m>0) {
			if (nums2[n - 1] >= nums1[m - 1])
				nums1[pos--] = nums2[--n];
			else
				nums1[pos--] = nums1[--m];
		}
		while (n>0)  // edge cases
			nums1[pos--] = nums2[--n];
	}
	// 350. Intersection of Two Arrays II
	vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {  // beat 100%
		sort(begin(nums1), end(nums1));
		sort(begin(nums2), end(nums2));
		vector<int> ans;
		set_intersection(begin(nums1), end(nums1), begin(nums2), end(nums2), back_inserter(ans));
		return ans;
	}
	// 532. K-diff Pairs in an Array, unique pairs of with value diff=k
	int findPairs(vector<int>& nums, int k) { // beat 77%
		if (nums.empty())
			return 0;
		sort(begin(nums), end(nums));
		int last = nums[0];
		auto start = nums.begin();
		auto found = lower_bound(start + 1, nums.end(), last + k);  // find does not work
		int count = 0;
		while (found != nums.end()) {
			if (*found == last + k)  // re-check if found is correct
				count++;
			while (++start != nums.end() && *start == last) {
				if (start == found)  // ensure start does not pass over found
					++found;
			}  // skip to next unique int
			if (start == nums.end())
				break;
			if (start == found)  // ensure start does not pass over found
				++found;
			last = *start;
			found = lower_bound(found, nums.end(), last + k);  // be careful what search start
		}
		return count;
	}
	// 1. Two Sum
	vector<int> twoSum(vector<int>& nums, int target) { // beat 99%
		vector<pair<int, int>> val_pos;
		val_pos.reserve(nums.size());
		for (size_t i = 0; i < nums.size(); i++)
			val_pos.emplace_back(nums[i], i);
		sort(val_pos.begin(), val_pos.end(), [](const auto&p1, const auto&p2) { return p1.first < p2.first; });
		auto start = begin(val_pos);
		auto last = end(val_pos) - 1;
		while (start != last) {
			int sum = start->first + last->first;
			if (sum == target)
				return{ start->second, last->second };
			else if (sum < target)
				++start;
			else
				--last;
		}
		return{ 0,0 }; // silence compiler, not reachable
	}
	// 922. Sort Array By Parity II, value parity match index parity
	vector<int> sortArrayByParityII(vector<int>& A) {
		size_t even = 0, odd = 1;
		while (even<A.size() && odd<A.size()) {
			if (A[even] % 2 != 0 && A[odd] % 2 == 0) {  // even on odd index, odd on even index, swap
				swap(A[even], A[odd]);
				even += 2;
				odd += 2;
				continue;
			}
			if (A[even] % 2 == 0)  // even index is good, move to next
				even += 2;
			if (A[odd] % 2 != 0)	// odd index is good, move to next
				odd += 2;
		}
		return A;
	}
};
TEST_CASE("532. K-diff Pairs in an Array", "[NEW]")
{
	CHECK(TwoPointerEasy().findPairs(vector<int>{6, 7, 3, 6, 4, 6, 3, 5, 6, 9}, 4) == 2);
	CHECK(TwoPointerEasy().findPairs(vector<int>{6, 2, 9, 3, 9, 6, 7, 7, 6, 4}, 3) == 3);
	CHECK(TwoPointerEasy().findPairs(vector<int>{1, 3, 1, 5, 4}, 0) == 1);
}
TEST_CASE("88. Merge Sorted Array", "[NEW]")
{
	vector<int> A{ 0, 0, 3, 0, 0, 0, 0, 0, 0,0 };
	vector<int> B{ -1,-1,1,1,1,2,3 };
	TwoPointerEasy().merge(A, 3, B, 7);
	CHECK(A == vector<int>{-1, -1, 0, 0, 1, 1, 1, 2, 3, 3});
}
TEST_CASE("844. Backspace String Compare", "[NEW]")
{
	CHECK(TwoPointerEasy().backspaceCompare("bxj##tw", "bxo#j##tw") == true);  // trick case
	CHECK(TwoPointerEasy().backspaceCompare("a#c", "c") == true);  // edge case
	CHECK(TwoPointerEasy().backspaceCompare("a##c", "#a#c") == true);
	CHECK(TwoPointerEasy().backspaceCompare("gtc#uz#", "gtcm##uz#") == true);
	CHECK(TwoPointerEasy().backspaceCompare("a#c", "b") == false);
	CHECK(TwoPointerEasy().backspaceCompare("ab##", "c#d#") == true);
	CHECK(TwoPointerEasy().backspaceCompare("ab#c", "ad#c") == true);
}
