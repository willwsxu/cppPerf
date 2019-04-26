#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <random>
#include <numeric>
#include "../../library/helper.h"
using namespace std;

template<typename T>
struct greater_size {
	bool operator()(const string&s1, const string& s2) { // sort by len, then lexico
		if (s1.size() == s2.size())
			return s1 < s2;  // so it allow set to store string of same length!!!
		return s1.size() > s2.size(); 
	};
};
// Given a list of unique words, find pair of words that rhyme (share same suffix), but not with any other pairs chosen
// e.g. ABC BBC CBC DBC are valid 2 pairs, one pair rhyme on BC, other pair rhyme on C
// ABC BBC CBC DBC EBC FBC only have 2 valid pairs. a third pair will conflict with other two pairs 
int pair_rhyme(vector<string>& words) {
	map<string, set<string>> rhymes;  // words group by same lenof suffix
	for (string& w : words) {
		rhymes[w.substr(w.size() - 1)].insert(move(w)); // group by last letter
	}
	auto erase_no_rhyme = [](map<string, set<string>>& rhy) {
		int count = 0;
		for (auto iter = begin(rhy); iter != end(rhy); ) {  //remove all word that share no suffix
			if (iter->second.size() < 2) {
				rhy.erase(iter++);
				count++;
			}
			else
				++iter;
		}
		return count;
	};
	erase_no_rhyme(rhymes);  // erased words with no rhymes at all
	size_t pos = 1;  //suffix len from back
	map<string, int> suffix_count;  // word count of same suffix
	while(!rhymes.empty()) {
		map<string, set<string>> rhymes2;
		pos++;  // increase suffix by one, each loop
		for (auto& p : rhymes) {
			int no_advance = 0;  // word cannot make longer suffix
			for (const string& w : p.second) {
				if (w.size() > pos)
					rhymes2[ w.substr(w.size() - pos) ].insert(w);
				else if (w.size() == pos)
					rhymes2[w].insert(w);
				else
					no_advance++;
			}
			no_advance += erase_no_rhyme(rhymes2);
			suffix_count[p.first] = no_advance;  // store suffix even if count is 0
		}
		rhymes.swap(rhymes2);
	}
	set<string, greater_size<string>> suffixes;  // sorted by len in reverse order
	for (const auto& p : suffix_count) {
		suffixes.insert(p.first);  // candidate to use multi index container
	}
	int pairs = 0;
	while (!suffixes.empty()) {
		// check longest suffix first, if count >=2, increment answer by 1, pass down left over values to suffix value of lesser len
		const string& longest_suf = *suffixes.begin();
		auto found = suffix_count.find(longest_suf);
		suffixes.erase(suffixes.begin());
		if (found == end(suffix_count))
			continue;
		const string& suf = found->first;
		int count = found->second;
		if (count > 1) {
			pairs++;  // a new pair at this long suffix
			count -= 2;
		}
		if (count>0 && suf.size()>1) {  // roll leftover value to next suffix of len-1, don't process
			string s = suf.substr(1);
			auto shorter = suffix_count.find(s);
			if (shorter != end(suffix_count)) {
				shorter->second += count;
			}
			else {  // not reachable due to all suffix len are added already
				suffix_count[s] = count;
				suffixes.insert(move(s)); // add the new suffix of this len
			}
		}
	}
	return pairs*2;
}

void online()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N;
		cin >> N;
		cin.ignore();
		vector<string> words;
		words.reserve(N);
		for (int i = 0; i < N; i++) {
			string s;
			getline(cin, s);
			words.push_back(move(s));
		}
		cout << "Case #" << t;
		cout << ": " << pair_rhyme(words) << "\n";
	}
}

// Given a grid RxC, find sequence of moves that visit each cell exactly once
// and consecutive moves don't share row, column, or diagonal
// if [r1,c1], [r2,c2] are two consecitive moves, then
// r1!=r2, c1!=c2, r1-c1!=r2-c2, r1+c1!=r2+c2
using vii = vector<pair<int, int>>;
bool dfs_path(int R, int C, vii& moves)
{
	if (moves.size() == R * C)
		return true;
	auto good_move = [&moves](int r, int c) {
		if (moves.empty())
			return true;
		int r2 = moves.back().first;
		int c2 = moves.back().second;
		if (r == r2 || c == c2 || r - c == r2 - c2 || r + c == r2 + c2)
			return false;

		return find(begin(moves), end(moves), pair<int, int>{r, c})==end(moves);
	};
	for (int r = 0; r < R; r++) {
		for (int c = 0; c < C; c++) {
			if (good_move(r, c)) {
				moves.emplace_back(r, c);
				if (dfs_path(R, C, moves))
					return true;
				moves.pop_back();
			}
		}
	}
	return false;
}

