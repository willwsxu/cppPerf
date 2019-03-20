#include <string>
#include <memory>
#include <functional>
#include <benchmark/benchmark.h>
#include <bitset>
#include <random>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
#include "bm_hashtable_small.cpp"
//#include "bm_hashtable_large.cpp"
//#include "bm_unique_test.cpp"

static void BM_wait(benchmark::State& state) {
	using namespace std::chrono_literals;
	for (auto _ : state)
		std::this_thread::sleep_for(300ms);
}
//BENCHMARK(BM_wait);
BENCHMARK_MAIN();