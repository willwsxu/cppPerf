#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
#include <map>

class Test {
	vector<int> B;  // broken id
	bool interactive = true;
	int  max_calls=0;
public:
	void set_test(vector<int> b) {
		B= move(b);
		interactive = false;
	}
	void set_max(int F) {
		max_calls = F;
		interactive = true;
	}
	pair<int, int> get_result(int bitmask1, int bitmask0) {
		if (interactive) {
			if (--max_calls < 0)
				return { 0,0 };
			cout << string(bitmask1, '1') << string(bitmask0, '0') << endl;
			string judge;
			getline(cin, judge);
			if (judge == "-1") {
				exit(0);
				return { 0,0 }; // failed
			}
			int ones = count(begin(judge), end(judge), '1');
			return { ones, (int)judge.size() - ones };
		}
		else
		{
			auto found = lower_bound(begin(B), end(B), bitmask1);
			return { bitmask1 - (int)distance(begin(B), found), bitmask0 - (int)distance(found, end(B)) };
		}
	}
};
Test test;
void bit_search_interactive(int N, int left_ones, int right_zero, int start, int mid, int end, vector<int>& result) {
	auto ans = test.get_result(mid, N-mid);
	// compute 1 and 0 within range [start, end)
	int exp1 = mid - start;
	int exp0 = end - mid;
	int act1 = ans.first - left_ones;
	int act0 = ans.second - right_zero;
	if (act0 < exp0) {
		int size = end - mid;
		if (size == 1)
			result.push_back(mid); // found the broken bit
		else
			bit_search_interactive(N, act1 + left_ones, right_zero, mid, mid+size/2, end, result);
	}
	if (act1 < exp1) {
		int size = mid-start;
		if (size == 1)
			result.push_back(start); // found the broken bit
		else
			bit_search_interactive(N, left_ones, act0 + right_zero, start, start+size/2, mid, result);
	}
}

void interactive()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N, B, F;
		cin >> N >> B >> F;
		cin.ignore();
		test.set_max(F);
		vector<int> result;
		bit_search_interactive(N, 0, 0, 0, N / 2, N, result);
		sort(begin(result), end(result));
		for (int b : result)
			cout << b << " ";
		cout << endl;
		string judge;
		getline(cin, judge);
		if (judge == "-1")
			break;
	}
}

int main()
{
	interactive();
}
/*
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "..\catch.hpp"

TEST_CASE("jam2019 qualify #1", "[Q4]")
{
	//interactive();

	vector<int> test1{1,5};
	test.set_test(test1);
	vector<int> result;
	bit_search_interactive(8, 0, 0, 0, 4, 8, result);
	sort(begin(result), end(result));
	CHECK(result == test1);

	result.clear();
	bit_search_interactive(9, 0, 0, 0, 4, 9, result);
	sort(begin(result), end(result));
	CHECK(result == test1);

	result.clear();
	bit_search_interactive(10, 0, 0, 0, 5, 10, result);
	sort(begin(result), end(result));
	CHECK(result == test1);

	result.clear();
	bit_search_interactive(11, 0, 0, 0, 5, 11, result);
	sort(begin(result), end(result));
	CHECK(result == test1);

	vector<int> test2{ 2,3,5 };
	test.set_test(test2);
	result.clear();
	bit_search_interactive(6, 0, 0, 0, 3, 6, result);
	sort(begin(result), end(result));
	CHECK(result == test2);

	result.clear();
	bit_search_interactive(1024, 0, 0, 0, 512, 1024, result);
	sort(begin(result), end(result));
	CHECK(result == test2);
}

/*
void bit_search_interactive2(int N, int B, map<int, int>& intervals) {  // map broken bits in each interval
	string bit_masks;
	bit_masks.reserve(N);
	pair<int, int>& prev{ 0,0 };
	bool done = true;
	int fill = 1;
	for (const pair<int, int>& p : intervals) {
		int bits = p.first - prev.first;
		prev = p;
		if (p.second && p.second < bits) {
			while (bits > p.second) {
				bit_masks.append(p.second, '0' + fill);
				fill = 1 - fill;
				bits -= p.second;
			}
			done = false;
		}
		if (bits) {
			bit_masks.append(bits, '0' + fill);
			fill = 1 - fill;
		}
	}
	auto ans = test.get_result(bit_masks);
	int prev_idx = 0;
	for (int idx = 1; idx < N; idx++) {
		if (bit_masks[idx] != bit_masks[prev_idx]) {

		}
	}
	// compute 1 and 0 within range [start, end)
	int exp1 = mid - start;
	int exp0 = end - mid;
	int act1 = ans.first - left_ones;
	int act0 = ans.second - right_zero;
	if (act0 < exp0) {
		int size = end - mid;
		if (size == 1)
			result.push_back(mid); // found the broken bit
		else
			bit_search_interactive(N, act1 + left_ones, right_zero, mid, mid + size / 2, end, result);
	}
	if (act1 < exp1) {
		int size = mid - start;
		if (size == 1)
			result.push_back(start); // found the broken bit
		else
			bit_search_interactive(N, left_ones, act0 + right_zero, start, start + size / 2, mid, result);
	}
}
*/