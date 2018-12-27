#include "..\catch.hpp"
#include "large_int_basic.h"
#include "large_int.h"
#include <chrono>
using namespace std;

TEST_CASE("larget int math", "[NEW]")
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
	 auto yy = multiply_fast(y, y);
	 CHECK((y*y).get() == yy.get());
	 yy -= yy;
	 yy.trim0();
	 CHECK(yy.get() == "0");
	 y -= big;
	 CHECK(y.get() == "99000001000");
}

string fibonacciModified(int t1, int t2, int n) {
	const static int SIZE = 12000;
	LargeInt lt1(t1, SIZE);
	LargeInt lt2(t2, SIZE);
	while (n-- > 2) {
		lt1 += lt2*lt2;
		lt1.swap(lt2);
		//cout << lt2.get().size() << " vec size\n";
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


TEST_CASE("large int fibonacci", "[NEW]")
{
	CHECK(fibonacciModified(0, 1, 6) == "27");

	CHECK(fibonacciModified(0, 1, 10) == "84266613096281243382112");
	//fibonacciModified(2, 2, 20);
	//CHECK(fibonacciModified_fast(0, 1, 10) == "84266613096281243382112");
	//CHECK(fibonacciModified_fast(0, 1, 20) == fibonacciModified(0, 1, 20));
	/*
	{
		auto start = chrono::high_resolution_clock::now();
		auto fast = fibonacciModified_fast(0, 1, 19);
		auto end = chrono::high_resolution_clock::now();
		auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
		std::cout << " fibonacciModified_fast " << nanos.count() << "\n";  // 188 ms
	}
	{
		auto start = chrono::high_resolution_clock::now();
		auto slow = fibonacciModified(0, 1, 19);
		auto end = chrono::high_resolution_clock::now();
		auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
		std::cout << " fibonacciModified slow " << nanos.count() << "\n";  // 692 ms
	}
	*/
	//auto t1=fibonacciModified_fast(2, 2, 20);
	//CHECK(t1.size()==46952);
	//CHECK(fibonacciModified_fast(1, 1, 20) == t1);
	//auto t2 = fibonacciModified(2, 2, 20);
	//cout << t2.size() << "\n";
}

TEST_CASE("large int with large scale", "[NEW]")
{
	SECTION("large in using scale 1000000000. multiply 999999") {
		vector<char> char_scale;
		vector<int>  bln_scale{ 999999 };
		large_int_fill(char_scale, 999999);
		int factor = 999999;
		for (int i = 0; i < 10; i++)
		{
			large_int_multiply(bln_scale, factor, 1000000000);
			large_int_multiply(char_scale, factor);
			auto bln_2_char = scale_down(bln_scale, 9);
			CHECK(char_scale == bln_2_char);
		}
	}
	SECTION("large in using scale 1000000000. multiply 999999999") {
		int factor = 999999999;
		vector<char> char_scale;
		vector<int>  bln_scale{ factor };
		large_int_fill(char_scale, factor);
		for (int i = 0; i < 10; i++)
		{
			large_int_multiply(bln_scale, factor, 1000000000);
			large_int_multiply(char_scale, factor);
			auto bln_2_char = scale_down(bln_scale, 9);
			CHECK(char_scale == bln_2_char);
		}
	}
	SECTION("large in using scale 1000000000. multiply 2000000001") {
		int factor = 2000000001;
		vector<char> char_scale;
		vector<int>  bln_scale{ factor };
		large_int_fill(char_scale, factor);
		for (int i = 0; i < 10; i++)
		{
			large_int_multiply(bln_scale, factor, 1000000000);
			large_int_multiply(char_scale, factor);
			auto bln_2_char = scale_down(bln_scale, 9);
			CHECK(char_scale == bln_2_char);
		}
	}

	SECTION("large in using scale 1000000000. add 2000000001") {
		int factor = 2000000001;
		vector<char> char_scale;
		vector<int>  bln_scale{ factor };
		large_int_fill(char_scale, factor);
		for (int i = 0; i < 10; i++)
		{
			sum(bln_scale, bln_scale, 1000000000);
			sum(char_scale, char_scale);
			auto bln_2_char = scale_down(bln_scale, 9);
			CHECK(char_scale == bln_2_char);
		}
	}
	SECTION("large in using scale 1000000000. add 999999999") {
		int factor = 999999999;
		vector<char> char_scale;
		vector<int>  bln_scale{ factor };
		large_int_fill(char_scale, factor);
		for (int i = 0; i < 10; i++)
		{
			sum(bln_scale, bln_scale, 1000000000);
			sum(char_scale, char_scale);
			auto bln_2_char = scale_down(bln_scale, 9);
			CHECK(char_scale == bln_2_char);
		}
	}
}