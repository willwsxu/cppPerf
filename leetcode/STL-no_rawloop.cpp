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
};