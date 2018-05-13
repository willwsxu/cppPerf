#include "dynBuffer.h"
#include "DynMsg.h"
#include <iostream>
#include <vector>
using namespace std;

#include "matrix2D.h"

typedef Matrix2D<int, vector> MatrixVii;
typedef Matrix2D<int, vector>::ColIterator vii_col_iter;

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "..\catch.hpp"

void tprintf(const char *fmt) {
	cout << fmt << endl;
}
template <typename T, typename ... Targs>
void tprintf(const char *fmt, T val, Targs ...Fargs)
{
	while (*fmt) {
		if (*fmt == '%') {
			cout << val;
			tprintf(fmt+2, Fargs...); // expand parameter pack
			return;  // must return to avoid repeat
		}
		else
			cout << *fmt++;
	}
}
class Console
{
public:
	template<typename... Targs>
	void operator()(const char *fmt, Targs...Fargs)
	{
		tprintf(fmt, Fargs...);
	}
	template<typename... Targs>
	void operator()(int sev, const char *fmt, Targs...Fargs)
	{
		cout << "WARN ";
		tprintf(fmt, Fargs...);
	}
};

template<typename ... Targs,typename=void, typename U>
void testPattern(U fmt, Targs...targs)
{
	tprintf(fmt, targs ...);
}
template<typename ... Targs, typename = void, typename U>
void testPattern2(U fmt, Targs...targs)
{
	testPattern(fmt, &targs ...);
}

TEST_CASE("DynBuffer", "BUF")
{
	Console c;
	c(1, "[%p] DynBuffer (from [%p]) resize to max allowed %d", &c, 10, 20);

	int i = 1;
	float f = 2.0f;
	testPattern("%f %f", i, f);
	testPattern2("%f %f", i, f);

	CDynBuffer<Console,char> buf;
	buf.AppendData("Hello");
	REQUIRE(buf.GetBufferLen() == 5);
}

TEST_CASE("DynMsg", "DMSG")
{
	SECTION("basic") {
		CDynMsg<Console, char> buf;
		buf.AppendData("Hello");
		REQUIRE(buf.GetBufferLen() == 5);
	}
	SECTION("copy") {
		CDynMsg<Console, char> buf;
		buf.AddMsg("Hello!", 6);
		CDynMsg<Console, char> buf2 = buf;
		CHECK(buf.GetBufferLen() == 10);
		CHECK(buf2.GetBufferLen() == 10);
		REQUIRE(buf2.GetNumMsgs() == 1);
	}
}


TEST_CASE("Matrix", "MATRIX")
{
	MatrixVii vii;
	vii.emplace_back(vector<int>{2, 4, 6});
	vii.emplace_back(vector<int>{1, 3, 7});
	auto it = vii.begin(1);
	++it;
	--it;
	it += 1;
	it -= 1;
	auto z=it + 1;
	auto y=it - 1;
	auto end = vii.end(1);
	auto size = end - it;
	vii_col_iter zz = z;
	//zz = z;
	auto c=count(vii.begin(1), end, 1);
	CHECK(c == 0);
	CHECK(count(vii.begin(1), end, 3)==1);
	//sort(vii.begin(1), vii.end(1));
	auto x= lower_bound(vii.begin(2), vii.end(2), 6);
	REQUIRE(*x == 6);
}