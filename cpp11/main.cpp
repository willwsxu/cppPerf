#include "dynBuffer.h"
#include "DynMsg.h"
#include <iostream>
using namespace std;

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
	CDynMsg<Console, char> buf;
	SECTION("basic") {
		buf.AppendData("Hello");
		REQUIRE(buf.GetBufferLen() == 5);
	}
	SECTION("copy") {
		CDynMsg<Console, char> buf2 = buf;
		REQUIRE(buf2.GetBufferLen() == 5);
	}
}