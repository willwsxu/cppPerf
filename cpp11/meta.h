#pragma once


template <int p>
struct Power
{
	enum { pow = Power<p - 1>::pow * 2 };
};

template <>   // specialization, must be defined after general template
struct Power<0> {
	enum { pow = 1 };
};

template <int X, int p>
struct Power2  // can not overload clas name Power
{
	enum { pow = Power2<X, p - 1>::pow * X };
};

template <int X>   // specialization, must be defined after general template
struct Power2<X, 0> {
	enum { pow = 1 };
};

template<bool b, typename T, typename F> class IF
{

};

template <int N, bool odd=N%2>
struct Power3  // can not overload class name Power
{
	double operator()(double x) {
		return Power3<N/2>()(x) * Power3<N / 2>()(x);
	}
};

template <int N>
struct Power3<N, true>  // specialize odd power
{
	double operator()(double x) {
		return x *Power3<(N-1) / 2>()(x) *Power3<(N - 1) / 2>()(x);
	}
};

template <>   // specialization, must be defined after general template
struct Power3<0, false> {
	double operator()(double x) {
		return 1;
	}
};

template <>   // specialization, must be defined after general template
struct Power3<1, true> {
	double operator()(double x) {
		return x;
	}
};
// constexpr template function, cannot specialize template function
template <class T>
constexpr T pow(const T base, unsigned exp) {  // must use recursion, one line restriction in c+=11
	return exp == 0 ? 1 : base * pow(base, exp - 1);
}

template <class T>
constexpr T pow2(const T base, unsigned exp) { // bisection method, faster
	return exp == 0 ? 1 :
		exp % 2 == 0 ? pow(base, exp / 2) * pow(base, exp / 2) :
		base * pow(base, (exp - 1) / 2) * pow(base, (exp - 1) / 2);
}
