#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

class BackTracking {
	// 842. Split Array into Fibonacci Sequence, a least 3 numbers
	// number cannot start with 0, unless it is 0
	// 0 <= F[i] <= 2^31 - 1
	vector<int> ans;
	int to_int(const string& S, int start, int last)
	{
		int len = last - start + 1;
		if (len == 10 && S[start] > '2')  // overflow
			return -1;
		return atoi(S.substr(start, len).c_str());
	}
	bool splitIntoFibonacci(const string& S, int start) {
		int len = S.size();
		if (start == len)
			return true;
		int n = ans.size();
		int sum = ans[n - 1] + ans[n - 2];
		for (int j = start; j < len; j++) {
			int next = to_int(S, start, j);
			if (next < 0)
				return false;
			if (sum == next) {
				ans.push_back(next);
				if (splitIntoFibonacci(S, j + 1))
					return true;
				ans.pop_back();
			}
			else if (next > sum)  // pruning, no need to check further
				return false;
			if (S[start] == '0')  // no leading 0 unless it is 0
				break;
		}
		return false;
	}

public:
	vector<int> splitIntoFibonacci(string S) {
		int n = S.size();
		for (int i = 0; i < n - 2; i++) {
			int next = to_int(S, 0, i);
			if (next < 0)  // overflow
				break;
			ans.push_back(next);
			for (int j = i + 1; j < n - 1; j++) {
				int next = to_int(S, i + 1, j);
				if (next < 0)  // overflow
					break;
				ans.push_back(next);
				if (splitIntoFibonacci(S, j + 1))
					return ans;
				ans.pop_back();
				if (S[i + 1] == '0')  // no leading 0 unless it is 0
					break;
			}
			ans.pop_back();
			if (S[0] == '0')  // no leading 0 unless it is 0
				break;
		}
		return{};
	}
};

TEST_CASE("splitIntoFibonacci", "[FIB]")
{
	BackTracking s;
	CHECK(s.splitIntoFibonacci("214748364721474836422147483641") == vector<int>{});
	CHECK(s.splitIntoFibonacci("0123") == vector<int>{});
	CHECK(s.splitIntoFibonacci("1023") == vector<int>{});
	CHECK(s.splitIntoFibonacci("1203") == vector<int>{});
	CHECK(s.splitIntoFibonacci("123456579") == vector<int>{123, 456, 579});
}
