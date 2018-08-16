#include "stdafx.h"
#include <string>
#include <memory>
#include <functional>
#include <benchmark/benchmark.h>

BENCHMARK_MAIN();

using namespace std;

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

//#include "bm_performance_general\pre_post_increment.h"
#include "bm_performance_general\pass_pointer.h"

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

/*BENCHMARK(BM_int_div);
BENCHMARK(BM_int_no_div);*/

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

static void BM_derived_copy_ctor(benchmark::State& state) {
	for (auto _ : state)
	{
		Derived d("1234567890");
		Derived d2(d);
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
/*
BENCHMARK(BM_base_ctor);//33ns
BENCHMARK(BM_copy_ctor);//50ns
BENCHMARK(BM_move_ctor);//47ns
BENCHMARK(BM_derived_copy_ctor);//67ns
BENCHMARK(BM_derived_move_ctor);//62ns
BENCHMARK(BM_derived2_failed_move_ctor);//67ns
*/

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
	//Str(string&& s) :_s(move(s)) {}
private:
	string _s;
};

// use initilizer, use const when possible
static void BM_redundant_construction(benchmark::State& state) {
	for (auto _ : state)
	{
		string t("test");
		t += "more";
		Str s(move(t));
	}
}
static void BM_redundant_construction_append(benchmark::State& state) {
	for (auto _ : state)
	{
		string t("test");
		t.append("more");
		Str s(move(t));
	}
}
static void BM_initializer_construction_append(benchmark::State& state) {
	for (auto _ : state)
	{
		Str s(move(string("test").append("more")));
	}
}

static void BM_initializer_construction(benchmark::State& state) {
	for (auto _ : state)
	{
		Str s(move(string("test") + "more")); // + is more costly then +=
	}
}

static void BM_initializer_construction_append_no_move(benchmark::State& state) {
	for (auto _ : state)
	{
		Str s(string("test").append("more"));
	}
}
BENCHMARK(BM_redundant_construction);  // 51ns
BENCHMARK(BM_redundant_construction_append);  // 50ns
BENCHMARK(BM_initializer_construction_append); // 49ns
BENCHMARK(BM_initializer_construction_append_no_move); //55ns
BENCHMARK(BM_initializer_construction);// 68ns, extra malloc

// endl slow down io due to flush
static void BM_endl(benchmark::State& state) {
	for (auto _ : state)
	{
		cout << "badd" << endl;
	}
}
static void BM_newline(benchmark::State& state) {
	for (auto _ : state)
	{
		cout << "good\n";
	}
}
//BENCHMARK(BM_endl);
//BENCHMARK(BM_newline);

// prefer lambda over bind/function object
string add(const string&lhs, const string&rhs)
{
	return lhs + rhs;
}

static void BM_lambda(benchmark::State& state) {
	const auto good = [](const string&b) {
		return add("Hello ", b);
	};
	for (auto _ : state)
	{
		good("World");
	}
}
static void BM_bind(benchmark::State& state) {
	const auto bad = bind(add, "Hello ", placeholders::_1);
	for (auto _ : state)
	{
		bad("World");
	}
}

static void BM_function(benchmark::State& state) {
	const function<string(const string&)> worse = bind(add, "Hello ", placeholders::_1);
	for (auto _ : state)
	{
		worse("World");
	}
}
/*
BENCHMARK(BM_lambda); // 76ns
BENCHMARK(BM_bind);   // 106ns
BENCHMARK(BM_function);//118ns
*/
int good(int err) {
	if (!err) {
		// do stuff
		return 0;
	}
	else {
		if (err == 1)		return -1;
		else if (err == 2)	return -2;
		else if (err == 3)	return -3;
		else return 1;
	}
};

int bad(int err) {
	if (err == 1) {
		return -1;
	}
	else if (err == 2) {
		return -2;
	}
	else if (err == 3) {
		return -3;
	}
	else if (err == 0) {
		// do stuff
		return 0;
	}
	else
		return 1;
};
// less branches, test 0
static void BM_branch_good(benchmark::State& state) {
	int ret = 0;
	for (auto _ : state)
	{
		ret=good(0);
	}
}

static void BM_branch_bad(benchmark::State& state) {
	int ret = 0;
	for (auto _ : state)
	{
		ret=bad(0);
	}
}

// too fast to show difference here. see test below
BENCHMARK(BM_branch_good); // 1ns, over 10% more iterations
BENCHMARK(BM_branch_bad);  // 1ns
/*
TEST_CASE("branch", "[NEW]")
{
	long loops = 1000000000;
	auto perfTest = [loops](const char *name, auto func) {
		auto start = chrono::high_resolution_clock::now();
		long count = 0;
		for (long i = 0; i < loops; i++)
			count += func(0);
		auto end = chrono::high_resolution_clock::now();
		auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
		cout << name << " nano seconds: " << nanos.count() << " count " << count << endl;
	};

	perfTest("test 0", good);  // 400ns
	perfTest("test 1", bad);   // 800ns
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
