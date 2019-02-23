#include <atomic>
#include <sstream>

#include "..\catch.hpp"

#include "ConsoleLogger.h"
#include "variadic.h"
#include "dynBuffer.h"
#include "DynMsg.h"
#include "matrix2D.h"

#include "meta_math.h"
using namespace std;

struct Base {
	Base(int) {}
};
struct Derived : public Base {
	Derived(int x):Base(x) {}
};

#include "meta_book_example.h"
TEST_CASE("type conversion check", "[NEW]")
{
	CHECK(Conversion<Derived, Base>::result == 1);
	CHECK(Conversion<Base, Derived>::result == 0);

	using yy = decltype(declval<Base&>() = declval<Derived&>());
	CHECK(type_convertable<Derived, Base>::value == true);  // Derived can be converted to Base
	CHECK(type_convertable<Base, Derived>::value == false);

	CHECK(IsConvertibleT<Derived, Base>::value == true);
	CHECK(IsConvertibleT<Base, Derived>::value == false);
}

TEST_CASE("Variadic template", "[META]")
{
	Console c;
	c(LOG_INFO, "[%p] DynBuffer (from [%p]) resize to max allowed %d", &c, 10, 20);

	int i = 1;
	float f = 2.1f;
	ostringstream oss;
	testPattern(oss, "%f %f", i, f);
	CHECK(oss.str() == "1 2.1");
	oss.clear();
	testPattern2(oss, "%p %p", i, f);  // print address
	CHECK(oss.str() != "1 2.1");

	CHECK(variadicTuple(i, f) == tuple<int, float>{i, f});

	CHECK(Min(2, 3, 4, 1, 6) == 1);
	stringstream ostr;
	expression(ostr, 1, 2, "bar");
	CHECK(ostr.str() == "1 2 bar ");

	Compose<int, float, char> test(1, 2.0f, '3');  //?

	//tuple piecewise constructor
	pair<vector<int>, vector<string>> p(piecewise_construct, forward_as_tuple(3, 0), forward_as_tuple(2, "TT"));
	CHECK(p.first.size() == 3);
	CHECK(p.second.size() == 2);
	CHECK(p.second[0] == "TT");
	CHECK(get<vector<int>>(p) == vector<int>{0, 0, 0});
	tuple<int, double,char> x=make_tuple( 1, 1.2,'x' );
	CHECK(get_back<tuple<int, double,char>>(x) == 'x');
}

#include "meta.h"
TEST_CASE("Meta Programming", "[META]")
{
	CHECK(PowerOf2<3>::pow == 8);
	CHECK(PowerX<3, 2>::pow == 9);
	CHECK(Power3<long, 30>()(2L) == 1073741824);
	CHECK(Power3<long, 29>()(2L) == 536870912);
	CHECK(pow1(2, 30) == 1073741824);  // constexpr
	CHECK(pow2(2, 29) == 536870912);  // constexpr
	CHECK(pow_const<int, 2, 29>::value == 536870912);  // constexpr forced
	CHECK(pow_const<int, 2, 29>() == 536870912);  // constexpr forced
												  //CHECK(power4<29, int>(2) == 536870912);  fail to compile

	CHECK(GCD<1200, 800>::value == 400);
	using array_t = int[2][3][4];
	CHECK(Rank<array_t>::value == 3);
	CHECK(Rank2<array_t>::value == 3);

	CHECK(std::is_void<void>::value == true);
	CHECK(bool(std::is_void<int>{}) == false);
	CHECK(std::is_void<int>{}() == false);
	CHECK(Is_void<int>::value == false);
	CHECK(Is_void2<int>::value == false);

//	CHECK(Is_copy_assignable<CDynBuffer<Console, char>>{}() == false);
//	CHECK(Is_copy_assignable<vii_col_iter>{}() == true);

	CHECK(enable_if_test<std::atomic<int*>>() == 0);
	CHECK(enable_if_test<int[]>() == 1);
	CHECK(enable_if_test<int *>() == 2);

	CHECK(Enable_If_Test<std::atomic<int*>>().enable_if_test() == 0);
	CHECK(Enable_If_Test<int[]>().enable_if_test() == 1);
	CHECK(Enable_If_Test<int*>().enable_if_test() == 2);
}

template <typename C, typename V>
vector<typename C::value_type*> find_all(C& c, const V&v) // must use const V to allow literal
{
	vector<typename C::value_type*> ans;  // must typename C::, value_type<C> does not compile
	for (auto& x : c) {
		if (x == v)
			ans.push_back(&x);
	}
	return ans;
}

TEST_CASE("template example from Stroustrup", "example")
{
	string s{ "Mary has a little lamb" };
	for (const auto p : find_all(s, 'a'))
		CHECK(*p == 'a');
	REQUIRE(find_all(s, 'a').size() == 4);
}

TEST_CASE("factorial test", "[META]")
{
	CHECK(Factorial < 3 >::value == 6);
}

TEST_CASE("fibonacci test", "META")
{
	CHECK(Fibonacci<5>::value == 5);
}

TEST_CASE("n choose 2 test", "META")
{
	CHECK(nChooseK < 5, 2 >::value == 10);
}

TEST_CASE("Tuple test", "[META]")
{
	auto t=Make_Tuple( 10, 'x', "tuple");
	CHECK(Get<0>(t) == 10);
	CHECK(Get<1>(t) == 'x');
	CHECK(Get<2>(t) == "tuple");
}