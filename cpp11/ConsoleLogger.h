#pragma once
#include <iostream>

enum { SEV_INFO, SEV_WARN };

#define LOG_INFO __FILE__,__LINE__
#define LOG_WARN SEV_WARN,__FILE__,__LINE__


inline void tprintf(std::ostream& os, const char *fmt) {
	os << fmt;
}

// void tprintf(const char *fmt, T val, Targs ...Fargs, std::ostream& os = std::cout)  won't work, use overload
// default argument parameter should not precede none-default parameter
// Variadic template parameters should not precede non-variadic template parameters
// Defaulted template parameters should not precede non-defaulted template parameters

template <typename T, typename ... Targs>
void tprintf(std::ostream& os, const char *fmt, T val, Targs ...Fargs)
{
	while (*fmt) {
		if (*fmt == '%') {
			os << val;
			tprintf(os, fmt + 2, Fargs...); // expand parameter pack
			return;  // must return to avoid repeat
		}
		else
			os << *fmt++;
	}
}

// overloads to use cout
inline void tprintf(const char *fmt) {
	tprintf(std::cout, fmt);
}
template <typename T, typename ... Targs>
void tprintf(const char *fmt, T val, Targs ...Fargs)
{
	tprintf(std::cout, fmt, Fargs...);
}

class Console
{
public:
	template<typename... Targs>
	void operator()(const char *filename, int lineNo, const char *fmt, Targs...Fargs)  // info level logging
	{
		tprintf(fmt, Fargs...);
		cout << " (" << filename << ":" << lineNo << ")" << endl;
	}

	template<typename... Targs>
	void operator()(int sev, const char *filename, int lineNo, const char *fmt, Targs...Fargs)  // warning level logging
	{
		cout << "WARN ";
		tprintf(filename, lineNo, fmt, Fargs...);
	}
};