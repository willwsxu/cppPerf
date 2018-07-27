#include "stdafx.h"
#include <string>
//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
//#include "catch.hpp"
#include <benchmark/benchmark.h>

using namespace std;


static void BM_post_increment(benchmark::State& state) {
	for (auto _ : state)
	{
		int i = 0;
		int a[10];
		while (i < 10)
			a[i++] = 0;
	}
}

static void BM_pre_increment(benchmark::State& state) {
	for (auto _ : state)
	{
		int i = -1;
		int a[10];
		while (i < 9)
			a[++i] = 0;
	}
}

static void BM_int_div(benchmark::State& state) {
	for (auto _ : state)
	{
		int i=1000;
		while (i > 0)
			i /= 10;
	}
}
static void BM_int_no_div(benchmark::State& state) {
	for (auto _ : state)
	{
		int i = 1000;
		while (i > 0) {
			if (i == 1000)
				i = 100;
			else if (i == 100)
				i = 10;
			else if (i == 10)
				i = 1;
			else
				i = 0;
		}
	}
}
/*BENCHMARK(BM_post_increment);
BENCHMARK(BM_pre_increment);
BENCHMARK(BM_int_div);
BENCHMARK(BM_int_no_div);*/
BENCHMARK_MAIN();

// Practical performance practices by Jason Turner
// aware compiler optimization
// use array or vector by default, avoid list
// prefer unique_ptr over shared_ptr
// watch out for template bloating
// avoid none local data
// constexpr can result in bigger code, compiler can 
// use final, help inlining
// few branches
// smaller code is faster code, improved cache hit rate

/* Implicitly-declared move constructor is created if 
there are no user-declared copy constructors;
there are no user-declared copy assignment operators;
there are no user-declared move assignment operators;
there are no user-declared destructors;
*/
class Base
{
	string _s;
public:
	Base(string s) : _s(s) { /*std::cout << "ctor" << endl;*/ }
	Base(const Base& copy) :_s(copy._s) { /*std::cout << "copy ctor" << endl;*/ }
	Base(Base&& copy) :_s(move(copy._s)) {/*std::cout << "move ctor" << endl;*/ }
	virtual ~Base() {};
	virtual void do_a_thing() {};
};

struct Derived : public Base
{
	Derived(string s) :Base(s) {}
	virtual void do_a_thing() override {}
};

// rule 0, don't disable move constructor by accident
struct Derived2 : public Base
{
	Derived2(string s) :Base(s) {}
	virtual ~Derived2() = default;  // destructor prevents implicit move constructor
	virtual void do_a_thing() override {}
};


static void BM_base_ctor(benchmark::State& state) {
	for (auto _ : state)
	{
		Base b("1234567890");
	}
}
static void BM_move_ctor(benchmark::State& state) {
	for (auto _ : state)
	{
		Base b("1234567890");
		Base b2(move(b));
	}
}

static void BM_copy_ctor(benchmark::State& state) {
	for (auto _ : state)
	{
		Base b("1234567890");
		Base b2(b);
	}
}

static void BM_derived_move_ctor(benchmark::State& state) {
	static_assert(sizeof(Derived) >= 4, "inheritance with virtual methods"); // 28 in release mode
	for (auto _ : state)
	{
		Derived d("1234567890");
		Derived d2(move(d));
	}
}

static void BM_derived2_failed_move_ctor(benchmark::State& state) {
	for (auto _ : state)
	{
		Derived2 d("1234567890");
		Derived2 d2(move(d));
	}
}
static void BM_derived_copy_ctor(benchmark::State& state) {
	for (auto _ : state)
	{
		Derived d("1234567890");
		Derived d2(d);
	}
}
BENCHMARK(BM_base_ctor);//33ns
BENCHMARK(BM_copy_ctor);//50ns
BENCHMARK(BM_move_ctor);//47ns
BENCHMARK(BM_derived_copy_ctor);//67ns
BENCHMARK(BM_derived_move_ctor);//62ns
BENCHMARK(BM_derived2_failed_move_ctor);//67ns


// const methods should be thread safe
// don't calculate more than once
struct Int
{
	Int(const string& s) :m_i(atoi(s.c_str())) {}
private:
	int m_i;
	//string _s; // don't
};
/*
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

string add(const string&lhs, const string&rhs)
{
	return lhs + rhs;
}
TEST_CASE("prefer lambda over bind/function object", "[NEW]")
{
	const auto good = [](const string&b) {
		return add("Hello ", b);
	};
	CHECK(good("World") == "Hello World");

	const auto bad = bind(add, "Hello ", placeholders::_1);
	CHECK(bad("World") == "Hello World");

	const function<string (const string&)> worse= bind(add, "Hello ", placeholders::_1);
	CHECK(worse("World")=="Hello World");
}

TEST_CASE("less branches, test 0", "[NEW]")
{
	auto good = [](int err) {
		if (!err) {
			// do stuff
			return 1;
		}
		else {
			if (err == 1)		return -1;
			else if (err == 2)	return -2;
			else if (err == 3)	return -3;			
		}
		return 0;
	};
	auto bad = [](int err) {
		if (err == 1) {
			return -1;
		}
		else if (err == 2) {
			return -2;
		}
		else if (err == 3) {
			return -3;
		}
		else {
			// do stuff
			return 1;
		}
		return 0;
	};

	long loops = 100000000;
	auto perfTest = [loops](const char *name, auto func) {
		auto start = chrono::high_resolution_clock::now();
		long count = 0;
		for (long i = 0; i < loops; i++)
			count += func(0);
		auto end = chrono::high_resolution_clock::now();
		auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
		cout << name << " nano seconds: " << nanos.count() << " count " << count << endl;

	};

	perfTest("test 0", good);
	perfTest("test 1", bad);
}
*/
// prefer template or factory over runtime polymorphism
// memory is slow, delete is slow and probably can be done in a separate thread
// keep cache hot, don't share L3 cache, use one CPU per thread
// placement new a little slow due to nullptr check
// static local initialization should not be used in hotpath, slow
// gcc pow(base, 1.5) is slow
// no system call in hotpath
// profiling vs benchmarking ( google mivro benchmarking ), should do both
// measure micro latency in environment simulating production data flow
// hash table probing, keep a pointer to large data. or try google densh hash map
// inline can slow down. don't inline rarely used code like error logging
