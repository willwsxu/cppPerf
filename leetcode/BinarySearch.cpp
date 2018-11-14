#include "stdafx.h"
#include <cassert>
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"

using namespace std;


class BinarySearch {
public:
	// 540. Given a sorted array consisting of only integers where every element appears twice except for one
	int binary_search(const vector<int>& nums, int low, int high) // beat 98%
	{
		if (low == high)
			return nums[low];
		int mid = (low + high) / 2; // at least 3 elem
		if (nums[mid] == nums[mid - 1]) {
			if ((high - mid) % 2 == 1)
				return binary_search(nums, mid + 1, high); // 1 1 2
			else
				return binary_search(nums, low, mid - 2);    // 1 2 2 3 3
		}
		else if (nums[mid] == nums[mid + 1]) {
			if ((mid - low) % 2 == 1)
				return binary_search(nums, low, mid - 1);    //1 2 2
			else
				return binary_search(nums, mid + 2, high); //1 1 2 2 3

		}
		else
			return nums[mid]; // 1 1 2 3 3
	}
	int singleNonDuplicate(vector<int>& nums) {
		assert(nums.size() % 2 == 1);// odd size
		return binary_search(nums, 0, nums.size() - 1);
	}

	int mySqrt_bs(int x, int lo, int hi) {
		long long mid = ((long long)lo + hi) / 2;
		long long sq = mid*mid;
		if (sq == x)
			return static_cast<int>(mid);
		if (sq > x)
			return mySqrt_bs(x, lo, static_cast<int>(mid) - 1);
		if (mid == lo)  // catch special case to avoid inifite loop
		{			
			return hi*hi>x?lo:hi;
		}
		return mySqrt_bs(x, static_cast<int>(mid), hi);
	}
	// 69. Sqrt(x), x>=0
	int mySqrt(int x) { // beat 50%, watch overflow
		if (x <= 1)  // iterative method, same speed
			return x;
		int L=0, R=x;
		while (L <= R) {
			int mid = L + (R - L) / 2;
			if (mid > x / mid)
				R = mid - 1;
			else {
				if (mid + 1 > x / (mid + 1))  // try mid+1
					return mid;
				L = mid + 1;
			}
		}
		return L;
	}
	// 441. Arranging Coins, in rows, return last row that is complete
	// *
	// **
	// ***
	int arrangeCoins(int n) { // similar to #69, beat 68%
		if (n < 2)
			return n;
		int lo = 1, hi = n;
		while (lo <= hi) {
			int mid = lo + (hi - lo) / 2;
			long long total = (long long)(mid + 1)*mid / 2;
			if (total > n)
				hi = mid - 1;
			else {
				if ((long long)(mid + 2)*(mid+1) / 2>n)
					return mid;
				lo = mid + 1;
			}
		}
		return lo;
	}

	int peakIndexInMountainArray(vector<int>& A, int lo, int hi) {
		int mid = (lo + hi) / 2;
		if (mid == lo)
			return hi;
		if (A[mid] > A[mid - 1] && A[mid] > A[mid + 1])
			return mid;
		if (A[mid] > A[mid - 1] && A[mid] < A[mid + 1])
			return peakIndexInMountainArray(A, mid, hi);
		return peakIndexInMountainArray(A, lo, mid);
	}
	// 852. Peak Index in a Mountain Array
	int peakIndexInMountainArray(vector<int>& A) {  // beat 98%
		return peakIndexInMountainArray(A, 0, A.size() - 1);
	}
	// 744. Find Smallest Letter Greater Than Target
	char nextGreatestLetter(vector<char>& letters, char target) {  // beat 96% on 3rd submit
		auto ans = upper_bound(begin(letters), end(letters), target);
		if (ans == end(letters))  // wrap around if all letter < target
			ans = begin(letters);
		return *ans;
	}
	// 704. Binary Search. If target exists, then return its index, otherwise return -1
	int search(vector<int>& nums, int target) {  // beat 96% on 2nd submit
		auto found = lower_bound(nums.begin(), nums.end(), target);
		if (found == nums.end() || *found != target)
			return -1;
		return distance(nums.begin(), found);
	}
	// 35. Search Insert Position. return the index if the target is found. If not, return the index where it would be if it were inserted in order
	int searchInsert(vector<int>& nums, int target) {  // beat 98%
		return distance(nums.begin(), lower_bound(nums.begin(), nums.end(), target));
	}

	// 475. Heaters, given positions of houses and heaters, find the min heating radius to warm all houses
	// idea: find max distance of any 2 neighboring heaters, find distance of first house to first heater, last house to last heater
	int findRadius(vector<int>& houses, vector<int>& heaters) { // beat 97%
		if (houses.empty() || heaters.empty())
			return 0;
		sort(begin(heaters), end(heaters)); // question did not mention sorted, don't assume
		int maxDist = 0;
		int h_size = heaters.size();
		for (int h : houses) {
			auto R_H = lower_bound(begin(heaters), end(heaters), h);
			if (R_H == end(heaters))
				maxDist = max(maxDist, h - heaters[h_size - 1]);  // left heater only
			else if (R_H == begin(heaters))
				maxDist = max(maxDist, *R_H - h);  // right heater only
			else {
				auto L_H = R_H - 1;
				maxDist = max(maxDist, min(*R_H - h, h - *L_H));
			}
		}
		return maxDist;
	}

