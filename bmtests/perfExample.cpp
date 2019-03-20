//#include "stdafx.h"
#include <string>
#include <memory>
#include <functional>
#include <iostream>
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
//#include "bm_performance_general\pass_pointer.h"
//#include "bm_performance_general\reduandant_construct.h"
//#include "bm_performance_general\branch.h"
//#include "bm_performance_general\lambda.h"
//#include "bm_performance_general\constructors.h"

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


// const methods should be thread safe
// don't calculate more than once
struct Int
{
	Int(const string& s) :m_i(atoi(s.c_str())) {}
private:
	int m_i;
	//string _s; // don't
};

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
