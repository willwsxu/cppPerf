#pragma once

#include <benchmark/benchmark.h>
#include "cpp11/slist.h"
#include <forward_list>

struct Slist_Tester
{
	int64_t	 a = 0;
	int64_t	 b = 0;
	int64_t	 c = 0;
	int64_t	 d = 0;
};

// benchmark std::forward_list vs my slist implemented with shared_ptr, unique_ptr, or raw pointer
/*
BM_slist_shared           122 ns        123 ns    5600000   7.76811M items/s
BM_slist_atomic           102 ns        103 ns    7466667    9.3006M items/s
BM_slist_unique            94 ns         94 ns    7466667   10.1273M items/s
BM_forward_list            86 ns         86 ns    7466667   11.1153M items/s
BM_slist_raw               80 ns         80 ns    7466667   11.9929M items/s
BM_slist_new_delete        80 ns         78 ns    8960000   12.1528M items/s
BM_slist_copy_base         10 ns         10 ns   64000000   95.2744M items/s
*/
static void BM_slist_shared(benchmark::State& state) {  // shared_ptr, 40ns overhead
	using TestSlist = slist<Slist_Tester>;
	int count = 0;
	TestSlist simple;
	for (auto _ : state) {
		simple.push_front(Slist_Tester());
		simple.pop_front();
		count++;
	}
	state.SetItemsProcessed(count);
}
BENCHMARK(BM_slist_shared);

static void BM_slist_unique(benchmark::State& state) {  // unique_ptr a few ns overhead
	using TestSlist = slist<Slist_Tester, false>;
	int count = 0;
	TestSlist simple;
	for (auto _ : state) {
		simple.push_front(Slist_Tester());
		simple.pop_front();
		count++;
	}
	state.SetItemsProcessed(count);
}
BENCHMARK(BM_slist_unique);

static void BM_forward_list(benchmark::State& state) {  // better than unique_ptr
	using TestSlist = std::forward_list<Slist_Tester>;
	int count = 0;
	TestSlist simple;
	for (auto _ : state) {
		simple.push_front(Slist_Tester());
		simple.pop_front();
		count++;
	}
	state.SetItemsProcessed(count);
}
BENCHMARK(BM_forward_list);

static void BM_slist_raw(benchmark::State& state) {  // best, all time is used for memory alloc
	using TestSlist = slist_r<Slist_Tester, bool*>;
	int count = 0;
	TestSlist simple;
	for (auto _ : state) {
		simple.push_front(Slist_Tester());
		simple.pop_front();
		count++;
	}
	state.SetItemsProcessed(count);
}
BENCHMARK(BM_slist_raw);

static void BM_slist_atomic(benchmark::State& state) {
	using TestSlist = slist_r<Slist_Tester, std::atomic<bool>>;
	int count = 0;
	TestSlist simple;
	for (auto _ : state) {
		simple.push_front(Slist_Tester());
		simple.pop_front();
		count++;
	}
	state.SetItemsProcessed(count);
}
BENCHMARK(BM_slist_atomic);

template<typename T>
class Slist_Test : public slist_r<T, bool*>
{
public:
	void test(T&& t)
	{
		auto n = new Node(t);
		delete n;
	}

	Node test_copy(T&& t)
	{
		return Node(t);
	}
};
static void BM_slist_raw_base(benchmark::State& state) {  // measure Node new and delete
	int count = 0;
	Slist_Test<Slist_Tester> simple;
	for (auto _ : state) {
		simple.test(Slist_Tester());
		count++;
	}
	state.SetItemsProcessed(count);
}
BENCHMARK(BM_slist_raw_base);

static void BM_slist_copy_base(benchmark::State& state) {  // copy Node
	int count = 0;
	Slist_Test<Slist_Tester> simple;
	for (auto _ : state) {
		auto x=simple.test_copy(Slist_Tester());
		count++;
	}
	state.SetItemsProcessed(count);
}
BENCHMARK(BM_slist_copy_base);
