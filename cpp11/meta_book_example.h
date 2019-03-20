#pragma once

// from Alexandrescu book
template <typename T, typename U>
class Conversion  // from T to U
{
	typedef char Small;
	//typedef char Big[2];  // won't work
	class Big { char dummy[2]; };
	static Small convert(U u);
	static Big convert(...);
	static T makeT();  // or use declval

public:
	enum { result = sizeof(convert(makeT())) == sizeof(Small) };  // can only call static methods
};

// type conversion using mordern c++
template <typename T, typename U>  // convert T to U
using conversion_t = decltype(declval<U>() = declval<T>());

template<typename T, typename U, class = void>  // primary template, default arg is essential
struct type_convertable : false_type {};

template <typename T, typename U>
struct type_convertable<T, U, void_t<conversion_t<T,U>>> : true_type {};  // void_t is necessary here

// Vandevoorde book, page 428. example has one mistake
template <typename FROM, typename TO>
struct IsConvertibleHelper {
private:
	static void aux(TO);
	template <typename F, typename=decltype(aux(declval<F>()))>
	static true_type test(void *);  // overload choice for nullptr
	template <typename>
	static false_type test(...);
public:
	using Type = decltype(test<FROM>(nullptr)); // nullptr match first overload test func better if FROM can be converted to TO
};
template <typename FROM, typename TO>
struct IsConvertibleT : IsConvertibleHelper<FROM, TO>::Type {};