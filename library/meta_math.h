#pragma once

template <size_t N> struct Factorial;
template<> struct Factorial<0> {
	static const int value = 1;
};
template <size_t N> struct Factorial
{
	static const int value = N*Factorial<N - 1>::value;
};
