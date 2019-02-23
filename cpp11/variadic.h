#pragma once

template<typename ... Targs, typename = void, typename U>
void testPattern(std::ostream& os, U fmt, Targs...targs)
{
	tprintf(os, fmt, targs ...);
}
template<typename ... Targs, typename = void, typename U>
void testPattern2(std::ostream& os, U fmt, Targs...targs)
{
	testPattern(os, fmt, &targs ...);
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
void expression(std::stringstream& ostr, ARGS...args)
{
	swallow{ (ostr << args << ' ', unit{})... };  // variadic expression, initializer list force evaluation from left to right
}

template <typename...ARGS>
struct pack {
	pack(ARGS...args) {}
};

template <typename Parent>
class VBase
{
public:
	VBase(typename Parent::Variant args) {}
};

template <typename... ARGS>
class Compose {
public:
	typedef pack<ARGS...>	Variant;
protected:
	VBase<Compose> base;
public:
	Compose(ARGS ...args) :base(Variant(args...)) {}
};

// Tuple impl, youtube Tech Talk: C++ Advanced Topics in Templates (Andy Modell)
// https://voidnish.wordpress.com/2013/07/13/tuple-implementation-via-variadic-templates/
template <typename...ARGS> class Tuple;  // forward declaration
template <> class Tuple<> {};  // full specialization, empty class

template<typename First, typename...Rest>
class Tuple<First, Rest...> : public Tuple<Rest...>  // Get fails if using private
{
public:
	First value;
};

template<size_t idx, typename Tuple_t> struct TupleElement;
template<typename First, typename...Rest>
struct TupleElement<0, Tuple<First, Rest...>> {
	using value_type = First;
	using tuple_type = Tuple<First, Rest...>;
};
template<size_t idx, typename First, typename...Rest>
struct TupleElement<idx, Tuple<First, Rest...>> : public TupleElement<idx - 1, Tuple<Rest...>>
{
};

template<size_t idx, typename...ARGS>
typename TupleElement<idx, Tuple<ARGS...>>::value_type& Get(Tuple<ARGS...>& t) {
	using tuple_type = TupleElement<idx, Tuple<ARGS...>>::tuple_type;
	return static_cast<tuple_type&>(t).value;
}

template<size_t idx, typename Tuple_t>
void FillTuple(Tuple_t& t) {}

template<size_t idx, typename Tuple_t, typename First, typename...Rest>
void FillTuple(Tuple_t& t, First& f, Rest...rest)
{
	Get<idx>(t) = f;
	FillTuple<idx + 1>(t, rest...);
}

template<typename ...ARGS>
Tuple<ARGS...> Make_Tuple(ARGS...args) {
	Tuple<ARGS...> ans;
	FillTuple<0>(ans, args...);
	return ans;
}
template<typename tuple_t>
decltype(auto) get_back(tuple_t& t) {
	return get<tuple_size_v<tuple_t>-1>(t);
}