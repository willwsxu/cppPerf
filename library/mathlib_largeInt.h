#pragma once

#include "large_int_basic.h"

using std::vector;

class Power_Large  // compute power in O(log(N)), keep only k digits, K <=19
{
public:
	static std::pair<long long, int> compute_2(long long base, int power, int digits) {
		vector<char> large_int;
		larget_int_fill(large_int, base);
		vector<long long> bases;
		while (power>1) {
			if (power % 2>0)
				bases.push_back(base);
			large_int_multiply(large_int, base);
			if ((int)large_int.size()>digits)
				large_int.resize(digits);
			base = large_int_get<long long>(large_int);
			power /= 2;
		}
		for (long long b : bases) {
			large_int_multiply(large_int, b);
			if ((int)large_int.size()>digits)
				large_int.resize(digits);
		}
		return { large_int_get<long long>(large_int), large_int.size() };
	}
	static long long compute(long long base, int power, int digits) {
		return compute_2(base, power, digits).first;
	}
};


class Fibonacci_Large
{
	vector<char>	F1, F2;
public:
	Fibonacci_Large(int f1, int f2, int capacity = 1000) {
		F1.reserve(capacity);
		F2.reserve(capacity);
		larget_int_fill(F1, f1);
		larget_int_fill(F2, f2);
	}
	const vector<char>& get() const {
		return F2;
	}
	const vector<char>& compute_next() {
		sum(F1, F2); // add f2 to f1
		swap(F1, F2);
		return F2;
	}
};
