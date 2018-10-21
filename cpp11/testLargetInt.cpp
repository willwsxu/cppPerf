#include "..\catch.hpp"
#include "large_int.h"
using namespace std;

TEST_CASE("larget int math", "NEW")
{
	LargeInt a(1234), b(9345);
	LargeInt c = a + b;
	CHECK(c.get() == "10579");

	LargeInt d = b * 2;
	CHECK(d.get() == "18690");

	LargeInt e = a * b;
	CHECK(e.get() == "11531730");
	LargeInt x(0);
	x = move(e);
	CHECK(x.get() == "11531730");

	LargeInt big(1000000000);
	auto p2 = big*big;
	auto p4 = p2*p2;
	auto p8 = p4*p4;
	auto p16 = p8*p8;
	auto p32 = p16*p16;
	string p144(145, '0');
	p144[0] = '1';
	CHECK(p16.get() == p144);
	string p288(289, '0');
	p288[0] = '1';
	CHECK(p32.get() == p288);

	 auto y=(big + LargeInt(10))* LargeInt(100);
	 CHECK((y*y).get() == multiply_fast(y, y).get());
	 y -= big;
	 CHECK(y.get() == "99000001000");
}

string fibonacciModified(int t1, int t2, int n) {
	const static int SIZE = 110000;
	LargeInt lt1(t1, SIZE);
	LargeInt lt2(t2, SIZE);
	LargeInt output(0, SIZE);
	LargeInt temp_copy(0, SIZE);
	while (n-- > 2) {
		multiply(output, temp_copy, lt2, lt2);
		output += lt1;
		lt1.swap(lt2);
		lt2.swap(output);
	}
	return lt2.get();
}


string fibonacciModified_fast(int t1, int t2, int n) {
	const static int SIZE = 110000;
	LargeInt lt1(t1, SIZE);
	LargeInt lt2(t2, SIZE);
	while (n-- > 2) {
		auto output = multiply_fast(lt2, lt2);
		output += lt1;
		lt1.swap(lt2);
		lt2.swap(output);
	}
	return lt2.get();
}


TEST_CASE("larget int fibonacci", "NEW")
{
	CHECK(fibonacciModified(0, 1, 6) == "27");

	CHECK(fibonacciModified(0, 1, 10) == "84266613096281243382112");
	CHECK(fibonacciModified_fast(0, 1, 10) == "84266613096281243382112");
	CHECK(fibonacciModified_fast(0, 1, 11) == fibonacciModified(0, 1, 11));
	//auto t1=fibonacciModified(1, 1, 20);
	//CHECK(t1.size()==46952);
	//CHECK(fibonacciModified_fast(1, 1, 20) == t1);
	//auto t2 = fibonacciModified(2, 2, 20);
	//cout << t2.size() << "\n";
}