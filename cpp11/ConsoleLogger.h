#pragma once
#include <iostream>

enum { SEV_INFO, SEV_WARN };

#define LOG_INFO __FILE__,__LINE__
#define LOG_WARN SEV_WARN,__FILE__,__LINE__

inline void tprintf(const char *fmt) {
	std::cout << fmt;
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