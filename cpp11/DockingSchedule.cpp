
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// giving a schedule of warehouse truck loading, per day
// find out how many docks are required
// e.g. 12:00-13:00, 13:00-14:00 1 dock, clarify that it is ok to start or stop at same time
//      12:00-13:01, 13:00-14:00 2 docks
string get_next(const string& t, size_t& start, char sep) {
	auto last = find(begin(t) + start, end(t), sep);
	size_t len = distance(begin(t) + start, last);
	string ans = t.substr(start, len);
	start += len;
	while (t[start] == sep)  // move start to next token
		start++;
	return ans;
}
// find maximum interval overlaps
int compute_docks(vector<string> schedule) {
	map<int, int> count;
	for (const auto& interval : schedule) {
		size_t start = 0;
		int h1 = stoi(get_next(interval, start, ':'));
		int m1 = stoi(get_next(interval, start, '-'));
		int h2 = stoi(get_next(interval, start, ':'));
		int m2 = stoi(get_next(interval, start, ':'));
		count[h1 * 60 + m1] += 1;
		count[h2 * 60 + m2] += -1;
	}
	int max_val = 0;
	int sum = 0;
	for(const auto& c : count) {
		sum += c.second;
		max_val = max(max_val, sum);
	}
	return max_val;
}

#include "..\catch.hpp"
TEST_CASE("GoHealth compute docks test", "[TEST]")
{
	CHECK(compute_docks({ "12:00-13:00" }) == 1);
	CHECK(compute_docks({ "12:00-13:00", "13:00-14:00" }) == 1);
	CHECK(compute_docks({ "12:00-13:01", "13:00-14:00" }) == 2);
	CHECK(compute_docks({ "12:00-13:00", "13:00-14:00","12:00-15:00" }) == 2);
	CHECK(compute_docks({ "12:00-13:01", "13:00-14:00","12:00-15:00" }) == 3);
}
