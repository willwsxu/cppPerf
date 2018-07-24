#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
using namespace std;


class SingleNumber
{
public:
	// 136 every element appears twice except for one.Find that single one.
	int singleNumber(vector<int>& nums) {  // beat 97%, interesting queestion
		int ans = 0;
		for (int n : nums)  // xor cancel out same number. xor has associative property and commutative property
			ans ^= n;
		return ans;
	}

	//137  every element appears three times except for one, which appears exactly once
	// bit counting ideas: 0, 1, 10, 11
	// first bit will alternate between 0 and 1, so x0=x0 ^ i
	// next bit change to 1 only if previous bit was 1 and new one is 1, x2=x2 ^(x1&i)
	// each bit of the integer is counted by same bit position using multpile int counters
	int singleNumber2(vector<int>& nums) {  // beat 99%
		int bit1Count = 0, bit2Count = 0;  // 2 bits needed to count k=3
		for (int n : nums) {
			bit2Count ^= (bit1Count&n);  // calculate second bit before first bit is updated
			bit1Count ^= n;
			int mask = ~(bit2Count &bit1Count);  // both bit 1, flip to 0 as we count base is 3 (0, 1, 2)
			bit1Count &= mask;  // set bit to 0 when count is 3 (binary 11)
			bit2Count &= mask;
		}
		return bit1Count;
	}

	// 260 exactly two elements appear only once and all the other elements appear exactly twice
	vector<int> singleNumber3(vector<int>& nums) {  // beat 98%
		int x = accumulate(nums.begin(), nums.end(), 0, bit_xor<int>()); // a^b
		int diff_bit = (x & (-x));  // a and b are different, pick right most bit that is different between and and b
		int a = 0, b = 0;
		for (int n : nums) {  // separate a and b into two groups, now that problem is reduced to problem 136
			if (n&diff_bit)
				a ^= n;
			else
				b ^= n;
		}
		return{ a, b };
	}
};