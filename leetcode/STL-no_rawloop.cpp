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

	// 896. Monotonic Array, An array A is monotone increasing if for all i <= j, A[i] <= A[j]
	// An array A is monotone decreasing if for all i <= j, A[i] >= A[j]
	bool isMonotonic(vector<int>& A) {  // beat 98% on 5th submit
		if (A.size() < 3)
			return true;
		auto notEq = adjacent_find(A.begin(), A.end(), not_equal_to<>()); // find first not equal
		if (notEq == A.end())  // all same
			return true;
		auto start = notEq + 1;  // staring point for next find
		if (*notEq < *start)
			return adjacent_find(start, A.end(), greater<>()) == A.end();
		return adjacent_find(start, A.end(), less<>()) == A.end();
	}
	// 506. Relative Ranks
	vector<string> findRelativeRanks(vector<int>& nums) {  // beat 99%
		vector<pair<int, int>> score_pos; // keep track of position of score
		score_pos.reserve(nums.size());
		for (size_t i = 0; i < nums.size(); i++) {
			score_pos.emplace_back(i, nums[i]);
		}
		sort(score_pos.begin(), score_pos.end(), [](const auto&p1, const auto&p2) { return p1.second > p2.second; }); // sort scorein reverse
		vector<string> ans(nums.size());
		const char*top3[] = { "Gold Medal", "Silver Medal", "Bronze Medal" };
		for (int i = 0; i < min(3, (int)score_pos.size()); i++)
			ans[score_pos[i].first] = top3[i];
		for (size_t i = 3; i < score_pos.size(); i++)
			ans[score_pos[i].first] = to_string(i + 1);
		return ans;
	}
	// 724. Find Pivot Index, sum of left =sum of right, 
	int pivotIndex(vector<int>& nums) {  // beat 97%
		if (nums.empty())  // special case
			return -1;
		partial_sum(nums.begin(), nums.end(), nums.begin());
		int last = nums.size() - 1;
		if (nums[last] == nums[0])  // special case
			return 0;
		for (int i = 1; i < last; i++)  // iterate from second to second to last
			if (nums[i - 1] == nums[last] - nums[i])
				return i;
		if (nums[last - 1] == 0)
			return last;
		return -1;
	}
	// 643. Maximum Average Subarray I, over k elements
	double findMaxAverage(vector<int>& nums, int k) {  // beat 66%
		if (k == 1)
			return *max_element(nums.begin(), nums.end());
		int k_sum = accumulate(nums.begin(), nums.begin() + k - 1, 0); // sum of k-1 
		double ans = INT32_MIN;
		for (size_t i = k - 1; i < nums.size(); i++) {
			k_sum += nums[i];
			ans = max(ans, (double)k_sum / k);
			k_sum -= nums[i + 1 - k];  // shift sum to right
		}
		return ans;
	}
	// 747. Largest Number At Least Twice of Others
	int dominantIndex(vector<int>& nums) {  // beat 98%
		auto max_elem = max_element(nums.begin(), nums.end());
		*max_elem /= 2;
		if (all_of(nums.begin(), nums.end(), [max_num = *max_elem](int n) {return n <= max_num; }))
			return distance(nums.begin(), max_elem);
		return -1;
	}
	// 581. Shortest Unsorted Continuous Subarray
	int findUnsortedSubarray(vector<int>& nums) {  // beat 76%
		auto sorted = adjacent_find(nums.begin(), nums.end(), [](int n1, int n2) {return n2 < n1; });
		if (sorted == nums.end())
			return 0;
		// sorted is the end of sorted at front, exclusive
		auto min_elem = min_element(++sorted, nums.end());// if there is any number less than values in the sorted, adjust
		sorted = lower_bound(nums.begin(), sorted, *min_elem);
		if (*sorted == *min_elem) {  // adjust to one pass last valid item
			++sorted;
		}
		auto sorted_back = adjacent_find(nums.rbegin(), nums.rend(), [](int n1, int n2) {return n2 > n1; }).base(); // sorted_back is the first of sorted at back
		auto max_elem = max_element(sorted, sorted_back); // if there is any number less than values in the sorted_back, adjust
		sorted_back = lower_bound(sorted_back, nums.end(), *max_elem);
		return distance(sorted, sorted_back);
	}
};
TEST_CASE("581. Shortest Unsorted Continuous Subarray", "[NEW]")
{
	CHECK(STL().findUnsortedSubarray(vector<int>{1, 3, 2, 3, 3}) == 2);
	CHECK(STL().findUnsortedSubarray(vector<int>{2, 6, 2, 1, 16, 9, 15}) == 7);
	CHECK(STL().findUnsortedSubarray(vector<int>{2, 6, 2, 1, 10, 9, 15}) == 6);
	CHECK(STL().findUnsortedSubarray(vector<int>{2, 6, 4, 8, 10, 9, 15}) == 5);
}
TEST_CASE("724. Find Pivot Index", "[NEW]")
{
	CHECK(STL().pivotIndex(vector<int>{}) == -1);
	CHECK(STL().pivotIndex(vector<int>{-1, -1, 0, 1, 1, -1}) == 0);
	CHECK(STL().pivotIndex(vector<int>{-1, -1, 0, 1, 1, 2}) == 5);
	CHECK(STL().pivotIndex(vector<int>{-1, -1, 0, 1, 1, 1}) == 5);
}
TEST_CASE("506. Relative Ranks", "[NEW]")
{
	CHECK(STL().findRelativeRanks(vector<int>{2, 3}) == vector<string>{"Silver Medal", "Gold Medal"});
	CHECK(STL().findRelativeRanks(vector<int>{5, 4, 3, 2, 1}) == vector<string>{"Gold Medal", "Silver Medal", "Bronze Medal", "4", "5"});
}
TEST_CASE("896. Monotonic Array", "[NEW]")
{
	CHECK(STL().isMonotonic(vector<int>{1, 2, 2, 4}) == true);

}

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