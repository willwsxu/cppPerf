#include <vector>
#include <map>
#include <set>
#include <functional>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>

using namespace std;

int makeup_school(int num_school, vector<int> powers, vector<int>school, vector<int>chosen)
{
	// group powers per school
	vector<set<int, greater<int>>> pow_by_sch(num_school);
	for (int i = 0; i < (int)powers.size(); i++) {
		pow_by_sch[school[i] - 1].insert(powers[i]);
	}
	int makeup = 0;
	for (int c : chosen) {
		int sch = school[c - 1] - 1; // school id of chosen student id, adjust base to 0
		if (*begin(pow_by_sch[sch]) != c)
			makeup++;
	}
	return makeup;
}

// given n sweets of distinct size, each childern will get 2 sweets of same combined size
// find max such pairs. N [2,1000]
int count_sweets_pair_same_size(vector<int> sweets) {
	map<int, int> count;
	int max_pair = 0;
	for (int i = 0; i < (int)sweets.size() - 1; i++) {
		for (int j = i + 1; j < (int)sweets.size(); j++) {
			auto& xref = count[sweets[i] + sweets[j]];
			xref++;
			max_pair = max(max_pair, xref);
		}
	}
	return max_pair;
}
#include "..\catch.hpp"
TEST_CASE("code force B test", "[NEW]")
{
	CHECK(count_sweets_pair_same_size(vector<int>{1, 8, 3, 11, 4, 9, 2, 7}) == 3);
	CHECK(count_sweets_pair_same_size(vector<int>{3, 1, 7, 11, 9, 2, 12}) == 2);
}

TEST_CASE("code force A test", "[NEW]")
{
	CHECK(makeup_school(3, vector<int>{1, 5, 3, 4, 6, 7, 2}, vector<int>{ 1, 3, 1, 2, 1, 2, 3 }, vector<int>{3}) == 1);
	CHECK(makeup_school(4, vector<int>{1, 2, 3, 4, 5, 6, 7, 8}, vector<int>{ 4, 3, 2, 1, 4, 3, 2, 1 }, vector<int>{3, 4, 5, 6}) == 2);
}

template<typename value_type>
vector<value_type> read_ints(istream& is, int count)
{
	string line;
	getline(is, line);
	istringstream strm(line);
	return vector<value_type>( istream_iterator<value_type>(iss), istream_iterator<value_type>());
}

