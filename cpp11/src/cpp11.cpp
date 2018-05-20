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

template <typename T>
class MyClass: public CRTP<MyClass<T>>
{

};
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "..\..\catch.hpp"

TEST_CASE("Variadic templates", "VARIA")
{
	Console c;
	hello(c, "Hello", "World","!","\n");

	CHECK((int)Sum::sum(1,3,5)==9); // @suppress("Invalid arguments")
	CHECK(static_cast<double>(Sum::sum(1,3,5.6))==9.6); // @suppress("Invalid arguments")
	CHECK(Sum::sum(10)==10);
	CHECK(static_cast<double>(Sum::avg(1,3,5.6))==Approx(3.2)); // @suppress("Invalid arguments")
	CHECK(static_cast<double>(::avg(1,3,5.6))==Approx(3.2)); // @suppress("Invalid arguments")

	arrayx<1,2,3,4,5> arr;
	REQUIRE( accumulate(begin(arr.arr), end(arr.arr),0) ==15 );;
}
TEST_CASE("CRTP", "CRTP")
{
	MyClass<int> obj1;
	MyClass<int> obj2(obj1);
	MyClass<double> obj3;
	CHECK(MyClass<int>::live()==2);
	REQUIRE(MyClass<double>::live()==1);
}
