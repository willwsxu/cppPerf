#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


class InteractiveTest
{
	bool online_test = true;
	unsigned long long query_online(int d) {
		return 0;
	}
public:
	void set_tests(vector<int> t) {
		online_test = false;
	}
	unsigned long long query(int d) {
		if (online_test)
			return query_online(d);
		unsigned long long ans = 0;
		return ans;
	}
	static InteractiveTest& instance() {
		static InteractiveTest test;
		return test;
	}
};

void online2() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
	}
}

/*
int main(int argc, char *argv[])
{
	online2();
	return 0;
}
*/

#include "catch.hpp"
TEST_CASE("jam2019 1C #2", "[1C2]")
{
}