	bool isBadVersion(int version)  // function given by tester
	{
		return version >= 7;  // test case
	}
	// 278. First Bad Version, [1,n], all the versions after a bad version are also bad
	int firstBadVersion(int lo, int hi) { // beat 100%
		if (isBadVersion(lo))
			return lo;
		int mid = lo + (hi - lo) / 2;  // trick to avoid overflow
		if (isBadVersion(mid))
			return firstBadVersion(lo, mid);
		else
			return firstBadVersion(mid + 1, hi);
	}
	int firstBadVersion(int n) {
		return firstBadVersion(1, n);
	}
	//930. Binary Subarrays With Sum
	int numSubarraysWithSum(vector<int>& A, int S) {
		if (S == 0) {  // special case
			auto counts = count_consecutive(begin(A), end(A), S);
			return accumulate(begin(counts), end(counts), 0,
				[](int init, int c) { return init + c*(c + 1) / 2; });
		}
		partial_sum(begin(A), end(A), begin(A));
		if (A.empty() || S>A[A.size()-1])
			return 0;
		int total = 0;
		// special case for first
		auto start = equal_range(begin(A), end(A), 0); // start from 0
		auto sum_end= equal_range(begin(A), end(A), S);
		total += (distance(start.first, start.second) + 1)*(distance(sum_end.first, sum_end.second));
		while (sum_end.second != end(A)) {
			int target = *start.second + S;
			start = equal_range(start.second, end(A), *start.second);
			sum_end = equal_range(sum_end.second, end(A), target);
			total += (distance(start.first, start.second))*(distance(sum_end.first, sum_end.second));
		}
		return total;
	}
};
TEST_CASE("930. Binary Subarrays With Sum", "[NEW]")
{
	CHECK(BinarySearch().numSubarraysWithSum(vector<int>{0, 0, 0, 0, 0}, 0) == 15);
	CHECK(BinarySearch().numSubarraysWithSum(vector<int>{0,0,1, 0, 0, 0, 0}, 1) == 15); //3x5
}
TEST_CASE("475. Heaters", "[NEW]")
{
	CHECK(BinarySearch().findRadius(vector<int>{282475249, 622650073, 984943658, 144108930, 470211272, 101027544, 457850878, 458777923}, vector<int>{823564440, 115438165, 784484492, 74243042, 114807987, 137522503, 441282327, 16531729, 823378840, 143542612}) == 161834419);
	CHECK(BinarySearch().findRadius(vector<int>{3, 5}, vector<int>{1}) == 4);
	CHECK(BinarySearch().findRadius(vector<int>{1, 5}, vector<int>{2}) == 3);
	CHECK(BinarySearch().findRadius(vector<int>{1, 2, 3}, vector<int>{2}) == 1);
	CHECK(BinarySearch().findRadius(vector<int>{1, 2, 3}, vector<int>{1}) == 2);
	CHECK(BinarySearch().findRadius(vector<int>{1, 2, 3}, vector<int>{3}) == 2);
	CHECK(BinarySearch().findRadius(vector<int>{1, 2, 3, 4}, vector<int>{1, 4}) == 1);
}
TEST_CASE("852. Peak Index in a Mountain Array", "[NEW]")
{
	CHECK(BinarySearch().peakIndexInMountainArray(vector<int>{24, 69, 100, 99, 79, 78, 67, 36, 26, 19}) == 2);
}

TEST_CASE("69. Sqrt(x)", "[BS]")
{
	CHECK(BinarySearch().mySqrt(0) == 0);
	CHECK(BinarySearch().mySqrt(1) == 1);
	CHECK(BinarySearch().mySqrt(2) == 1);
	CHECK(BinarySearch().mySqrt(3) == 1);
	CHECK(BinarySearch().mySqrt(4) == 2);
	CHECK(BinarySearch().mySqrt(2147395599) == 46339);
}
class TopVotedCandidate {
	vector<int> winners;
	vector<int> times_;
public:
	TopVotedCandidate(vector<int> persons, vector<int> times): times_(move(times)), winners(persons.size(), 0) {
		vector<int> count(persons.size() + 1, 0);
		int top = 0;
		int time = 0;
		for (int p : persons) {
			if (++count[p] >= count[top])
				top = p;
			winners[time++] = top; // winning person at each time, pre-process
		}
	}

	int q(int t) {  // beat 95%
		auto found = lower_bound(begin(times_), end(times_), t);
		if (found ==end(times_) || *found > t)
			--found;
		int pos = distance(begin(times_), found);
		return winners[pos];
	}
};
TEST_CASE("911. Online Election", "[NEW]")
{
	TopVotedCandidate vote(vector<int>{0, 0, 1, 1, 2}, vector<int>{0, 67, 69, 74, 87});
	CHECK(vote.q(70) == 0);
	CHECK(vote.q(100) == 1);
	CHECK(vote.q(75) == 1);
}
