/*
 * cpp11.h
 *
 *  Created on: May 14, 2018
 *      Author: willw
 */

#ifndef CPP11_H_
#define CPP11_H_

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
	static T sum(T t)
	{
		return t;
	}
	template <typename T, typename ...TArgs>
	static auto sum(T t, TArgs...args) -> decltype(t+sum(args...)) // recursive arguments only works as member function
	{
		return t+sum(args...);
	}

	template <typename ...TArgs>
	static auto avg(TArgs...args) -> decltype(sum(args...))  // decltype(avg(args...)) fails
	{
		return sum(args...)/sizeof...(args);
	}
};

template <typename ...TArgs>
auto avg(TArgs...args) -> decltype(Sum::sum(args...))  // decltype(avg(args...)) fails
{
	return Sum::sum(args...)/sizeof...(args);
}
#endif /* CPP11_H_ */
