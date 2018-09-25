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
};