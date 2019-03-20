#pragma once

#include <atomic>
#include <mutex>

#include <benchmark/benchmark.h>

using namespace std;

static void BM_call_once(benchmark::State& state) {  // call_once use 25ns comparing 1ns in other tests
	int count = 0;
	for (auto _ : state) {
		static std::once_flag flag;
		std::call_once(flag, [&count]() { count++; });
	}
	state.SetItemsProcessed(count);
}
BENCHMARK(BM_call_once);


static void BM_atomic_once(benchmark::State& state) {
	int count = 0;
	for (auto _ : state) {
		static std::atomic<bool> flag;
		if ((bool)flag == false) {
			count++;
			flag.exchange(true);
		}
	}
	state.SetItemsProcessed(count);
}
BENCHMARK(BM_atomic_once);


static void BM_atomic(benchmark::State& state) {  // baseline for atomic
	int count = 0;
	for (auto _ : state) {
		std::atomic<bool> flag;
		if ((bool)flag == false) {
			count++;
			flag.exchange(true);
		}
	}
	state.SetItemsProcessed(count);
}
BENCHMARK(BM_atomic);


static void BM_thread_local(benchmark::State& state) {
	int count = 0;
	thread_local bool flag = false;
	for (auto _ : state) {
		if (!flag) {
			count++;
			flag = true;
		}
	}
	state.SetItemsProcessed(count);
}
BENCHMARK(BM_thread_local);
