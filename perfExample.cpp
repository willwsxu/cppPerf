#include "stdafx.h"

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

using namespace std;

// use vector by default, avoid list

/* Implicitly-declared move constructor is created if 
there are no user-declared copy constructors;
there are no user-declared copy assignment operators;
there are no user-declared move assignment operators;
there are no user-declared destructors;
*/
class Base
{
public:
	Base() = default;
	Base(const Base&) = default;
	//Base(Base&&) = delete;
	virtual ~Base() {};
	virtual void do_a_thing() {};
	string _s;
};

struct Derived : public Base
{
	//virtual ~Derived() = default;  not needed
	virtual void do_a_thing() override {}
};


TEST_CASE("Object Factory", "[NEW]")
{
	Base b;
	Base b2(std::move(b)); // why move constructor is not dead? maybe only c++11 issue
	Derived d;
	Derived d2(std::move(d));
	CHECK(sizeof(b2) == 4);
}

// const methods should be thread safe
// don't calculate more than once
struct Int
{
	Int(const string& s) :m_i(atoi(s.c_str())) {}
private:
	int m_i;
	//string _s; // don't
};

struct Str
{
	Str(string s) :_s(move(s)) {}
private:
	string _s;
};
TEST_CASE("redundant construction, use const, use initializer", "[NEW]")
{
	auto bad = []() {
		string t("test");
		t += "move";
		Str s(move(t));
	};
	auto good = []() {
		Str s(string("test") + "more");
	};
}

TEST_CASE("don't pass shared_ptr if no ownership is involved", "[NEW]")
{
	auto bad = [](const shared_ptr<Base>&b) {};
	auto good = [](const Base&b) {};
	auto d = make_shared<Derived>();
	good(*d.get());
}

TEST_CASE("endl slow down io due to flush", "[NEW]")
{
	cout << " good\n good\n";
	cout << " bad " << endl << " bad " << endl;
}