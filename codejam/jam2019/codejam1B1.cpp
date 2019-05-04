#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// in a Q by Q grid of streets, people are all walking toward to tasty food cart
// given initial location ( they can only walk on grid lines) and direction
// find the location of food cart, (0,0) is at southwest corner
// A person is moving toward a street intersection if their current movement 
//  is on a shortest path to that street intersection within the Manhattan grid
// when there is tie, it is more likely closer to origin (smallest x or y)
pair<int,int> find_food_cart(vector<int> x, vector<int> y, vector<char> d, int P, int Q)
{
	vector<int> x_line(Q+1, 0);
	vector<int> y_line(Q + 1, 0);
	for (int i = 0; i < P; i++) {
		switch (d[i]) {
		case 'W':  // cart is at west of x[i]
			x_line[0]++;   // this person could at any x from 0 to x[i]-1
			x_line[x[i]]--;// trick is add delta +1 at begining of interval, -1 at 1 past end of interval
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
	for (int i = 1; i <= Q; i++) {  // calculate count from delta values
		x_line[i] += x_line[i - 1];
		y_line[i] += y_line[i - 1];
	}
	auto max_x = max_element(begin(x_line), end(x_line));  // max at smallest x
	auto max_y = max_element(begin(y_line), end(y_line));  // max at smallest y
	return {distance(begin(x_line), max_x), distance(begin(y_line), max_y) };
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
