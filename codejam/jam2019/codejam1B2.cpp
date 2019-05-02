#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

class RingTest
{
	bool online_test = true;
	vector<int> rings;
	static constexpr unsigned long long MOD = (1LL << 63);;
	unsigned long long query_online(int d) {
		cout << d << endl;
		unsigned long long result;
		cin >> result;
		return result;
	}
public:
	constexpr static unsigned long long pow2(int exp) {
		return 1LL << exp;
	}
	void set_tests(vector<int> t) {
		rings = move(t);
		online_test = false;
	}
	unsigned long long query(int d) {
		if (online_test)
			return query_online(d);
		unsigned long long ans = 0;
		for (int i = 1; i <= 6; i++) {
			int exp = d / i;
			if (exp<63)
				ans = (ans + pow2(exp) * rings[i - 1]) % MOD;
		}
		return ans;
	}
	static RingTest& instance() {
		static RingTest test;
		return test;
	}
};

// Brief description: There are 6 types of ring that double every 1 2,3,4,5,6 days
// Given unknown number of these rings at day 0, you can query find out total rings at any day between 1 and 500
// The answer will be modulo of 2^63
// Find the original number of rings at end of day 0, each ring is [0,100]
// You can ask 6 or 2 questions
// Observation: 1. any ring number will become 0 when doubles 63 times or more
//              2. At day d, total rings=sum(Ri * 2^(d/f). 
//                 Give max number is 100, if two neighboring numbers has exp diff >7, they will never overlap
vector<int> query_rings_W6()  // brute force, slow
{
	//on day 50, power 50, 25, 16, 12, 10, 8
	unsigned long long ans = RingTest::instance().query(50);
	vector<int> R(6, 0);
	R[0] = static_cast<int>(ans / RingTest::pow2(50));
	//on day 64, power 64, 32, 21, 16, 12, 10
	ans = RingTest::instance().query(64);
	R[1] = static_cast<int>(ans / RingTest::pow2(32));
	//on day 128, power 128, 64, 42, 32, 25, 21
	ans = RingTest::instance().query(128);
	R[2] = static_cast<int>(ans / RingTest::pow2(42));

	//on day 192, power 192, 96, 64, 48, 38, 32
	ans = RingTest::instance().query(192);
	R[3] = static_cast<int>(ans / RingTest::pow2(48));
	//on day 256, power 256, 128, 85, 64, 51, 42
	ans = RingTest::instance().query(256);
	R[4] = static_cast<int>(ans / RingTest::pow2(51));

	R[5] = static_cast<int>(ans % RingTest::pow2(51) / RingTest::pow2(42));
	return R;
}
// given max count is 100 for each ring type, (2^7)
// wee need to compute the valid days so each exponent separated by at least 7
int compute_valid_exp(int ring_days, int gap)  // group ring type into 1,2,3 and 4,5,6
{
	for (int i = 10; i <= 500; i++) {
		int exp2 = i / (ring_days + 1);
		int exp3 = i / (ring_days + 2);
		if (exp2 - exp3 >= gap)
			return i;
	}
	return 0;
}

vector<int> query_rings_W2()  // solve 3 numbers on each try by find the correct expoent so the values don't overlap
{
	vector<int> R(6, 0);
	// query 1:to compute ring count of period 4, 5, 6 (days to double)
	int d = compute_valid_exp(4, 8);  // minimum 8 in poew difference between numbers
	unsigned long long ans = RingTest::instance().query(d);
	int exp1 = d / 4;
	int exp2 = d / 5;
	int exp3 = d / 6;
	R[3] = static_cast<int>(ans / RingTest::pow2(exp1));
	R[4] = static_cast<int>(ans % RingTest::pow2(exp1) / RingTest::pow2(exp2));
	R[5] = static_cast<int>(ans % RingTest::pow2(exp2) / RingTest::pow2(exp3));

	// query 2:to compute ring count of period 1,2,3
	d = compute_valid_exp(1, 9); // minimum 9 in poew difference between numbers
	ans = RingTest::instance().query(d);
	ans = ans - R[3] * RingTest::pow2(d / 4) - R[4] * RingTest::pow2(d / 5) - R[5] * RingTest::pow2(d / 6);
	R[0] = static_cast<int>(ans / RingTest::pow2(d));
	R[1] = static_cast<int>(ans % RingTest::pow2(d) / RingTest::pow2(d/2));
	R[2] = static_cast<int>(ans % RingTest::pow2(d/2) / RingTest::pow2(d/3));
	return R;
}

void online2()
{
	int T, W;
	cin >> T >> W;
	for (int t = 1; t <= T; t++) {
		auto ring_count = query_rings_W2();
		copy(begin(ring_count), end(ring_count), ostream_iterator<int>(cout, " "));
		cout << endl;
		int result;
		cin >> result;
		cerr << "cpp test #" << t << ": success=" << result << " rings=";
		copy(begin(ring_count), end(ring_count), ostream_iterator<int>(cerr, " "));
		cerr << endl;
		if (result < 0) {  // wrong answer
			exit(0);
		}
	}
}
/*
int main(int argc, char* argv[])
{
	online2();
	return 0;
}
*/

#include "catch.hpp"

TEST_CASE("jam2019 1B #2", "[1B2]")
{
	vector<int> t1{ 97,3,7,11,23,57 };
	RingTest::instance().set_tests(t1);
	CHECK(query_rings_W2() == t1);
}
