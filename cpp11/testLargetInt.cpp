#include "..\catch.hpp"
#include "large_int.h"
using namespace std;

TEST_CASE("larget int math", "NEW")
{
	LargeInt a(1234), b(9345);
	LargeInt c = a + b;
	CHECK(c.get() == vector<int>{1,0,5,7,9});

	LargeInt d = b * 2;
	CHECK(d.get() == vector<int>{1,8,6,9,0});

	LargeInt e = a * b;
	CHECK(e.get() == vector<int>{1,1,5,3,1,7,3,0});
	LargeInt x(0);
	x = move(e);
	CHECK(x.get() == vector<int>{1, 1, 5, 3, 1, 7, 3, 0});
}

vector<int> fibonacciModified(int t1, int t2, int n) {
	LargeInt lt(t2);
	LargeInt lt1(t1);
	LargeInt lt2(t2);
	while (n-- > 2) {
		lt = lt1 + lt2*lt2;
		lt1 = move(lt2);
		lt2 = move(lt);
	}
	return lt2.get();
}

TEST_CASE("larget int fibonacci", "NEW")
{
	vector<int> x = fibonacciModified(0, 1, 5);
	CHECK(x == vector<int>{5});
}