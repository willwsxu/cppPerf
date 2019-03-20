
#include <benchmark/benchmark.h>
BENCHMARK_MAIN();

#include "movector.h"

/*
BM_ctor_forward         109 ns        110 ns    6400000
BM_ctor_by_value        154 ns        153 ns    4480000
BM_ctor_by_ref          108 ns        107 ns    6400000
*/
static void BM_ctor_forward(benchmark::State& state) {
	//size_t len = static_cast<size_t>(state.range(0));
	for (auto _ : state) {
		string s("Irina");
		Cust c1("Joe", "Fix", 42);	// 2 mallocs (2cr)
		Cust c2(s, "Fix", 42);		// 2 mallocs (1cp + 1cr)
		Cust c3(move(s), "z");		// 1 mallocs (1mv + 1cr)
	}
	//state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_ctor_forward);//->Range(8, 8 << 10);

static void BM_ctor_by_value(benchmark::State& state) {
	//size_t len = static_cast<size_t>(state.range(0));
	for (auto _ : state) {
		string s("Irina");
		Cust1 c1("Joe", "Fix", 42);	// 2 mallocs (2cr+2mv)
		Cust1 c2(s, "Fix", 42);		// 2 mallocs (1cp + 1cr+2mv)
		Cust1 c3(move(s), "z");		// 1 mallocs (1mv + 1cr)
	}
	//state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_ctor_by_value);//->Range(8, 8 << 10);

static void BM_ctor_by_ref(benchmark::State& state) {
	//size_t len = static_cast<size_t>(state.range(0));
	for (auto _ : state) {
		string s("Irina");
		Cust2 c1("Joe", "Fix", 42);	// 2 mallocs (2cr)
		Cust2 c2(s, "Fix", 42);		// 2 mallocs (1cp + 1cr)
		Cust2 c3(move(s), "z");		// 1 mallocs (1mv + 1cr)
	}
	//state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_ctor_by_ref);//->Range(8, 8 << 10);
