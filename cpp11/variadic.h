#pragma once
/*
void tprintf(const char *fmt) {
	cout << fmt << endl;
}
template <typename T, typename ... Targs>
void tprintf(const char *fmt, T val, Targs ...Fargs)
{
	while (*fmt) {
		if (*fmt == '%') {
			cout << val;
			tprintf(fmt + 2, Fargs...); // expand parameter pack
			return;  // must return to avoid repeat
		}
		else
			cout << *fmt++;
	}
}
//tuple piecewise constructor
/*
class Console
{
public:
	template<typename... Targs>
	void operator()(const char *fmt, Targs...Fargs)
	{
		tprintf(fmt, Fargs...);
	}
	template<typename... Targs>
	void operator()(int sev, const char *fmt, Targs...Fargs)
	{
		cout << "WARN ";
		tprintf(fmt, Fargs...);
	}
};
*/
template<typename ... Targs, typename = void, typename U>
void testPattern(U fmt, Targs...targs)
{
	tprintf(fmt, targs ...);
}
template<typename ... Targs, typename = void, typename U>
void testPattern2(U fmt, Targs...targs)
{
	testPattern(fmt, &targs ...);
}

template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch, Tr>& os,
	const Tuple& t,
	std::index_sequence<Is...>)
{
	((os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), ...); // c++17 only
}

template<class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os,
	const std::tuple<Args...>& t)
{
	os << "(";
	print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
	return os << ")";
}

template<typename ... Targs>
auto variadicTuple(Targs...targs)
{
	auto x = std::make_tuple (targs ...);
	return x;
}

template <typename T>
auto Min(T first)
{
	return first;
}
template <typename T, typename ...ARGS>
auto Min(T first, ARGS...args)
{
	auto second = Min(args...);
	return first < second ? first : second;
}

struct unit
{};
struct swallow
{
	template <typename...ARGS>
	swallow(ARGS&&...) {}
};
template <typename... ARGS>
void expression(ARGS...args)
{
	swallow{ (std::cout << args << ' ', unit{})... };  // variadic expression, initializer list force evaluation from left to right
}

template <typename... ARGS>
class VBase
{
public:
	VBase(ARGS...args) {}
};

template <typename... ARGS>
class Compose {
protected:
	typedef tuple<ARGS...> Variant;
	VBase<ARGS...> base;
public:
	Compose(ARGS ...args) :base(forward<ARGS>(args)...) {}
};