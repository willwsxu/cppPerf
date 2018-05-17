/*
 * cpp11.h
 *
 *  Created on: May 14, 2018
 *      Author: willw
 */

#ifndef CPP11_H_
#define CPP11_H_
#include <cstddef>

template <typename Logger, typename T>
void hello(Logger log, T t) {
	log(t);
	log();
}

template <typename Logger, typename T, typename...TArgs>
void hello(Logger log, T t, TArgs...args) {
	log(t);
	hello(log, args...);
}

struct Sum
{
	template <typename T>
	static auto sum(T t)
	{
		return t;
	}

	// decltype for return is needed in c++1, but not in c++14
	template <typename T, typename ...TArgs>
	static auto sum(T t, TArgs...args)// -> decltype(t+sum(args...)) // recursive arguments only works as member function
	{
		return t+sum(args...);
	}

	template <typename ...TArgs>
	static auto avg(TArgs...args)// -> decltype(sum(args...))  // decltype(avg(args...)) fails
	{
		return sum(args...)/sizeof...(args);
	}
};

template <typename ...TArgs>
auto avg(TArgs...args) -> decltype(Sum::sum(args...))  // decltype(avg(args...)) fails
{
	return Sum::sum(args...)/sizeof...(args);
}

//template <typename T>
//constexpr
template <typename Derived> class CRTP  // curiously recurring template pattern
{
protected:
	static size_t count;
	CRTP()
	{
		++CRTP::count;
	}
	CRTP(const CRTP&)
	{
		++CRTP::count;
	}
	~CRTP()
	{
		--CRTP::count;
	}
public:
	static size_t live()
	{
		return CRTP::count;
	}
};
template <typename Derived>
size_t CRTP<Derived>::count=0;


#endif /* CPP11_H_ */
