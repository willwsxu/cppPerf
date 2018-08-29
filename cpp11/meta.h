#pragma once
#include <type_traits>

// force constexpr
template <class T, T base, unsigned exp>
using pow_const = integral_constant<T, pow2(base, exp)>;

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
template<class T, std::enable_if_t < is_class_v<T>, int > = 0 >
int enable_if_test() {
	return 0;
}
/* cannot coexist with is_pointer_v
template<class T, typename = std::enable_if_t<std::is_array_v<T>> >
int enable_if_test() {
	return 1;
}
*/
template<class T, typename = std::enable_if_t<std::is_pointer_v<T>> >
int enable_if_test() {
	return 2;
}

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

template <typename ...> using Void_t = void;  // use to test if any class is well formed

template <typename, typename=void>   //primary, detect presence/absence of a type member
struct Has_type_member : false_type {};
template <typename T> // specialization
struct Has_type_member<T, void_t<typename T::type>> : true_type {};  // if T::type is well formed, more specialized

// test if class T supports copy assignment
template <typename T>
using copy_assignment_t = decltype(declval<T&>()=declval<T const&>());

template<typename T, class=void>  // primary template, default arg is essential
struct Is_copy_assignable : false_type {};

template <typename T>
struct Is_copy_assignable<T, void_t<copy_assignment_t<T>>> : Test::is_same<copy_assignment_t<T>, T& > {};