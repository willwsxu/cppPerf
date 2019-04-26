#include <vector>
#include <numeric>
#include "library/helper.h"

using namespace std;

class GoptherTest
{
	int num_gophers;
	bool online_test = true;
	vector<int> query_online(vector<int>&& blades) {
		vector<int> result(blades.size(), 0);
		for (int b : blades)
			cout << b << " ";
		cout << endl;
		for (int& r : result) {
			cin >> r;
			if (r < 0)
			{
				exit(0);
			}
		}
		return result;
	}
public:
	void set_tests(int gophers) {
		num_gophers = gophers;
		online_test = false;
	}
	vector<int> query(vector<int>&& blades) {
		if (online_test)
			return query_online(move(blades));
		int n = static_cast<int>(blades.size());
		RandomHelper rand(0, n - 1);
		vector<int> result(n, 0);
		for (int i = 0; i < num_gophers; i++) {  // randomly distribute gophers to one of the 18 holes
			result[rand.getNext()]++;
		}
		for (int& mod : result)
			mod %= blades[0];
		return result;
	}
	static GoptherTest& instance() {
		static GoptherTest test;
		return test;
	}
};
int find_gophers() {
	vector<int> blades{ 5,7,9,11,13,16,17 };  // blade count each night, relative prime
	vector<int> gophers;
	for (int b : blades) {
		auto q = GoptherTest::instance().query(vector<int>(18, b));
		gophers.push_back(accumulate(begin(q), end(q), 0));
	}
	int max_gophers = 1000000;
	set<int> potential_count1, potential_count2;
	int lower = gophers[6];
	int upper = 0; // compute new bounds each round
	for (int count = lower; count <= max_gophers; count += 17) {  // start with last quest result
		potential_count1.insert(count);
		upper = max(upper, count);
	}
	for (int i = 5; i >= 0; i--) {  // find common number among all 7 queries
		max_gophers = upper;
		upper = 0;
		if (lower > gophers[i])
			lower = gophers[i] + (lower - gophers[i] + blades[i] - 1) / blades[i] * blades[i];
		else
			lower = gophers[i];
		for (int count = lower; count <= max_gophers; count += blades[i]) {
			if (potential_count1.count(count)) {
				potential_count2.insert(count);
				upper = max(upper, count);
			}
		}
		potential_count1.swap(potential_count2);
		potential_count2.clear();
		lower = *begin(potential_count1);
	}
	return lower;  // should only have 1 item left in set
}

void online2()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N, M;
		cin >> N >> M;
		cin.ignore();
		cout << find_gophers() << endl;
		int result;
		cin >> result;
		if (result < 0) {  // wrong answer
			exit(0);
		}
	}
}

#include "catch.hpp"

TEST_CASE("jam2019 1A #2", "[1A2]")
{
	online2();
	GoptherTest::instance().set_tests(100);
	CHECK(find_gophers() == 100);
	GoptherTest::instance().set_tests(1000000);
	CHECK(find_gophers() == 1000000);
}