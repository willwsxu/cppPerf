#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
long long fair_game_brute(const vector<int>& C, const vector<int>& D, int K)
{
	int N = C.size();
	long long total = 0;
	for (int R = 0; R < N; R++) {
		int high_C = C[R];
		int high_D = D[R];
		for (int L = R; L >= 0; L--) {
			high_C = max(high_C, C[L]);
			high_D = max(high_D, D[L]);
			if (abs(high_C - high_D) <= K)
				total++;
		}
	}
	return total;
}

void online3() {
	int T;
	cin >> T;
	auto read_ints = [](int N) {
		vector<int> v;
		v.reserve(N);
		for (int i = 0; i < N; i++) {
			int val;
			cin >> val;
			v.push_back(val);
		}
		return v;
	};
	for (int t = 1; t <= T; t++) {
		int N, K;
		cin >> N >> K;
		vector<int> C= read_ints(N);
		vector<int> D = read_ints(N);
		cout << "Case #" << t;
		auto ans = fair_game_brute(C, D, K);
		cout << ": " << ans << "\n";
	}
}
/*
int main(int argc, char* argv[])
{
	online3();
	return 0;
}
*/
#include "catch.hpp"
TEST_CASE("jam2019 1B #3", "[1B3]")
{
	CHECK(fair_game_brute(vector<int>{1, 1, 1, 8}, vector<int>{8, 8, 8, 8}, 0) == 4);
	CHECK(fair_game_brute(vector<int>{0,1,1}, vector<int>{1,1,0}, 0) == 4);
	CHECK(fair_game_brute(vector<int>{3}, vector<int>{3}, 0) == 1);
	CHECK(fair_game_brute(vector<int>{0,8,0,8,0}, vector<int>{4,0,4,0,4}, 0) == 0);
	CHECK(fair_game_brute(vector<int>{1, 0,0}, vector<int>{0,1,2}, 0) == 1);
	CHECK(fair_game_brute(vector<int>{1, 2,3,4,5}, vector<int>{5,5,5,5,10}, 2) == 7);
}
