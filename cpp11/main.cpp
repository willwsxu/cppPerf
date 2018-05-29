#include "dynBuffer.h"
#include "DynMsg.h"
#include <iostream>
#include <vector>
#include <random>
using namespace std;

#include "matrix2D.h"
#include "variadic.h"

typedef Matrix2D<int, vector> MatrixVii;
typedef Matrix2D<int, vector>::ColIterator vii_col_iter;

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "..\catch.hpp"

TEST_CASE("Variadic template", "VARIA")
{
	Console c;
	c(1, "[%p] DynBuffer (from [%p]) resize to max allowed %d", &c, 10, 20);

	int i = 1;
	float f = 2.0f;
	testPattern("%f %f", i, f);
	testPattern2("%f %f", i, f);

	CHECK(variadicTuple(i, f) == tuple<int,float>{i,f});

	/*ostringstream oss;
	oss << "test";
	//cout << variadicTuple(i, f);
	REQUIRE(oss.str() == string("1,2.0"));*/

}

TEST_CASE("DynBuffer", "BUF")
{
	CDynBuffer<Console,char> buf;
	buf.AppendData("Hello");
	REQUIRE(buf.GetBufferLen() == 5);
}

TEST_CASE("DynMsg", "DMSG")
{
	SECTION("basic") {
		CDynMsg<Console, char> buf;
		buf.AppendData("Hello");
		REQUIRE(buf.GetBufferLen() == 5);
	}
	SECTION("copy") {
		CDynMsg<Console, char> buf;
		buf.AddMsg("Hello!", 6);
		CDynMsg<Console, char> buf2 = buf;
		CHECK(buf.GetBufferLen() == 10);
		CHECK(buf2.GetBufferLen() == 10);
		REQUIRE(buf2.GetNumMsgs() == 1);
	}
}


TEST_CASE("Matrix", "[MATRIX1]")
{
	MatrixVii vii;
	vii.emplace_back(vector<int>{2, 4, 6});
	vii.emplace_back(vector<int>{1, 7, 7});
	vii.emplace_back(vector<int>{1, 3, 7});
	auto it = vii.begin(1);
	++it;
	--it;
	it += 1;
	it -= 1;
	auto z=it + 1;
	auto y=it - 1;
	auto end = vii.end(1);
	auto size = end - it;
	vii_col_iter zz = z;
	zz = z;
	CHECK(count(vii.begin(1), end, 1) == 0);
	CHECK(count(vii.begin(1), end, 3)==1);
	CHECK(count_if(vii.begin(1), end, [](auto i) {return i % 2 == 0; }) == 1);
	CHECK(find_if(vii.begin(1), end, [](auto i) {return i % 2 == 0; }) != end);
	CHECK(is_sorted(vii.begin(1), vii.end(1)) == false);
	reverse(vii.begin(1), end);
	CHECK(*vii.begin(1) == 3);
	auto x= lower_bound(vii.begin(2), vii.end(2), 6);
	REQUIRE(*x == 6);
}
TEST_CASE("Matrix 2", "[MATRIX]")
{
	MatrixVii vii;
	vii.emplace_back(vector<int>{2, 4, 6});
	vii.emplace_back(vector<int>{1, 3, 7});
	stable_sort(vii.begin(1), vii.end(1));
	CHECK(vii[0][1] == 3);

	std::random_device rd;
	std::mt19937 g(rd());
	shuffle(vii.begin(1), vii.end(1),g);
	sort(vii.begin(1), vii.end(1));
	CHECK(vii[1][1] == 4);
	CHECK(is_sorted(vii.begin(1), vii.end(1)) == true);
}

#include "meta.h"
TEST_CASE("Meta Programming", "[META]")
{
	CHECK(Power<2>::pow == 4);
	CHECK(Power2<3,2>::pow == 9);
	CHECK(Power3<long, 30>()(2L) == 1073741824);
	CHECK(Power3<long, 29>()(2L) == 536870912);
	CHECK(pow1(2, 30) == 1073741824);  // constexpr
	CHECK(pow2(2, 29) == 536870912);  // constexpr

	CHECK(GCD<1200, 800>::value == 400);
	using array_t = int[2][3][4];
	CHECK(Rank<array_t>::value == 3);
	CHECK(Rank2<array_t>::value == 3);

	CHECK( std::is_void<void>::value ==true);
	CHECK(bool(std::is_void<int>{}) == false);
	CHECK(std::is_void<int>{}() == false);
	CHECK(Is_void<int>::value == false);
	CHECK(Is_void2<int>::value == false);

	CHECK(Is_copy_assignable<CDynBuffer<Console, char>>{}() == false);
	CHECK(Is_copy_assignable<vii_col_iter>{}() == true);
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
	for (const auto p: find_all(s, 'a')) 
		CHECK(*p=='a');
	REQUIRE(find_all(s, 'a').size() == 4);
}

struct Base {};
struct Derived : public Base {};

#include "Alexandrescu.h"
TEST_CASE("type conversion check", "[CONV]")
{
	using dyn_msg_t = CDynMsg<Console, char>;
	using dyn_buf_t = CDynBuffer<Console, char>;
	//CHECK(Conversion<double, int>::result == 1);  //convert double to int, ok but warning
	CHECK(Conversion<int, double>::result == 1);

	dyn_msg_t x;
	dyn_buf_t &y = x;
	//dyn_msg_t &z = y;
	//auto x=conversion_t<dyn_msg_t, dyn_buf_t>;
	//CHECK(Conversion<dyn_msg_t, dyn_buf_t>::result == 1);   // lack copy constructor
	//using xx = decltype(declval<Derived&>() = declval<Base&>());  // not well formed
	using yy = decltype(declval<Base&>() = declval<Derived&>());
	CHECK(type_convertable<Derived, Base>::value == true);  // Derived can be converted to Base
	CHECK(type_convertable<Base, Derived>::value == false);
}

#include "courseSchedule.h"

TEST_CASE("Course Schedule conflict", "SCHED")
{
	std::vector<Schedule> schedule;
	const char*str[] = { "CS101", "W", "1730", "2030","CS242", "T", "1000", "1130","CS242", "T", "1230", "1430","CS242", "R", "1000", "1130","CS281", "T", "1300", "1430",
		"CS281", "R", "1300", "1430","CS282", "M", "1300", "1430","CS282", "W", "1300", "1430","CS201", "T", "1600", "1730","CS201", "R", "1600", "1730" };
	auto count = sizeof(str) / sizeof(char*);
	std::copy(cstring_iterator<Schedule>(count, str, 4), cstring_iterator<Schedule>(), back_inserter(schedule));
	CHECK(schedule.size() == 10);
	sort(begin(schedule), end(schedule));
	copy(begin(schedule), end(schedule), ostream_iterator<Schedule>(cout, "\n"));
	auto conflict = adjacent_find(begin(schedule), end(schedule));
	if (conflict != end(schedule)) {
		cout << "CONFLICT:" << endl;
		cout << *conflict << endl;
		cout << *(conflict + 1) << endl;
	}
	std::transform(begin(schedule), end(schedule) - 1, begin(schedule) + 1, begin(schedule), [](const auto& lhs, const auto& rhs)->const auto& {
		if (lhs == rhs) {
			cout << "CONFLICT:" << endl;
			cout << lhs << endl;
			cout << rhs << endl;
		}
		return lhs;
	});
	copy(begin(schedule), end(schedule), ostream_iterator<Schedule>(cout, "\n"));
}