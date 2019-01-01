#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"

using namespace std;
#include "eraseRemove.h"

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


TEST_CASE("398. Random Pick Index", "[RAND]")
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

TEST_CASE("497. Random Point in Non-overlapping Rectangles", "[RAND]")
{
	RectanglesPick rects({ {-2,-2,-1,-1},{1,0,3,0} });
	auto x=rects.pick();
}


// 519. Random Flip Matrix, initially all cells are 0
class FlipMatrix { // beat 100%
	int rows, cols;
	int total;			// # of 0 in matrix
	map<int,int> matrix; // flattened matrix mapping of index to matrix cell, MLE if use vector
	std::random_device rd;
	std::mt19937 g;
public:
	FlipMatrix(int n_rows, int n_cols): rows(n_rows), cols(n_cols),g(rd()) {
		total = rows*cols;
	}

	// uniformly flip any cell of 0 to 1, assume flip is not called unless there is 0
	vector<int> flip() { // Fisher–Yates shuffle
		auto r = uniform_int_distribution<>(0, --total)(g);  // random position of matrix with values 0
		if (r == total) {  //last value selected
			r = WXU::map_get_or_default(matrix, r, r); // last value could be mapped to something else
			return { r / cols, r%cols };
		}
		int x = WXU::map_get_or_default(matrix, r, r); // look for existing mapping
		matrix[r]= WXU::map_get_or_default(matrix, total, total); // update random selected cell to last elementas last element will drop out in next call
		return{ x / cols, x%cols };
	}

	void reset() {
		total = rows*cols;
		matrix.clear(); //remove all mapping to start over
	}
};

TEST_CASE("519. Random Flip Matrix", "[RAND]")
{
	FlipMatrix matrix(2, 2);
	vector<int> ans;
	for (int i = 0; i < 4; i++) {
		auto a1 = matrix.flip();
		ans.push_back(a1[0] * 2 + a1[1]);
	}
	sort(ans.begin(), ans.end());
	CHECK(ans == vector<int>{0,1,2,3});
	matrix.reset();
	ans.clear();
	for (int i = 0; i < 4; i++) {
		auto a1 = matrix.flip();
		ans.push_back(a1[0] * 2 + a1[1]);
	}
	sort(ans.begin(), ans.end());
	CHECK(ans == vector<int>{0, 1, 2, 3});

	FlipMatrix large(10000, 10000);
	large.flip();
}

// 478. Generate Random Point in a Circle. Given the radius and x-y positions of the center of a circle
class RandomCircle {
	std::random_device rd;
	std::mt19937 g;
	double r, x, y;
	const double PI = 3.14159265358979732384626433832795;
	uniform_real_distribution<> dis_r;
	uniform_real_distribution<> dis_t;
public:
	RandomCircle(double radius, double x_center, double y_center):r(radius), x(x_center),y(y_center), g(rd()), dis_r(0,1), dis_t(0, 2 * PI) {
	}

	vector<double> randPoint() {  // polar coordinates, slow
		double r_ = sqrt(dis_r(g))*r;  // sqrt is key to get correct distribution (not too dense at center)
		double theta = dis_t(g);
		return{ x+r_*cos(theta), y+r_*sin(theta) };
	}
};


TEST_CASE("478. Generate Random Point in a Circle", "[RAND]")
{
	RandomCircle circ(10, 5, -7.5);
	auto ans = circ.randPoint();
	double dx = abs(ans[0] - 5);
	double dy = abs(ans[1] + 7.5);
	CHECK(dx*dx + dy*dy <= 100);

	RandomCircle circ2(0.01, -73839.1, -3289891.3);
	int i = 0;
	int loops = 10000;
	for (i = 0; i < loops; i++) {
		auto ans = circ2.randPoint();
		double dx = abs(ans[0] + 73839.1);
		double dy = abs(ans[1] + 3289891.3);
		CHECK(dx <= 0.01);
		CHECK(dy <= 0.01);
		CHECK(dx*dx + dy*dy <= 0.0001);
	}
	CHECK(i == loops);
}


