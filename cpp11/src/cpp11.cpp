//============================================================================
// Name        : cpp11.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "cpp11.h"
#include <iostream>
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



int main() {
	Console c;
	hello(c, "Hello", "World");
	hello(c, "Hello", "World");
	return 0;
}
