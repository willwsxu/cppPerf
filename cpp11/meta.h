#pragma once
#include <type_traits>

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

// examples from cppcon2014 Brown
// gcd
template <long long P, long long Q>  // P>Q
struct GCD
{
	const static long long value = GCD<Q, P%Q>::value;
};
template<long long P>
struct GCD<P,0>
{
	static_assert(P != 0, "GCD(0,0) is invalid");
	const static long long value = P;
};


// following are in c++11, use upper case to avoid conflict
template <typename T>
struct Rank  // scalar base case
{
	static const size_t value = 0u;
};

template <typename U, size_t N>
struct Rank<U[N]>  // partial specialization
{
	static const size_t value = 1u + Rank<U>::value;  // recurse on next dimension
};

// declare Rank new way
template <typename T>
struct Rank2 : integral_constant<size_t, 0u> {};

template <typename U, size_t N>
struct Rank2<U[N]> : integral_constant<size_t, 1u + Rank2<U>::value>{};  // partial specialization

template <typename U>
struct Rank2<U[]> : integral_constant<size_t, 1u + Rank2<U>::value> {};  // partial specialization for unbound array

template <typename T>
struct Remove_const
{
	using type = T;
};
template <typename T>
struct Remove_const<const T>
{
	using type = T;
};

template <typename T>
struct type_is   // useful tool, as base class
{
	using type = T;
};

// same as conditional in c++11
template <bool, typename T, typename>
struct IF : type_is<T> {};   // true case

template <typename T, typename F>  // partial specialization
struct IF<false, T, F> : type_is<F> {};   // true case

template<bool, typename T=void>
struct Enable_if : type_is<T> {};

template<typename T>
struct Enable_if<false, T> {};  // no type when false

//SFINAE and enable_if

namespace Test
{
	template <bool b>
	using bool_constant = integral_constant<bool, b>;
	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

	template <typename T> struct is_void : false_type {};
	template <> struct is_void<void> : true_type {};  // more for const void, volatile void, and const volatile void

	template <typename T, typename U> struct  is_same : false_type {};
	template <typename T> struct  is_same<T, T> : true_type {};
	template <typename T> using remove_cv = remove_volatile<remove_const_t<T>>;
		
}
template <typename T> struct Is_void : Test::is_same<void, Test::remove_cv<T>> {};

// generalize is_same, does T match any one of the types listed
template <typename T, typename... Args> struct is_one_of;  // just declaration, 3 specializations
template <typename T> struct is_one_of<T> : public Test::false_type {};  // no other types
template <typename T, typename...Args> struct is_one_of<T,T,Args...> : public Test::true_type {}; // match top of the Args
template <typename T, typename U, typename...Args> struct is_one_of<T,U,Args...> : is_one_of<T, Args...> {}; // no match

template <typename T> struct Is_void2 : public is_one_of<T, void, void const, volatile void, volatile const void> {};