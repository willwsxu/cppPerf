#pragma once

template <int X, int p>  // X^p
struct PowerX  // can not overload clas name Power
{
	enum { pow = PowerX<X, p - 1>::pow * X };
};

template <int X>   // specialization, must be defined after general template
struct PowerX<X, 0> {
	enum { pow = 1 };
};

template <int p>
struct PowerOf2  // 2^p
{
	const static long pow = PowerX<2, p>::pow;
};
/*
template<class T>
inline constexpr T power4(const T x, integral_constant<T, 0>)
{
return 1;
}
template<class T, int N>  // helper func
inline constexpr T power4(const T x, integral_constant<T, N>)
{
return power4(x, integral_constant<T, N - 1>)*x;
}
template<int N, class T>   // public interface
inline constexpr T power4(const T x)
{
return power4(x, integral_constant<T, N>);
}
*/
template <typename T, int N, bool odd = N % 2>
struct Power_bisect  // can not overload class name Power
{
	T operator()(T x) {
		return Power_bisect<T, N / 2>()(x) * Power_bisect<T, N / 2>()(x);
	}
};

template <typename T, int N>
struct Power_bisect<T, N, true>  // specialize odd power
{
	T operator()(T x) {
		return x *Power_bisect<T, (N - 1) / 2>()(x) *Power_bisect<T, (N - 1) / 2>()(x);
	}
};

template <typename T>   // specialization, must be defined after general template
struct Power_bisect<T, 0, false> {
	T operator()(T x) {
		return 1;
	}
};

template <typename T>   // specialization, must be defined after general template
struct Power_bisect<T, 1, true> {
	T operator()(T x) {
		return x;
	}
};
// constexpr template function, cannot specialize template function
template <class T>
constexpr T pow_const(const T base, unsigned exp) {  // must use recursion, one line restriction in c+=11
	return exp == 0 ? 1 : base * pow_const(base, exp - 1);
}

template <class T>
constexpr T pow_const_bisect(const T base, unsigned exp) { // bisection method, faster
	return exp == 0 ? 1 :
		exp % 2 == 0 ? pow_const_bisect(base, exp / 2) * pow_const_bisect(base, exp / 2) :
		base * pow_const_bisect(base, (exp - 1) / 2) * pow_const_bisect(base, (exp - 1) / 2);
}

// force constexpr
template <class T, T base, unsigned exp>
using pow_const_force = integral_constant<T, pow_const_bisect(base, exp)>;

// examples from cppcon2014 Walter Brown
// gcd
template <long long P, long long Q>  // P>Q
struct GCD
{
	const static long long value = GCD<Q, P%Q>::value;
};
template<long long P>
struct GCD<P, 0>
{
	static_assert(P != 0, "GCD(0,0) is invalid");
	const static long long value = P;
};

template <size_t N> struct Factorial
{
	static const int value = N*Factorial<N - 1>::value;
};
template<> struct Factorial<0> {
	static const int value = 1;
};

template<size_t N> struct Fibonacci; // primary template, forward declaration
template<> struct Fibonacci<0> { static const int value = 0; };
template<> struct Fibonacci<1> { static const int value = 1; };
template<size_t N> struct Fibonacci
{
	static const int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};
//template <size_t N> using fib_v = Fibonacci<N>::value; nontype alias template not supported

template<size_t N, size_t K> struct nChooseK;
template<size_t N> struct nChooseK<N, 0>
{
	static const int value = 1;
};
template<size_t N, size_t K> struct nChooseK
{
	static const int value = N * nChooseK<N - 1, K - 1>::value/K;
};