// average O(1) time
class RandomizedSet {  // beat 93% after making random engine as data member
	vector<int> indexMap;
	unordered_map<int, int> valueMap;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()

public:
	/** Initialize your data structure here. */
	RandomizedSet() :gen(rd()) {	}

	/** Inserts a value to the set. Returns true if the set did not already contain the specified element. */
	bool insert(int val) {
		if (valueMap.find(val) != valueMap.end())
			return false;
		auto size = valueMap.size();  // solve leetcode compiler bug, valueMap[val]=valueMap.size() didn't work
		valueMap[val] = size; // map value to last index (0 based)
		indexMap.emplace_back(val);
		return true;
	}

	/** Removes a value from the set. Returns true if the set contained the specified element. */
	bool remove(int val) {
		auto found = valueMap.find(val);
		if (found == valueMap.end())
			return false;
		int remIndex = found->second; // index to remove
		valueMap.erase(found);
		int last = indexMap.size() - 1;
		if (remIndex < last)  // remove is not the last one
		{
			indexMap[remIndex] = indexMap[last]; // swap value at last
			valueMap[indexMap[remIndex]] = remIndex;  // update index in value map
		}
		indexMap.pop_back();
		return true;
	}

	/** Get a random element from the set. */
	int getRandom() {
		std::uniform_int_distribution<int> dist(0, indexMap.size() - 1);
		return indexMap.at(dist(gen));
	}
};


TEST_CASE("random set O(1) op", "[RAND]")
{
	RandomizedSet t;
	CHECK(t.insert(1) == true);
	CHECK(t.remove(2) == false);
	CHECK(t.insert(2) == true);
	cout << "get random=" << t.getRandom() << endl;
	CHECK(t.remove(1) == true);
	CHECK(t.insert(2) == false);
	CHECK(t.getRandom() == 2);  // fix bug insert
}

class HeavyHitter
{
	vector<int> majorityElements(vector<int>& nums, int k) { // elements appears > n/(k+1)
		map<int, int> maj;
		for (int n : nums) {
			if ((int)maj.size() < k || maj.count(n))
				maj[n]++;
			else {  // K+1 different values, time to cancel out 
				erase_remove_if(maj, [](const auto&p) { return p.second == 1; });
				for (auto& p : maj)
					p.second--;
			}
		}
		// verify if those retained in map are actually majority
		for_each(begin(maj), end(maj), [](auto&p) {p.second = 0; });
		for (int n : nums) {
			if (maj.count(n))
				maj[n]++;
		}
		vector<int> ans;
		const int threshold = nums.size() / (k + 1);
		WXU:: transform_if(begin(maj), end(maj), back_inserter(ans), [](const auto& p) { return p.first; }, [threshold](const auto& p) { return p.second > threshold; });
		return ans;
	}

public:
	// 169. Majority Element, return value appears > n/2
	int majorityElement(vector<int>& nums) {  // beat 97%
		int maj = nums[0];
		int count = 1;
		for (size_t i = 1; i < nums.size(); i++) {
			if (count == 0) {
				count = 1;
				maj = nums[i]; // new majority
			}
			else if (nums[i] == maj)
				count++;
			else
				count--;  // cancel out when numbers are different
		}
		return maj;
	}
	// 229. Majority Element II, find all elements that appear more than ⌊ n/3 ⌋ times
	vector<int> majorityElements(vector<int>& nums) {// beat 98%
		return majorityElements(nums, 2);
	}
};



TEST_CASE("229. Majority Element II", "[NEW]")
{
	CHECK(HeavyHitter().majorityElements(vector<int>{3, 2, 3}) == vector<int>{3});
	CHECK(HeavyHitter().majorityElements(vector<int>{1, 1, 1, 3, 3, 2, 2, 2}) == vector<int>{1, 2});
	CHECK(HeavyHitter().majorityElements(vector<int>{1, 1, 1, 3, 3, 2, 2, 2, 2, 2}) == vector<int>{2});
}

