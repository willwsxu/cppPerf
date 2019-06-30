#include <string>
#include <map>
#include <numeric>
#include <iostream>

using namespace std;
//1041 robot started at origin, facing N. Given instructions
// G move a step, L turn left, R turn right
// if the instruction is repeated, is robot bounded
bool isRobotBounded(string instructions) {
	int L = 0, R = 0, U = 0, D = 0;  // bounds
	int x = 0, y = 0;  // location
	char dir = 'N';
	map<char, char> turns_L{ {'N','W'},{'W','S'},{'S','E'},{'E','N'} };
	map<char, char> turns_R{ {'N','E'},{'W','N'},{'S','W'},{'E','S'} };
	for (int r = 0; r < 20; r++) {
		bool no_change = true;  // compute bounds in each round and check if bound is changing
		auto update = [&no_change](int& max_val, int new_val) {
			if (new_val > max_val) {
				max_val = new_val;
				no_change = false;
			}
		};
		for (char m : instructions) {
			switch (m) {
			case 'G':
				switch (dir) {
				case 'N': y++;  break;
				case 'S': y--;  break;
				case 'E': x++;  break;
				case 'W': x--;  break;
				}
				update(L, -x);
				update(R, x);
				update(U, y);
				update(D, -y);
				break;
			case 'L':
				dir = turns_L[dir]; break;
			case 'R':
				dir = turns_R[dir]; break;
			}
		}
		if (no_change)
			return true;
	}
	return false;
}

#include <vector>
#include <iostream>
#include <algorithm>
// swap two numbers to generate largest permutation that is smaller than current
// numbers in array may not be unique
// if no swap is possible, return original
// idea: search from back and find the first number that is increasing, say it is target1
//       search left-to-right increasing sequence after target1, find target2<target1 (gurranteed if target1 exists)
//       swap(target1, target2)
vector<int> prevPermOpt1(vector<int>& A) {
	auto increasing_from_back = adjacent_find(rbegin(A), rend(A), [](int a, int b) { return a < b; });
	if (increasing_from_back != rend(A)) {
		auto start = increasing_from_back.base()-1;  // base point to to right, find start of increasing sequence
		auto target1 = start - 1;  // target1>start, *target1>0
		auto target2 = lower_bound(start, end(A), *target1 - 1);  // find largest digit smaller than target1
		if (target2==end(A) || *target2 >= *target1)  // did not find the exact value (*target1-1)
			target2 = lower_bound(start, end(A), *(target2 - 1)); // search for first of value target2
		iter_swap(target1, target2);
	}
	return A;
}

vector<int> addNegabinary(vector<int>& arr1, vector<int>& arr2) {
	reverse(begin(arr1), end(arr1));
	reverse(begin(arr2), end(arr2));
	size_t size=max(arr1.size(), arr2.size());
	arr1.resize(size);
	arr2.resize(size);
	vector<int> result;
	result.reserve(size + 1);
	int carry = 0;  // in decimal, for human brain
	for (int i = 0; i < size; i++) {
		carry = arr1[i] + arr2[i] + carry;
		result.push_back(carry & 1);
		carry = -(carry >> 1);
	}
	while (carry != 0) {
		result.push_back(carry & 1);
		carry = -(carry >> 1);
	}
	auto last = end(result) - 1;
	while (last > begin(result) && *last == 0)
		--last;
	++last;
	result.erase(last, end(result));
	reverse(begin(result), end(result));
	return result;
}
vector<double> sampleStats(vector<int>& count) {
    int mode = distance(begin(count), max_element(begin(count), end(count)));
    int total_count = accumulate(begin(count), end(count), 0);
    int min_val = -1, max_val = -1;
    double total_val = 0;
    int running_count = 0;
    bool odd = total_count % 2 > 0;
    int mid = total_count / 2;
    if (odd)
        mid++;
    double median = -1;
    bool median2 = false;
    for (int i = 0; i < count.size(); i++) {
        if (count[i] == 0)
            continue;
        if (min_val < 0)
            min_val = i;
        max_val = i;
        total_val += (double)count[i] * i;
        running_count += count[i];
        if (median2) {
            median2 = false;
            median = (median + i) / 2;
        }
        if (median < 0 && running_count >= mid) {
            median = i;
            if (!odd && running_count == mid)
                median2 = true;
        }
    }
    return vector<double>{(double)min_val, (double)max_val, total_val / total_count, median, (double)mode};
}

vector<int> pathInZigZagTree(int label) {
    vector<int> power2(21, 1);
    for (int i = 1; i < power2.size(); i++) {
        power2[i] = 2 * power2[i - 1];
    }
    int depth;
    for (int i = power2.size() - 1; i >= 0; i--) {
        if (label >= power2[i]) {
            depth = i + 1;
            break;
        }
    }
    vector<int> ans{ label };
    int leaf_count = label - power2[depth - 1] + 1;
    int left2right_pos = depth % 2 == 0 ? (power2[depth - 1] - leaf_count) : leaf_count - 1;
    while (depth > 1) {
        left2right_pos /= 2;
        depth--;
        int count = power2[depth - 1];
        if (depth % 2 > 0)
            count += left2right_pos;
        else
            count = power2[depth] - left2right_pos - 1;
        ans.push_back(count);
    }
    reverse(begin(ans), end(ans));
    return ans;
}


#include "catch.hpp"
TEST_CASE("1104. Path In Zigzag Labelled Binary Tree", "[MATH]")
{
    CHECK(pathInZigZagTree(14) == vector<int>{1, 3, 4, 14});
    CHECK(pathInZigZagTree(26) == vector<int>{1, 2, 6, 10, 26});
}

TEST_CASE("1093. Statistics from a Large Sample", "[MATH]")
{
    CHECK(sampleStats(vector<int>{0, 1, 3, 4}) == vector<double>{1.0, 3.0, 2.375, 2.5, 3.0});
}

TEST_CASE("1041 robot bounded", "[MATH]")
{
	CHECK(isRobotBounded("LLGLR") == true);
}

TEST_CASE("1053. Previous Permutation With One Swap", "[MATH]")
{
	CHECK(prevPermOpt1(vector<int>{1,2,3}) == vector<int>{1,2,3});
	CHECK(prevPermOpt1(vector<int>{3, 2, 1}) == vector<int>{3, 1,2});
	CHECK(prevPermOpt1(vector<int>{3, 3,1}) == vector<int>{3, 1, 3});  // 
	CHECK(prevPermOpt1(vector<int>{3,1,1,3}) == vector<int>{1,3,1,3});
	CHECK(prevPermOpt1(vector<int>{1,9,4,6,7}) == vector<int>{1,7,4,6,9});
}
TEST_CASE("1073. Adding Two Negabinary Numbers", "[MATH]")
{
	CHECK(addNegabinary(vector<int>{1,0,1}, vector<int>{1,0,1}) == vector<int>{1,1,1,1,0});
	CHECK(addNegabinary(vector<int>{1, 1, 1, 1}, vector<int>{1, 1, 1, 1}) == vector<int>{1, 0, 1, 0});
	CHECK(addNegabinary(vector<int>{1, 1}, vector<int>{1}) == vector<int>{0});  // corner case, leading 0
}