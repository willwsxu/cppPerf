#include "dynBuffer.h"
#include <iostream>
using namespace std;

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "..\catch.hpp"

void tprintf(const char *fmt) {
	cout << fmt;
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

TEST_CASE("DynBuffer", "BUF")
{
	Console c;
	c(1, "[%p] DynBuffer (from [%p]) resize to max allowed %d", &c, 10, 20);

	CDynBuffer<Console,char> buf;
	buf.AppendData("Hello");
	REQUIRE(buf.GetBufferLen() == 5);
}