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
class RandomNode {  // beat 73%
	ListNode *head_;
	std::random_device rd;  // same idea as ReservoirSampling
	std::mt19937 g;
public:
	/** @param head The linked list's head.
	Note that the head is guaranteed to be not null, so it contains at least one node. */
	RandomNode(ListNode* head):head_(head), g(rd()){	}

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

class Random
{
	std::random_device rd;
	std::mt19937 g;
public:
	Random() :g(rd()) {}

	int rand7() {  // my own impl of rand7, for testing
		return uniform_int_distribution<>(1, 7)(g); 
	}
	// 470. Implement Rand10() Using Rand7()
	int rand10() { // borrowed idea
		int ret;
		do {
			ret = 7*(rand7()-1) + rand7();  // generate rand 1 to 49
		} while (ret > 10);  // only pick first 10
		return ret;
	}
};

class WeightedPick
{
	// 528. Random Pick with Weight
	// 1 <= w.length <= 10000,1 <= w[i] <= 10^5, pickIndex will be called at most 10000 times.
	vector<int> weighted;
	std::random_device rd;
	std::mt19937 g;
	unique_ptr<uniform_int_distribution<>> dis;
public:
	WeightedPick(vector<int> w): weighted(w), g(rd()){
		prefix_sum(weighted);
		dis = make_unique<uniform_int_distribution<>>(1, weighted[weighted.size() - 1]);
	}

	int pickIndex() {
		auto x=lower_bound(begin(weighted), end(weighted), (*(dis.get()))(g));
		return distance(begin(weighted), x);
	}
};

int Points(const vector<int>& rect) {  // total valid points inside a rectangle, include sides
	return (rect[2] - rect[0]+1)*(rect[3] - rect[1]+1);
}
// 497. Random Point in Non-overlapping Rectangles
class RectanglesPick {
	vector<vector<int>> rects_;
	vector<int> areas;
	std::random_device rd;
	std::mt19937 g;
	unique_ptr<uniform_int_distribution<>> dis;
public:
	RectanglesPick(vector<vector<int>> rects):rects_(rects), g(rd()) {  // rect(x1,y1,x2,y2), bottom left, top right
		transform(begin(rects), end(rects), back_inserter(areas), [](const auto& rect) {
			return Points(rect);
		});
		prefix_sum(areas);
		dis = make_unique<uniform_int_distribution<>>(1, areas[areas.size() - 1]);
	}

	vector<int> pick() { // two picks, beat 58%
		auto x = lower_bound(begin(areas), end(areas), (*(dis.get()))(g));  // pick rect first proportional to its points
		size_t idx = distance(begin(areas), x);
		int A = Points(rects_[idx]);
		uniform_int_distribution<> rect_pick(0, A-1); // uniformly chose any point in a chosen rect
		int p = rect_pick(g); // second pick
		int width = rects_[idx][2] - rects_[idx][0]+1;  // points horizontally, add 1
		return{ rects_[idx][0] + p%width, rects_[idx][1] + p / width };
	}
};

TEST_CASE("497. Random Point in Non-overlapping Rectangles", "[NEW]")
{
	RectanglesPick rects({ {-2,-2,-1,-1},{1,0,3,0} });
	auto x=rects.pick();
}