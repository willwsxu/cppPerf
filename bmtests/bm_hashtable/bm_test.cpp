#include "stdafx.h"
#include <string>
#include <memory>
#include <functional>
#include <benchmark/benchmark.h>
#include <bitset>
#include <random>
#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_set>

using namespace std;
//#include "bm_hashtable_large.cpp"

vector<int> candies(10000);
static void BM_bitset_create(benchmark::State& state) {
	for (auto _ : state) {
		if (candies.empty()) {
			std::random_device rd;
			std::mt19937 g(rd());
			uniform_int_distribution<> dist(-100000, 100000);
			for (int &n : candies)
				n = dist(g);
			cout << candies.size() << endl;
		}
	}
	state.SetItemsProcessed(candies.size());
}
BENCHMARK(BM_bitset_create);

static void BM_bitset(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		bitset<200001> exist;  // beat 31%
		size_t count = 0;
		for (int c : candies) {
			if (!exist.test(c + 100000)) {  // much faster to use test and set, than [], beat 89%
				exist.set(c + 100000, 1);
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_bitset);

static void BM_bitset_bracket(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		bitset<200001> exist;  // beat 31%
		size_t count = 0;
		for (int c : candies) {
			if (!exist[c + 100000]) {  // much faster to use test and set, than [], beat 89%
				exist[c + 100000]= 1;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_bitset_bracket);

static void BM_unordered_set(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		ans = min(unordered_set<int>(begin(candies), end(candies)).size(), candies.size() / 2);;
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_unordered_set);

static void BM_wait(benchmark::State& state) {
	using namespace std::chrono_literals;
	for (auto _ : state)
		std::this_thread::sleep_for(300ms);
}
//BENCHMARK(BM_wait);
BENCHMARK_MAIN();