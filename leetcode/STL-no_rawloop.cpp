#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

class STL
{
public:
	// 905. Sort Array By Parity, even then odd
	vector<int> sortArrayByParity(vector<int>& A) {  // beat 74%
		partition(A.begin(), A.end(), [](int n) { return n % 2 == 0; });
		return A;
	}

	// 832. Flipping an Image, slip horizontally, then invert values
	vector<vector<int>> flipAndInvertImage(vector<vector<int>>& A) { // beat 100%
		for (auto& v : A) {
			reverse(begin(v), end(v));
			transform(begin(v), end(v), begin(v), [](int i) {return 1 - i; });
		}
		return A;
	}
	// 412. Fizz Buzz
	vector<string> fizzBuzz(int n) {  // generate beat 68%, generate_n beat 100%
		vector<string> ans;
		ans.reserve(n);
		std::generate_n(back_inserter(ans), n, [n = 0]() mutable->string { 
			if (++n % 15 == 0)
				return "FizzBuzz";
			else if (n % 5 == 0)
				return "Buzz";
			else if (n % 3 == 0)
				return "Fizz";
			else
				return to_string(n);
		});
		return ans;
	}
	// 908. Smallest Range I, add x to each element, |x|<=K
	// Return the smallest possible difference between the maximum value of B and the minimum value of B
	int smallestRangeI(vector<int>& A, int K) { // beat 99%
		auto x = minmax_element(A.begin(), A.end());
		int maxDiff = *x.second - *x.first;  // diff between min and max
		maxDiff -= 2 * K; // diff can be compensated by K in either direction
		return maxDiff < 0 ? 0 : maxDiff;
	}
	// 665. Non-decreasing Array
	// check if it could become non-decreasing by modifying at most 1 element
	bool checkPossibility(vector<int>& nums) {  // beat 98%
		auto dec = adjacent_find(nums.begin(), nums.end(), greater<>());
		if (dec == nums.end()) // no decrease
			return true;
		auto dec2 = adjacent_find(dec+1, nums.end(), greater<>()); 
		if (dec2 != nums.end()) // two decrease
			return false;
		// one decrease
		if (dec == nums.begin() || dec == nums.end() - 2)  // decrease at first pair or last pair
			return true;
		// trick condition
		if (*dec > *(dec + 2) && *(dec - 1) > *(dec + 1)) //[3, 4, 2, 3], 50 60 5 49
			return false;
		return true; // [2, 3, 3, 2, 4]
	}
	void rotate(vector<int>& nums, int k) { // beat 100%
		k %= nums.size();
		if (k>0)
			std::rotate(nums.begin(), nums.end()-k, nums.end());
	}

	// 414. Third Maximum Number, O(n)
	// Given a non-empty array of integers, return the third maximum number. If it does not exist, return the maximum number. 
	int thirdMax(vector<int>& nums) { // beat 99%
		vector<int> counter(3, 0); // counter for top 3
		if (nums.size() < 3) {
			return *max_element(nums.begin(), nums.end());
		}
		nth_element(nums.begin(), nums.begin() + 2, nums.end(), greater<>());
		sort(nums.begin(), nums.begin() + 2, greater<>()); // sort first 2, could use partial_sort
		int rank = 0;
		counter[0] = 1;
		if (nums[0] > nums[1])
			rank++;
		counter[rank]++;
		if (nums[1] > nums[2])
			rank++;
		counter[rank]++;
		int No3 = 2; //
		while (rank < 2) {
			counter[rank++] = count(nums.begin(), nums.end(), nums[No3]);
			No3 = accumulate(counter.begin(), counter.begin() + rank, 0);
			if (No3 == nums.size())
				return nums[0];
			nth_element(nums.begin(), nums.begin() + No3, nums.end(), greater<>());
		}
		return nums[No3];
	}

	// 697. Degree of an Array.  maximum frequency of any one of its elements
	// nums.length will be between 1 and 50, 000.
	//nums[i] will be an integer between 0 and 49, 999.
	struct triple_int
	{
		int count=0;
		int start=0;
		int end=0;
	};
	int findShortestSubArray(vector<int>& nums) {  // beat 88%, using map
		map<int, triple_int> freq_count;
		int i = 0;
		for (int n:  nums) {
			if (freq_count[n].count++ == 0)  // first one
				freq_count[n].start = i;
			else
				freq_count[n].end = i;
			i++;
		}
		auto max_elem = max_element(begin(freq_count), end(freq_count), 
			[](const auto&v1, const auto&v2) {return v1.second.count < v2.second.count; });
		int max_cnt = max_elem->second.count;
		if (max_cnt == 1)  // special case, degree is 1
			return 1;
		int shortest = INT32_MAX;
		for (const auto& p : freq_count) {
			if (p.second.count == max_cnt)
				shortest = min(shortest, p.second.end - p.second.start);
		}
		return shortest+1;
	}
	// 888. Fair Candy Swap, swap one candy so they have same total size
	vector<int> fairCandySwap(vector<int>& A, vector<int>& B) {  // beat 95%
		int ta = accumulate(A.begin(), A.end(), 0);
		int tb = accumulate(B.begin(), B.end(), 0);
		int diff = (ta - tb) / 2;
		unordered_set<int> b_set{ begin(B),end(B) };
		for (int a : A) {
			if (b_set.count(a - diff))  // look for a-diff in B
				return{ a, a - diff };
		}
		return{ 0,0 };
	}
};

TEST_CASE("697. Degree of an Array", "[NEW]")
{
	CHECK(STL().findShortestSubArray(vector<int>{1, 2, 2, 3, 1}) == 2);
}
TEST_CASE("414. Third Maximum Number", "[NEW]")
{
	CHECK(STL().thirdMax(vector<int>{2, 2, 3, 1}) == 1);
	CHECK(STL().thirdMax(vector<int>{1, 2}) == 2);
	CHECK(STL().thirdMax(vector<int>{1, 1, 2}) == 2);
	CHECK(STL().thirdMax(vector<int>{2, 2, 3, 3, 3, 2, 1}) == 1);
	CHECK(STL().thirdMax(vector<int>{2, 2, 3, 3, 3, 2}) == 3);
	CHECK(STL().thirdMax(vector<int>{3, 3, 3}) == 3);
}