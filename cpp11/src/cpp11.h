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


#endif /* CPP11_H_ */
