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

template <typename T, int N, bool odd=N%2>
struct Power3  // can not overload class name Power
{
	T operator()(T x) {
		return Power3<T, N/2>()(x) * Power3<T, N / 2>()(x);
	}
};

template <typename T, int N>
struct Power3<T, N, true>  // specialize odd power
{
	T operator()(T x) {
		return x *Power3<T, (N-1) / 2>()(x) *Power3<T, (N - 1) / 2>()(x);
	}
};

template <typename T>   // specialization, must be defined after general template
struct Power3<T, 0, false> {
	T operator()(T x) {
		return 1;
	}
};

template <typename T>   // specialization, must be defined after general template
struct Power3<T, 1, true> {
	T operator()(T x) {
		return x;
	}
};
// constexpr template function, cannot specialize template function
template <class T>
constexpr T pow1(const T base, unsigned exp) {  // must use recursion, one line restriction in c+=11
	return exp == 0 ? 1 : base * pow1(base, exp - 1);
}

template <class T>
constexpr T pow2(const T base, unsigned exp) { // bisection method, faster
	return exp == 0 ? 1 :
		exp % 2 == 0 ? pow2(base, exp / 2) * pow2(base, exp / 2) :
		base * pow2(base, (exp - 1) / 2) * pow2(base, (exp - 1) / 2);
}
