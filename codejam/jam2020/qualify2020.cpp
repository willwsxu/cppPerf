#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <numeric>
#include <iterator>
using namespace std;

// 2020 qualifying round
tuple<int,int,int> latin_square(vector<vector<int>> matrix)
{
	int N = matrix.size();
	int trace = 0;
	int r = 0, c = 0;
	static vector<int> repeat(N, 0);
	for (int i = 0; i < N; i++) {
		trace += matrix[i][i];
		fill(begin(repeat), end(repeat), 0);
		for (int j = 0; j < N; j++) {
			if (repeat[matrix[i][j] - 1] > 0) {
				r++;
				break;
			}
			repeat[matrix[i][j] - 1] = 1;
		}
		fill(begin(repeat), end(repeat), 0);
		for (int j = 0; j < N; j++) {
			const int idx = matrix[j][i]-1;
			if (repeat[idx] > 0) {
				c++;
				break;
			}
			repeat[idx] = 1;
		}
	}
	return { trace, r,c };
}

int test_onlineQ1()
{
	int T;
	cin >> T;
	cin.ignore();
	for (int t = 1; t <= T; t++) {
		int N;
		cin >> N;
		cin.ignore();
		vector<vector<int>> M(N, vector<int>(N, 0));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++)
				cin >> M[i][j];
		}
		cout << "Case #" << t;
		auto x = latin_square(std::move(M));
		cout << ": " << get<0>(x) << " " << get<1>(x) << " " << get<2>(x) << "\n";
	}
	return 0;
}


string nesting_depth(string digits) {
	string paren;
	paren.reserve(digits.size() * 2);
	int open_paren = 0;
	for (char c : digits) {
		int num = c - '0';
		if (num > open_paren) {
			paren.append(num - open_paren, '(');
		} else if (num < open_paren) {
			paren.append(open_paren - num, ')');
		}
		open_paren = num;
		paren.append(1, c);
	}
	paren.append(open_paren, ')');
	return paren;
}

int test_onlineQ2()
{
	int T;
	cin >> T;
	cin.ignore();
	for (int t = 1; t <= T; t++) {
		string s;
		std::getline(cin, s);
		cout << "Case #" << t;
		cout << ": " << nesting_depth(s) << "\n";
	}
	return 0;
}

string parent_schedule(vector<pair<int,int>> sched)
{
	static const int MAX_SIZE = 24 * 60 + 1;
	vector<int> slots(MAX_SIZE, 0);
	for (const auto& interval : sched) {
		slots[interval.first] += 1;  // enter activity
		slots[interval.second] += -1;  // leaving activity
	}
	partial_sum(begin(slots), end(slots), begin(slots));
	if (*max_element(begin(slots), end(slots)) > 2)  // some interval overlaps more than 2, so 2 people can not cover
		return {"IMPOSSIBLE"};
	static const char symbol[2] = { 'C', 'J' };
	int activity_count[2] = { 0,0 };
	int end_time[2] = { 0,0 };  // resolve schedule overlap
	map<pair<int, int>, vector<int>> interval_position; // need to save position before sorting, as result depend on original interval order
	for (int i = 0; i < sched.size(); i++) {
		interval_position[sched[i]].push_back(i);
	}
	string result(sched.size(), 'X');
	sort(begin(sched), end(sched), [](const auto& p1, const auto& p2) {
		return (p1.first == p2.first) ? p1.second < p2.second : p1.first < p2.first;
		});
	auto pick_shorter = [&activity_count]() { return activity_count[0] <= activity_count[1] ? 0 : 1; };
	for (int i = 0; i < sched.size(); i++) {
		int pick = pick_shorter();
		const int start_interval = sched[i].first;
		if (start_interval < end_time[0])
			pick = 1;
		else if (start_interval < end_time[1])
			pick = 0;
		auto& position = interval_position[sched[i]];
		result[position[0]] = symbol[pick];
		position.erase(position.begin());
		activity_count[pick]++;
		end_time[pick] = sched[i].second;
	}
	return result;
}

int test_onlineQ3()
{
	int T;
	cin >> T;
	cin.ignore();
	for (int t = 1; t <= T; t++) {
		int N;
		cin >> N;
		vector<pair<int,int>> sched(N);
		for (int i = 0; i < N; i++) {
			cin >> sched[i].first;
			cin >> sched[i].second;
		}
		cout << "Case #" << t;
		cout << ": " << parent_schedule(move(sched)) << "\n";
	}
	return 0;
}

vector<int> latin_square_candidate(int N, int K) {
	if (K<N || K>N * N)
		return {};
	if (K % N == 0)
		return vector<int>(N, K / N);  // all same at diagonal
	vector<int> ans;
	const int max_occur = N - 2;
	if (max_occur <= 0)
		return {};
	int selected = 0;
	while (ans.size()<N) {
		int min_sel = K / (N- ans.size());
	}
}

