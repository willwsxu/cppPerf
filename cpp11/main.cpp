
#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <random>
#include <string>
using namespace std;

#include "ConsoleLogger.h"
#include "dynBuffer.h"
#include "DynMsg.h"
#include "matrix2D.h"

typedef Matrix2D<int, vector> MatrixVii;
typedef Matrix2D<int, vector>::ColIterator vii_col_iter;

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "..\catch.hpp"

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


#include <thread>
#include <mutex>

thread_local unsigned int rage = 1;
std::mutex cout_mutex;

void increase_rage(const std::string& thread_name)
{
	++rage; // modifying outside a lock is okay; this is a thread-local variable
	std::lock_guard<std::mutex> lock(cout_mutex);
	std::cout << "Rage counter for " << thread_name << ": " << rage << '\n';
}

void threadtest()
{
	std::thread a(increase_rage, "a"), b(increase_rage, "b");

	{
		std::lock_guard<std::mutex> lock(cout_mutex);
		std::cout << "Rage counter for main: " << rage << '\n';
	}

	a.join();
	b.join();

	{
		std::lock_guard<std::mutex> lock(cout_mutex);
		std::cout << "After Rage counter for main: " << rage << '\n';  // rage is not changed in the main thread
	}
}


TEST_CASE("Thread local", "THREAD")
{
	threadtest();
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

#include "movector.h"
TEST_CASE("Small class Custom", "CUST")
{
	string s("Irina");
	Cust c1("Joe", "Fix",42);	// 2 mallocs (2cr)
	Cust c2(s, "Fix", 42);		// 2 mallocs (1cp + 1cr)
	Cust c3(move(s), "z");		// 1 mallocs (1mv + 1cr)
	Cust c4{ "Jill" };
	Cust c5{ c4 };
	Cust c6 = "Jill";
	VIP  v{ "Boss" };
	VIP  v2{ v };
	Cust c7{ v };
}

#include "eraseRemove.h"

TEST_CASE("remove erase idiom", "[NEW]")
{
	vector<int> v{ 1,2,3,4,5 };
	erase_remove(v, 3);
	CHECK(v == vector<int>{1, 2, 4, 5});

	deque<int> d{ 1,2,3,4,5 };
	erase_remove(d, 3);
	CHECK(d == deque<int>{1, 2, 4, 5});

	string st{ "12345" };  // string and set have same template signature, how to distinguish?
	erase_remove(st, '3');
	CHECK(st == "1245");

	list<int> l{ 1,2,3,4,5 };
	erase_remove(l, 3);
	CHECK(l == list<int>{1, 2, 4, 5});

	set<int> s{ 1,2,3,4,5 };
	erase_remove(s, 3);
	CHECK(s == set<int>{1, 2, 4, 5});
}

TEST_CASE("remove erase idiom with predicate", "[NEW]")
{
	auto even = [](int i) { return i % 2 == 0; };
	vector<int> v{ 1,2,3,4,5 };
	erase_remove_if(v, even);
	CHECK(v == vector<int>{1, 3, 5});

	deque<int> d{ 1,2,3,4,5 };
	erase_remove_if2(d, even);
	CHECK(d == deque<int>{1, 3, 5});

	string st{ "12345" };  // string and set have same template signature, how to distinguish?
	erase_remove_if(st, [](char i) { return (i-'0') % 2 == 0; });
	CHECK(st == "135");

	list<int> l{ 1,2,3,4,5 };
	erase_remove_if(l, even);
	CHECK(l == list<int>{1, 3, 5});

	set<int> s{ 1,2,3,4,5 };
	erase_remove_if(s, even);
	CHECK(s == set<int>{1, 3, 5});
}


class Base1
{
private:
	int b1=0;
public:
	int mf1()
	{
		return 1;
	}
};

class Base2
{
private:
	int b2 = 0;
public:
	int mf2()
	{
		return 2;
	}
};

class MultiInheritance : public Base1, public Base2
{
private:
	int d = 0;
public:
	int mf3()
	{
		return 3;
	}
};


int test(MultiInheritance& obj, int (MultiInheritance::*mf)())
{
	printf("derived %p\n", mf);
	return (obj.*mf)();
}

int test2(Base2& obj, int (Base2::*mf)())
{
	printf("Base2 %p\n", mf);
	return (obj.*mf)();
}

TEST_CASE("member fun test multi inheritance pointer offset", "[NEW]")
{
	MultiInheritance obj;
	Base1*pB1 = &obj;
	Base2*pB2=&obj;    // pB1 address +4
	MultiInheritance*pD = &obj; // same as pB1 address
	CHECK((Base1*)pB2 - pB1 == 1);
	CHECK((Base1*)pD - pB1 == 0);

	// pmf_b==pmf_a, c++ template complete guide ed 1 state adjust is different, but no longer in vs2015 compiler
	int (MultiInheritance::*pmf_a)() = &MultiInheritance::mf2; // adjustment of +4 recorded
	int (Base2::*pmf_b)() = (int (Base2::*)())pmf_a; // adjustment changed to 0

	CHECK(test(obj, &MultiInheritance::mf1) == 1);
	CHECK(test(obj, &MultiInheritance::mf2) == 2);
	CHECK(test(obj, &MultiInheritance::mf3) == 3);
	CHECK(test(obj, &Base2::mf2) == 2);

	CHECK(test2(obj, &MultiInheritance::mf2) == 2);
}

#include "str.h"
TEST_CASE("test construction", "[NEW]")
{
	Str s1("test ");
	CHECK(s1.state == Str::STRING_MOVE_CTOR);
	string x("test2 ");
	Str s2(x);
	CHECK(s2.state == Str::STRING_CTOR);
	Str s2a(move(x));
	CHECK(s2a.state == Str::STRING_MOVE_CTOR);
	Str s2b(string("test2b "));
	CHECK(s2b.state == Str::STRING_MOVE_CTOR);
	Str s3(Str("test3 "));  // elision, one constructor call
	CHECK(s3.state == Str::STRING_MOVE_CTOR);
	Str s9 = string("test9 ");
	CHECK(s9.state == Str::STRING_MOVE_CTOR);
	Str s10 = "test10 ";  // up to 2 conversions, const char * -> string -> Str
	CHECK(s10.state == Str::STRING_MOVE_CTOR);

	Str s4(s1);
	CHECK(s4.state == Str::COPY_CTOR);
	Str s4a(move(s1));
	CHECK(s4a.state == Str::MOVE_CTOR);

	copy_elision(make_str("test5 "));  // elision, one constructor call
	copy_elision(Str("test5a "));
	Str s6(make_str("test6 "));
	CHECK(s6.state == Str::STRING_MOVE_CTOR);

	Str s7 = s6;
	CHECK(s7.state == Str::COPY_CTOR);
	Str s8 = move(s6);
	CHECK(s8.state == Str::MOVE_CTOR);

	s9 = move(s10);  // move assignment
	s8 = s7;  // copy assignment

	Str2 s20 = string("test20 "); // up to 2 conversions, string -> Str -> Str2
	//Str2 s20 = "test20 ";  can't compile
}
