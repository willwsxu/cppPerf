
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
using namespace std;

// given N stones, pick any two, smash them. if weight x==y, both destroyed
// if x<y, x is destroyed, y weight is y-x
// find the smallest weight of last stone
// N [1,30], weight [1,100]
using mvi=map<vector<int>, int>;
int lastStoneWeightII(vector<int>& stones, mvi& memo) {
	sort(begin(stones), end(stones), greater<int>());
	while (stones.back() == 0)
		stones.erase(end(stones) - 1);
	if (stones.size() ==1) {
		return stones[0];
	}
	if (memo.find(stones) != end(memo))
		return memo[stones];
	int result = INT32_MAX;
	for (int i = 0; i < stones.size()-1; i++) {
		if (i > 0 && stones[i] == stones[i - 1])
			continue;
		for (int j = i + 1; j < stones.size(); j++) {
			if (j > i + 1 && stones[j] == stones[j - 1])
				continue;
			int saveI = stones[i];
			int saveJ = stones[j];
			vector<int> next(begin(stones), end(stones));
			if (saveI == saveJ) {
				next[i] = next[j] = 0;
			}
			else if (saveI > saveJ) {
				next[i] -= next[j];
				next[j] = 0;
			}
			else {
				next[j] -= next[i];
				next[i] = 0;
			}
			result = min(result, lastStoneWeightII(next, memo));
		}
	}
	memo[stones] = result;
	return result;
}
int lastStoneWeightII(vector<int>& stones) {
	mvi memo;
	memo[vector<int>{}] = 0;
	return lastStoneWeightII(stones, memo);
}

// clash 2 heaviest stones at each round
int lastStoneWeight(vector<int>& stones) {
	int size = stones.size();
	while (size > 1) {
		sort(begin(stones), end(stones), greater<int>());
		stones[0] -= stones[1];
		stones[1] = 0;
		size--;
		if (stones[0] == 0)
			size--;
	}
	sort(begin(stones), end(stones), greater<int>());
	return size > 0 ? stones[0] : 0;
}

#include "catch.hpp"
TEST_CASE("contest 1", "[1B3]")
{
	CHECK(lastStoneWeight(vector<int>{2, 7, 4, 1, 8, 1}) == 1);
}
TEST_CASE("last stone weight II", "[DYN]")
{
	CHECK(lastStoneWeightII(vector<int>{2, 7, 4, 1, 8, 1}) == 1);
}

/*
int main(int argc, char* argv[])
{
	cout << argc << endl;
}
*/