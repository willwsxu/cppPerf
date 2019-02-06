#include "..\catch.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <iterator>
#include <sstream>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <cassert>

using namespace std;

int solution1(int N) {
	// write your code in C++14 (g++ 6.2.0)
	vector<int> digits;
	while (N > 0) {
		digits.push_back(N % 10);
		N /= 10;
	}
	long result=0;
	sort(begin(digits), end(digits), greater<int>());
	for (int d : digits) {
		result = result * 10 + d;
	}
	return result > 100000000 ? -1 : result;
}
TEST_CASE("Q1- large test", "[JUMP]")
{
	CHECK(solution1(0) == 0);
	CHECK(solution1(1) == 1);
	CHECK(solution1(70473) == 77430);
	CHECK(solution1(553) == 553);
	CHECK(solution1(213) == 321);
	CHECK(solution1(100000000) == 100000000);
	CHECK(solution1(100000001) == -1);
	CHECK(solution1(2147483647) == -1);
}


vector<string> get_tokens(const string& str, char sep = ' ') {
	vector<string> tokens;
	istringstream strm(str);
	for (string token; getline(strm, token, sep); ) {
		tokens.push_back(move(token));
	}
	return tokens;
}

class WordMachine
{
	vector<unsigned int> numbers;
	bool range_check(unsigned int x)
	{
		static const unsigned int MAX = 1 << 20;
		return x < MAX;
	}
public:
	bool pop() {
		if (numbers.empty())
			return false;
		numbers.pop_back();
		return true;
	}
	bool push(unsigned int x) {
		if (!range_check(x))
			return false;
		numbers.push_back(x);
		return true;
	}
	bool dup() {
		if (numbers.empty())
			return false;
		numbers.push_back(numbers.back());
		return true;
	}
	bool add() {
		if (numbers.size() < 2)
			return false;
		unsigned int x = numbers.back();
		numbers.pop_back();
		numbers.back() += x;
		return range_check(numbers.back());
	}
	bool subtract()
	{
		if (numbers.size() < 2)
			return false;
		unsigned int x = numbers.back();
		numbers.pop_back();
		numbers.back() = x- numbers.back();
		return range_check(numbers.back());
	}
	int top() const {
		if (numbers.empty())
			return -1;
		return numbers.back();
	}
};

int solution2(string &S) {
	// write your code in C++14 (g++ 6.2.0)
	auto tokens = get_tokens(S); 
	WordMachine machine;
	for (const auto& t : tokens) {
		if (t == "POP") {
			if (!machine.pop())
				return -1;
		}
		else if (t == "DUP") {
			if (!machine.dup())
				return -1;
		}
		else if (t == "+") {
			if (!machine.add())
				return -1;
		}
		else if (t == "-") {
			if (!machine.subtract())
				return -1;
		}
		else {
			if (!machine.push(stoi(t)))
				return -1;
		}
	}
	return machine.top();
}

TEST_CASE("Q2- unit test", "[JUMP]")
{
	WordMachine machine;
	CHECK(machine.top() == -1);
	machine.push(100);
	CHECK(machine.add() == false);
	CHECK(machine.subtract() == false);
	machine.dup();
	CHECK(machine.add() == true); //200
	machine.dup();
	CHECK(machine.subtract() == true); //0
	machine.push(100);
	CHECK(machine.subtract() == true); // 100
	machine.push(10);
	CHECK(machine.subtract() == false); // -90
	machine.push(50);
	CHECK(machine.add() == false); // -40
	machine.push((1 << 20) - 1);
	CHECK(machine.add() == true); //
	machine.push((1 << 20) - 1);
	CHECK(machine.add() == false); // overflow
}

TEST_CASE("Q2- test", "[JUMP]")
{
	CHECK(solution2(string("13 DUP 4 POP 5 DUP + DUP + -")) == 7);
	CHECK(solution2(string("5 6 + -")) == -1);
	CHECK(solution2(string("3 DUP 5 - -")) == -1);
	CHECK(solution2(string("")) == -1);
	CHECK(solution2(string("DUP")) == -1);
	CHECK(solution2(string("DUP 5")) == -1);
	CHECK(solution2(string("1000000 DUP")) == 1000000);
	CHECK(solution2(string("1000000 DUP +")) == -1);
	CHECK(solution2(string("POP")) == -1);
	CHECK(solution2(string("POP 5")) == -1);
	CHECK(solution2(string("10 POP")) == -1);
	CHECK(solution2(string("10 POP POP 5")) == -1);
	CHECK(solution2(string("+")) == -1);
	CHECK(solution2(string("1 +")) == -1);
	CHECK(solution2(string("-")) == -1);
	CHECK(solution2(string("1 -")) == -1);
	CHECK(solution2(string("10")) == 10);
	CHECK(solution2(string("1048575 1 +")) == -1);
	CHECK(solution2(string("1048575 0 +")) == 1048575);
	CHECK(solution2(string("1 0 -")) == -1);
	CHECK(solution2(string("0 1 -")) == 1);
	CHECK(solution2(string("0 1 - DUP POP")) == 1);
	CHECK(solution2(string("0 1 - DUP POP DUP POP")) == 1);
}

string solution3(int A, int B) {
	// write your code in C++14 (g++ 6.2.0)
	string decimal=to_string(A/B);
	A %= B;
	if (A == 0)
		return decimal;
	decimal.append(1, '.');
	vector<int> digits;
	std::map<int, int> remainders_pos{ { A, 0 } };  // track repeating remainder
	int pos = 1;
	while (A) {
		A *= 10;
		digits.push_back(A / B); // next decimal digit
		A %= B;  // new remainder
		auto found = remainders_pos.find(A);
		if (found != end(remainders_pos) ) {  // find repeating decimal, stop
			for (int i = 0; i < (int)digits.size(); i++) {
				if (i == found->second)
					decimal.append(1, '(');
				decimal.append(1, digits[i] + '0');
			}
			decimal.append(1, ')');
			return decimal;
		}
		remainders_pos[A] = pos++;
	}
	transform(begin(digits), end(digits), back_inserter(decimal), [](int d) { return d + '0'; });
	return decimal;
}

TEST_CASE("Q3- test", "[JUMP]")
{
	CHECK(solution3(3, 28) == "0.10(714285)");
	CHECK(solution3(1000000, 3) == "333333.(3)");
	CHECK(solution3(1000000, 1) == "1000000");
	CHECK(solution3(1000000, 2) == "500000");
	CHECK(solution3(0, 10) == "0");
	CHECK(solution3(1, 10) == "0.1");
	CHECK(solution3(1, 8) == "0.125");
	CHECK(solution3(1, 4096) == "0.000244140625");
	CHECK(solution3(10, 10) == "1");
	CHECK(solution3(1, 7) == "0.(142857)");
	CHECK(solution3(1, 777) == "0.(001287)");
	CHECK(solution3(900001, 777) == "1158.(302445)");
}

int solution4(int N) {
	// write your code in C++14 (g++ 6.2.0)
	if (N == 0)
		return 0;
	vector<int> binary;
	while (N > 0) {
		binary.push_back(N % 2);
		N /= 2;
	}
	int sparse = 0;
	for (int i = (int)binary.size() - 1; i >= 0; --i) {
		sparse *= 2;
		if (i % 2 == 0)  // pick only even digits
			sparse += binary[i];
	}
	return sparse;
}

TEST_CASE("Q4- test", "[JUMP]")
{
	CHECK(solution4(0) == 0);
	CHECK(solution4(1) == 1);
	CHECK(solution4(2) == 0);
	CHECK(solution4(26) == 16);
	CHECK(solution4(1000000000) == 286277632);
}