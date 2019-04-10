#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>

using namespace std;
// 2019 qualifying round
// Editorial idea: flip each move
string go_your_own_way(string lydia)
{
	int last = lydia.size() - 1;
	int N = lydia.size() / 2 + 1;

	//easy cases
	if (lydia[0] == 'E' && lydia[last] == 'S') {
		string ans(N - 1, 'S');
		ans.append(N - 1, 'E');
		return ans;
	}
	if (lydia[0] == 'S' && lydia[last] == 'E') {
		string ans(N - 1, 'E');
		ans.append(N - 1, 'S');
		return ans;
	}
	auto same_last_move = [&lydia, N](char end_move, char other) {
		int count = 0;
		int prev = 0;
		for (char c : lydia) {
			if (c == other) {
				if (c == prev)
					break;
				count++;
			}
			prev = c;
		}
		string ans(count, other);
		ans.append(N - 1, end_move).append(N - 1 - count, other);
		return ans;
	};
	if (lydia[0] == 'E' && lydia[last] == 'E') {  // must have 2 consecutive S moves
		return same_last_move('E', 'S');
	}
	return same_last_move('S', 'E');
}
void test_onlineQ2()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N;
		string s;
		cin >> N;
		cin.ignore();
		std::getline(cin, s);
		cout << "Case #" << t;
		cout << ": " << go_your_own_way(s) << "\n";
	}
}

// split N=A+B, N has at least one digit 4, A & B has no 4
pair<string, string> split_no_4(string N)
{
	string second;
	auto start = find(begin(N), end(N), '4');
	while (start != end(N)) {
		if (*start == '4') {
			*start = '2';
			second.append(1, '2');
		}
		else
			second.append(1, '0');
		++start;
	}
	return { N, second };
}
void test_onlineQ1()
{
	int T;
	cin >> T;
	cin.ignore();
	for (int t = 1; t <= T; t++) {
		string s;
		std::getline(cin, s);
		cout << "Case #" << t;
		auto x = split_no_4(s);
		cout << ": " << x.first << " " << x.second << "\n";
	}
}

template<typename T>
T gcd(T p, T q)  // P>=q
{
	return q == 0 ? p : gcd(q, p%q);
}
// C[i]=L[i] * L[i+1]
// same idea to solve large int problem using python
string cryptopangram(vector<long long> cipher, long long N)
{
	auto b = gcd(cipher[0], cipher[1]); // a*b=c0, b*c=c1
	vector<long long> pangram_cipher;
	pangram_cipher.push_back(cipher[0] / b);
	pangram_cipher.push_back(b);
	for (size_t i = 1; i < cipher.size(); i++) {
		b = cipher[i] / b;
		pangram_cipher.push_back(b);
	}
	set<long long> alphabet(begin(pangram_cipher), end(pangram_cipher));
	map<long long, char> mapping;
	char c = 'A';
	for (auto a : alphabet) {
		mapping[a] = c++;
	}
	string ans;
	ans.reserve(pangram_cipher.size());
	transform(begin(pangram_cipher), end(pangram_cipher), back_inserter(ans), [&mapping](auto p) { return mapping[p];});
	return ans;
}

