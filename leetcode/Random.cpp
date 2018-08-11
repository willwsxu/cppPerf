#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"

using namespace std;

//384. Shuffle an Array
class Shuffle {  // beat 98%
	vector<int> orig;
	std::random_device rd;
	std::mt19937 g;
public:
	Shuffle(vector<int> nums) :orig(std::move(nums)), g(rd()) {

	}

	/** Resets the array to its original configuration and return it. */
	vector<int> reset() {
		return orig;
	}

	/** Returns a random shuffling of the array. */
	vector<int> shuffle() {
		vector<int> ret(orig);
		std::shuffle(ret.begin(), ret.end(), g);
		return ret;
	}
};

