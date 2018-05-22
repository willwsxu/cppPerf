#pragma once

void tprintf(const char *fmt) {
	cout << fmt << endl;
}
template <typename T, typename ... Targs>
void tprintf(const char *fmt, T val, Targs ...Fargs)
{
	while (*fmt) {
		if (*fmt == '%') {
			cout << val;
			tprintf(fmt + 2, Fargs...); // expand parameter pack
			return;  // must return to avoid repeat
		}
		else
			cout << *fmt++;
	}
}
//tuple piecewise constructor, variadic, tie
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

template<typename ... Targs, typename = void, typename U>
void testPattern(U fmt, Targs...targs)
{
	tprintf(fmt, targs ...);
}
template<typename ... Targs, typename = void, typename U>
void testPattern2(U fmt, Targs...targs)
{
	testPattern(fmt, &targs ...);
}