// Given N bits, each processed by a work. B works are broken and return none
// You are allowed perform F tests and decide whcih B workers are broken
class Test
{
	set<int> broken;
	bool interactive = true;
	string query_interactive(const string& from)
	{
		cout << from << endl;
		string judge;
		getline(cin, judge);
		if (judge == "-1") {
			exit(0);
			return ""; // failed
		}
		return judge;
	}
public:
	void set_test(set<int> B) {
		broken = move(B);
		interactive = false;
	}
	string query_result(const string& from)
	{
		if (interactive)
			return query_interactive(from);
		string result;
		result.reserve(from.size() - broken.size());
		for (size_t i = 0; i < from.size(); i++) {
			if (broken.count(i) == 0) {
				result.push_back(from[i]);
			}
		}
		return result;
	}
	static Test& get_instance() {
		static Test test;
		return test;
	}
};
set<int> bits_search_interactive(int N, int B, int F)
{
	int probe_len = (1 << F);  // given F bit sequence, how many numbers it can represent
	vector<string> tests(F);   // design F tests to solve missing bits
	for (int i = 0; i < N; i += probe_len) {
		int len = min(N - i, probe_len);
		for (int j = 0; j < len; j++) {
			int val = j;
			for (int k = 0; k < F; k++) {  // conver val into bits, fill test string 1 bit at a time
				tests[k].push_back('0' + val % 2);
				val /= 2;
			}
		}
	}
	vector<string> answer;  // answer to test, after removing bits from broken worker
	answer.reserve(F);
	for (const string& s : tests) {
		answer.push_back(Test::get_instance().query_result(s));
	}
	vector<int> probe_seq;  // construct probing sequence number from bits
	for (int i = 0; i < N - B; i++) {
		int val = 0;  // bits in little endian
		for (int j = F - 1; j >= 0; j--)
			val = val * 2 + answer[j][i] - '0';
		probe_seq.push_back(val);
	}
	// inspect each probing sequence to find missing number, therefore the position of broken bits
	int repeat = 0;
	set<int> broken;
	auto find_missing = [probe_len,&probe_seq, &broken,&repeat](int start, int end, int bits_exp) {
		if (end - start < bits_exp) {  // some bits are missing
			vector<int> slots(bits_exp, 0);
			for (int j = start; j < end; j++)
				slots[probe_seq[j]] = 1;  // bits present
			for (int j = 0; j < bits_exp; j++) {  // !watch out for how long to iterate
				if (!slots[j])  // bits missing
					broken.insert(repeat*probe_len + j);
			}
		}
	};
	int start = 0;  // check missing number in each sequence
	for (int i = 1; i < N - B; i++) {
		if (probe_seq[i] < probe_seq[i - 1]) { // end of a sequence
			find_missing(start, i, probe_len);
			repeat++;
			start = i;
		}
	}
	find_missing(start, N-B, N- repeat* probe_len);
	return broken;
}
void interactive()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N, B, F;
		cin >> N >> B >> F;
		cin.ignore();
		auto result = bits_search_interactive(N, B, F);
		for (int b : result)
			cout << b << " ";
		cout << endl;
		string judge;
		getline(cin, judge);
		if (judge == "-1")
			break;
	}
}
#include "catch.hpp"
TEST_CASE("No 4 missing bits", "[Q4]")
{
	//interactive();
	set<int> small{ 2,3,5 };
	Test::get_instance().set_test(small);
	CHECK(bits_search_interactive(6, small.size(), 5) == small);
	CHECK(bits_search_interactive(1024, small.size(), 10) == small);
	CHECK(bits_search_interactive(1024, small.size(), 5) == small);

	set<int> medium{ 2,3,5,200, 300,500, 700, 800, 900, 1000, 1023 };
	Test::get_instance().set_test(medium);
	CHECK(bits_search_interactive(1024, medium.size(), 10) == medium);
	CHECK(bits_search_interactive(1024, medium.size(), 5) == medium);
}
TEST_CASE("No 3. cryptopangram", "[Q3]")
{
	CHECK(cryptopangram(vector<long long>{3292937, 175597, 18779, 50429, 375469, 1651121, 2102, 3722, 2376497, 611683, 489059, 2328901, 3150061, 829981, 421301, 76409, 38477, 291931, 730241, 959821, 1664197, 3057407, 4267589, 4729181, 5335543}, 10000) == "SUBDERMATOGLYPHICFJKNQVWXZ");
	CHECK(cryptopangram(vector<long long>{217, 1891, 4819, 2291, 2987, 3811, 1739, 2491, 4717, 445, 65, 1079, 8383, 5353, 901, 187, 649, 1003, 697, 3239, 7663, 291, 123, 779, 1007, 3551, 1943, 2117, 1679, 989, 3053}, 103) == "CJQUIZKNOWBEVYOFDPFLUXALGORITHMS");
	//test_onlineQ3();
}

TEST_CASE("No 1. forgone solution", "[Q1]")
{
	CHECK(split_no_4("4") == pair<string, string>{"2", "2"});
	CHECK(split_no_4("940") == pair<string, string>{"920", "20"});
	//test_onlineQ1();
}
TEST_CASE("No 2. go_your_own_way", "[Q2]")
{
	CHECK(go_your_own_way("SE") == "ES");
	CHECK(go_your_own_way("ES") == "SE");
	CHECK(go_your_own_way("EESSSESE") == "SEEEESSS");
	CHECK(go_your_own_way("SEES") == "ESSE");
	//test_onlineQ2();
}