// My first successful application of randomization
// Compute all valid moves based on last move
// Randomly pick a move among all candidates, recursively try next move
// remove tried move from candidate list, and try again if last pick failed
// when grid is large, it takes one try at each move
bool dfs_path_random(int R, int C, vector<vector<int>>& moves, pair<int,int> last, int count, std::mt19937& engine)
{
	if (count == R * C)
		return true;
	auto good_move = [&moves, count, last](int r, int c) {  // rules of a valid move
		if (count==0)
			return true;
		if (r == last.first || c == last.second || r - c == last.first - last.second || r + c == last.first + last.second)
			return false;

		return moves[r][c]<0;
	};
	vii candidate_moves;
	for (int r = 0; r < R; r++) {
		for (int c = 0; c < C; c++) {
			if (good_move(r, c)) {
				candidate_moves.emplace_back(r, c);
			}
		}
	}
	//int random_tries = 0;
	while (!candidate_moves.empty()) {
		//random_tries++;
		uniform_int_distribution<> dis(0, candidate_moves.size()-1);
		int pick = dis(engine);
		int r = candidate_moves[pick].first;
		int c = candidate_moves[pick].second;
		moves[r][c] = count;
		if (dfs_path_random(R, C, moves, { r,c }, count + 1, engine)) {
			//cout << " random try " << random_tries << " at round " << count << "\n";
			return true;
		}
		moves[r][c] = -1;  // mark for not used
		candidate_moves.erase(begin(candidate_moves) + pick);
	}
	return false;
}

pair<bool, vii> jump_path(int R, int C)
{
	std::random_device rd;
	std::mt19937 engine(rd());
	vector<vector<int>> grid(R, vector<int>(C,-1));  // grid value is the the ordering, 0 based
	bool good = dfs_path_random(R, C, grid, {0,0}, 0, engine);  // using grid to spped up search, avoid TLE!!!
	if (!good)
		return { false, vii{} };
	vii moves(R*C);
	for (int r = 0; r < R; r++) {
		for (int c = 0; c < C; c++) {
			moves[grid[r][c]] = { r,c };
		}
	}
	return { true, moves };
}

void online1()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int R,C;
		cin >> R >> C;
		cin.ignore();
		auto result = jump_path(R, C);
		cout << "Case #" << t;
		cout << ": " << (result.first?"POSSIBLE":"IMPOSSIBLE") << "\n";
		if (result.first) {
			for (const auto& xx : result.second)
				cout << xx.first+1 << " " << xx.second+1 << "\n";
		}
	}
}

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
	void set_tests(int gophers){
		num_gophers = gophers;
		online_test = false;
	}
	vector<int> query(vector<int>&& blades) {
		if (online_test)
			return query_online(move(blades));
		int n = blades.size();
		RandomHelper rand(0, n-1);
		vector<int> result(n,0);
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

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

TEST_CASE("jam2019 1A #2", "[1A2]")
{
	online2();
	GoptherTest::instance().set_tests(100);
	CHECK(find_gophers() == 100);
	GoptherTest::instance().set_tests(1000000);
	CHECK(find_gophers() == 1000000);
}
TEST_CASE("jam2019 1A", "[1A1]")
{
	//online1();
	auto t1 = jump_path(2, 2);
	CHECK(t1.first == false);
	auto t2 = jump_path(2, 5);
	CHECK(t2.first == true);
	//CHECK(t2.second == vii{ {0,0},{1,2},{0,4},{1,1},{0,3},{1,0},{0,2},{1,4},{0,1},{1,3} });  fail due to randomness
	CHECK(t2.second.size() == 10);
	auto t3 = jump_path(20, 20);
	CHECK(t3.first == true);
}

TEST_CASE("jam2019 1C", "[1A3]")
{
	//online();
	CHECK(pair_rhyme(vector<string>{"1YXA", "2YXA", "3YXA", "1ZXA", "2ZXA", "3ZXA","1WXA","2WXA","3WXA","A"}) == 10);
	CHECK(pair_rhyme(vector<string>{"1YXA", "2YXA", "3YXA", "1ZXA", "2ZXA", "3ZXA"}) == 6);
	CHECK(pair_rhyme(vector<string>{"CODEJAM", "JAM", "HAM", "HUM"}) == 4);
	CHECK(pair_rhyme(vector<string>{"TARPOL","PROL"}) == 2);
	CHECK(pair_rhyme(vector<string>{"TARPOR", "PROL", "TARPRO"}) == 0);
	CHECK(pair_rhyme(vector<string>{"CODEJAM", "JAM","HAM","NALAM","HUM","NOLOM"}) == 6);
	CHECK(pair_rhyme(vector<string>{"PI", "HI","WI","FI"}) == 2);
}