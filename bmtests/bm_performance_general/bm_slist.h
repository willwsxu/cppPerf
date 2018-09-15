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
BM_slist_shared        131 ns        132 ns    4977778    7.2338M items/s
BM_slist_unique         89 ns         88 ns    7466667   10.8507M items/s
BM_forward_list         84 ns         85 ns    8960000   11.1607M items/s
BM_slist_raw            80 ns         80 ns    8960000   11.8886M items/s
*/
static void BM_slist_shared(benchmark::State& state) {  // call_once use 25ns comparing 1ns in other tests
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

static void BM_slist_unique(benchmark::State& state) {  // call_once use 25ns comparing 1ns in other tests
	using TestSlist = slist_u<Slist_Tester>;
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

static void BM_forward_list(benchmark::State& state) {  // call_once use 25ns comparing 1ns in other tests
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

static void BM_slist_raw(benchmark::State& state) {  // call_once use 25ns comparing 1ns in other tests
	using TestSlist = slist_r<false, Slist_Tester>;
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