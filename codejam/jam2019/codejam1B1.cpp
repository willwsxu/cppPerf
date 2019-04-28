#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

pair<int,int> find_food_cart(vector<int> x, vector<int> y, vector<char> d, int P, int Q)
{
	vector<int> x_line(Q+1, 0);
	vector<int> y_line(Q + 1, 0);
	for (int i = 0; i < P; i++) {
		switch (d[i]) {
		case 'W':
			x_line[0]++;
			x_line[x[i]]--;
			break;
		case 'E':
			x_line[x[i]+1]++;
			break;
		case 'S':
			y_line[0]++;
			y_line[y[i]]--;
			break;
		case 'N':
			y_line[y[i]+1]++;
			break;
		}
	}
	for (int i = 1; i <= Q; i++) {
		x_line[i] += x_line[i - 1];
		y_line[i] += y_line[i - 1];
	}
	int max_people = 0;
	pair<int, int> ans;
	for (int i = 0; i <= Q; i++) {
		for (int j = 0; j <= Q; j++) {
			int total = x_line[i] + y_line[j];
			if (total > max_people) {
				max_people = total;
				ans = { i, j };
			}
			else if (total == max_people) {
				if (i < ans.first || j < ans.second)
					ans = { i, j };
			}
		}
	}
	return ans;
}

void online1() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int P, Q;
		cin >> P >> Q;
		vector<int> X, Y;
		vector<char> D;
		X.reserve(P);
		Y.reserve(P);
		D.reserve(P);
		for (int i = 0; i < P; i++) {
			int x, y;
			char d;
			cin >> x >> y >> d;
			X.push_back(x);
			Y.push_back(y);
			D.push_back(d);
		}
		cout << "Case #" << t;
		auto ans = find_food_cart(X, Y, D, P, Q);
		cout << ": " << ans.first << " " << ans.second << "\n";
	}
}
/*
int main(int argc, char *argv[])
{
	online1();
	return 0;
}
*/

#include "catch.hpp"
TEST_CASE("jam2019 1B #1", "[1B1]")
{
	auto t1 = find_food_cart(vector<int>{5}, vector<int>{5}, vector<char>{'N'}, 1, 10);
	CHECK(t1.first == 0);
	CHECK(t1.second == 6);

	auto t2 = find_food_cart(vector<int>{2,2,1,3}, vector<int>{4,6,5,5}, vector<char>{'N','S','E','W'}, 4, 10);
	CHECK(t2.first == 2);
	CHECK(t2.second == 5);

	auto t3 = find_food_cart(vector<int>{0,0,0,0,0,0,0,1}, vector<int>{2,3,3,4,5,5,8,5}, vector<char>{'S', 'N', 'N','N', 'S', 'S', 'S', 'W'}, 8, 10);
	CHECK(t3.first == 0);
	CHECK(t3.second == 4);
}