bool latin_square_brute(vector<vector<int>>& M, int N, int which, vector<int>& row_set, vector<int>& col_set)
{
	if (which == N * N - 1)
		return true;
	int r = which / N;
	int c = which % N;
	if (r == c)
		return latin_square_brute(M, N, which + 1, row_set, col_set);
	for (int i = 1; i <= N; i++) {
		if ((row_set[r] & (1 << i)) || (col_set[c] & (1 << i)))
			continue;
		row_set[r] |= (1 << i);
		col_set[c] |= (1 << i);
		M[r][c] = i;
		if (latin_square_brute(M, N, which+1, row_set, col_set))
			return true;
		row_set[r] ^= (1 << i);
		col_set[c] ^= (1 << i);
	}
	return false;
}
bool latin_square_brute_diag(vector<vector<int>>& M, int N, int K, int which, vector<int>& row_set, vector<int>& col_set)
{
	if (which == N)
		return K==0 && latin_square_brute(M, N, 1, row_set, col_set);
	if (K<N - which || K> N * (N - which))
		return false;
	for (int i = 1; i <= N; i++) {
		row_set[which] |= (1 << i);  // row which has value i
		col_set[which] |= (1 << i);  // col which has value i
		M[which][which] = i;
		if (latin_square_brute_diag(M, N, K-i, which+1, row_set, col_set))
			return true;
		row_set[which] ^= (1 << i);  // row which has value i
		col_set[which] ^= (1 << i);  // col which has value i
	}
	return false;
}
vector<vector<int>> latin_square(int N, int K)
{
	vector<vector<int>> vec(N, vector<int>(N, 0));
	vector<int> row_set(N, 0), col_set(N, 0);
	auto x = latin_square_brute_diag(vec, N, K, 0, row_set, col_set);
	return x ? move(vec) : vector<vector<int>>{};
}

int test_onlineQ5()
{
	int T;
	cin >> T;
	cin.ignore();
	for (int t = 1; t <= T; t++) {
		int N, K;
		cin >> N >> K;
		auto x = latin_square(N, K);
		cout << "Case #" << t;
		cout << ": " << (x.empty()?"IMPOSSIBLE":"POSSIBLE") << "\n";
		for (const auto& row : x) {
			copy(cbegin(row), cend(row), ostream_iterator<int>(cout, " "));
			cout << "\n";
		}
	}
	return 0;
}

#include "catch.hpp"

TEST_CASE("No 5. latin square", "[Q5]")
{
	SECTION("test 1: ") {
		auto x = latin_square(3, 6);
		CHECK(x == vector<vector<int>>{ {1, 3, 2}, { 3,2,1 }, { 2,1,3 }});
	}
	SECTION("test 2: ") {
		auto x = latin_square(2,3);
		CHECK(x == vector<vector<int>>{ });
		test_onlineQ5();
	}
}
TEST_CASE("No 3. parenting partnering returns", "[Q3]")
{
	SECTION("test 3: great test case") {
		auto x = parent_schedule(vector<pair<int, int>>{ {99, 150}, { 1, 100 }, { 100, 301 }, { 2,5 }, { 150, 250 }});
		CHECK(x == "JCCJJ");
	}
	SECTION("test 1: ") {
		auto x = parent_schedule(vector<pair<int,int>>{ {360, 480}, { 420, 540 }, { 600, 660 }});
		CHECK(x == "CJC");
	}
	SECTION("test 2: ") {
		auto x = parent_schedule(vector<pair<int, int>>{ {0, 1440}, { 1, 3 }, { 2, 4 }});
		CHECK(x == "IMPOSSIBLE");
	}
	SECTION("test 4: ") {
		auto x = parent_schedule(vector<pair<int, int>>{ {0, 720}, { 720, 1440 }});
		CHECK(x == "CJ");
		//test_onlineQ3();
	}
}

TEST_CASE("No 2. nesting depth", "[Q2]")
{
	CHECK(nesting_depth("0000") == "0000");
	CHECK(nesting_depth("101") == "(1)0(1)");
	CHECK(nesting_depth("111000") == "(111)000");
	CHECK(nesting_depth("4") == "((((4))))");
	CHECK(nesting_depth("221") == "((22)1)");
	CHECK(nesting_depth("021") == "0((2)1)");
	CHECK(nesting_depth("312") == "(((3))1(2))");
	//test_onlineQ2();
}

TEST_CASE("No 1. latin_square", "[Q1]")
{
	SECTION("test 1: ") {
		auto t = latin_square(vector<vector<int>>{ {1, 2, 3, 4}, { 2,1,4,3 }, { 3,4,1,2 }, { 4,3,2,1 }});
		CHECK(get<0>(t) == 4);
		CHECK(get<1>(t) == 0);
		CHECK(get<2>(t) == 0);
	}
	SECTION("test 2: ") {
		auto t = latin_square(vector<vector<int>>{ {2,2,2,2}, { 2,3,2,3 }, { 2,2,2,3 }, { 2,2,2,2 }});
		CHECK(get<0>(t) == 9);
		CHECK(get<1>(t) == 4);
		CHECK(get<2>(t) == 4);
	}
	SECTION("test 3: ") {
		auto t = latin_square(vector<vector<int>>{ {2,1,3}, { 1,3,2 }, { 1,2,3 }});
		CHECK(get<0>(t) == 8);
		CHECK(get<1>(t) == 0);
		CHECK(get<2>(t) == 2);
	}
	//test_onlineQ1();
}



