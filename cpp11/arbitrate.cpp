#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// Give 2 sequence numbers, combine into one and print any gap
vector<int> arbitrate(vector<int>& A, vector<int>& B)
{
	auto a = begin(A), b=begin(B);
	vector<int> result;
	result.reserve(max(A.size(), B.size()));
	auto add = [&result](int s) {
		if (!result.empty() && result.back() + 1 < s)
			cout << "Gap from " << result.back() << " to " << s << "\n";
		result.push_back(s);
	};
	while (a != end(A) && b != end(B)) {
		if (*a < *b) {
			add(*a);
			++a;
		}
		else if (*a > *b) {
			add(*b);
			++b;
		}
		else {
			add(*a);
			++a, ++b;
		}
	}
	auto append = [&add](auto start, auto end) {
		while (start != end) {
			add(*start);
			++start;
		}
	};
	if (a != end(A))
		append(a, end(A));
	else if (b != end(B))
		append(b, end(B));
	return result;
}

// return true if string has all unique char
bool unique_char(string str) {
	vector<char> count(128, 0);
	for (char c : str) {
		if (count[c] > 0)
			return false;
		count[c]++;
	}
	return true;
}

#include "..\catch.hpp"
TEST_CASE("Trading Technology sequence number arbitration test", "[TEST]")
{
	CHECK(arbitrate(vector<int>{1,3,5}, vector<int>{ 2, 3, 6, 8}) == vector<int>{1,2,3,5,6,8});
	CHECK(arbitrate(vector<int>{1}, vector<int>{}) == vector<int>{1});
}

TEST_CASE("Trading Technology unique char test", "[TEST]")
{
	CHECK(unique_char("a") == true);
	CHECK(unique_char("abcdefghijklmnopqrstu1234A") == true);
	CHECK(unique_char("abcdefghijklmnopqrstu1234Azi") == false);
}