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


// 398. Random Pick Index
class ReservoirSampling {  // beat 48%
	vector<int> reserve;
	std::random_device rd;  // slow, use as seed for mt19937
	std::mt19937 g;
public:
	ReservoirSampling(vector<int> nums) : reserve(nums), g(rd()) {	}

	// first target, pick 100%
	// second target, give 50% chance to be picked. 50% chance of first two is picked
	// 3rd target, 1/3 pick, 2/3 not picked. 1/3 chance any last three is picked
	int pick(int target) {
		int ret = -1;
		int count = 0;  // count of target
		for (size_t i = 0; i < reserve.size(); i++) {
			if (reserve[i] == target) {
				auto x = uniform_int_distribution<>(0, count++)(g);
				if (x == 0) // new number is picked 1/count chances
					ret = i;
			}
		}
		return ret;
	}
};


TEST_CASE("398. Random Pick Index", "[NEW]")
{
	ReservoirSampling rs(vector<int>{1});
	CHECK(rs.pick(1) == 0);
	CHECK(rs.pick(1) == 0);
	CHECK(rs.pick(1) == 0);
	CHECK(rs.pick(1) == 0);
}

#include "ListNode.h"
// 382. Linked List Random Node
class Solution {  // beat 73%
	ListNode *head_;
	std::random_device rd;  // same idea as ReservoirSampling
	std::mt19937 g;
public:
	/** @param head The linked list's head.
	Note that the head is guaranteed to be not null, so it contains at least one node. */
	Solution(ListNode* head):head_(head), g(rd()){	}

	/** Returns a random node's value. */
	int getRandom() {
		int ret = 0;
		int count = 0;
		ListNode *cur = head_;
		while (cur) {
			if (uniform_int_distribution<>(0, count++)(g) == 0)  // 1 of count chance to pick new number
				ret = cur->val;
			cur = cur->next;
		}
		return ret;
	}
};