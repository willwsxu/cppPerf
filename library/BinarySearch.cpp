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
};
