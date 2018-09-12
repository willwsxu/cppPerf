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
	static T makeT();

public:
	enum { result = sizeof(convert(makeT())) == sizeof(Small) };  // can only call static methods
};

template <typename T, typename U>  // convert T to U
using conversion_t = decltype(declval<U&>() = declval<T>());

template<typename T, typename U, class = void>  // primary template, default arg is essential
struct type_convertable : false_type {};

template <typename T, typename U>
struct type_convertable<T, U, void_t<conversion_t<T,U>>> : true_type {};