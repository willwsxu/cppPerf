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
};
TEST_CASE("69. Sqrt(x)", "[NEW]")
{
	CHECK(BinarySearch().mySqrt(0) == 0);
	CHECK(BinarySearch().mySqrt(1) == 1);
	CHECK(BinarySearch().mySqrt(2) == 1);
	CHECK(BinarySearch().mySqrt(3) == 1);
	CHECK(BinarySearch().mySqrt(4) == 2);
	CHECK(BinarySearch().mySqrt(2147395599) == 46339);
}
