//============================================================================
// Name        : cpp11.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "cpp11.h"
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

struct Console
{
	template <typename T> void operator()(T t) {
		cout << t << " ";
	}
	void operator()() { // cannot be static
		cout << endl;
	}
};

template <size_t... args>
struct arrayx
{
	enum {N=sizeof...(args)};
	size_t arr[N]={args...};
};

int main() {
	Console c;
	hello(c, "Hello", "World");
	hello(c, "Hello", "World");

	auto x=Sum::sum(1,3,5);
	c(x);
	c(Sum::sum(1,3,5.6));
	c(Sum::sum(10));
	c(Sum::avg(1,3,5.6));
	c(::avg(1,3,5.6));

	arrayx<1,2,3,4,5> arr;
	cout << accumulate(begin(arr.arr), end(arr.arr),0);
	return 0;
}
