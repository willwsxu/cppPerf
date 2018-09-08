#include <benchmark/benchmark.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_set>
#include <array>

vector<int> candies(10000);  // find unique number in vector, value range [-100000,100000]
static void BM_vector_random_gen(benchmark::State& state) {
	for (auto _ : state) {
		std::random_device rd;
		std::mt19937 g(rd());
		uniform_int_distribution<> dist(-100000, 100000);
		for (int &n : candies)
			n = dist(g);
	}
	state.SetItemsProcessed(candies.size());
}
BENCHMARK(BM_vector_random_gen);

static const int VALUE_RANGE = 200001;
static const int OFFSET = 100000;
static void BM_vector_int(benchmark::State& state) { // vector<int> is 10x slower than vector<bool>
	int ans = 0;
	for (auto _ : state) {
		vector<int> exist(VALUE_RANGE, 0);
		size_t count = 0;
		for (int c : candies) {
			if (!exist[c + OFFSET]) {
				exist[c + OFFSET] = 1;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_vector_int);

static void BM_vector_bool_at(benchmark::State& state) { // using at is 10% faster than [], 
	int ans = 0;
	for (auto _ : state) {
		vector<bool> exist(VALUE_RANGE, false);
		size_t count = 0;
		for (int c : candies) {
			if (!exist.at(c + OFFSET)) {
				exist.at(c + OFFSET) = true;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_vector_bool_at);

static void BM_array_class_int(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		std::array<int, VALUE_RANGE> exist{};  // array<bool> is 2x faster than array<int>
		size_t count = 0;
		for (int c : candies) {
			if (!exist[c + OFFSET]) {
				exist[c + OFFSET] = 1;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_array_class_int);

static void BM_array_class_bool_at(benchmark::State& state) { // using at is slightly slower
	int ans = 0;
	for (auto _ : state) {
		std::array<bool, VALUE_RANGE> exist{};
		size_t count = 0;
		for (int c : candies) {
			if (!exist.at(c + OFFSET)) {
				exist.at(c + OFFSET) = true;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_array_class_bool_at);

static void BM_array_c(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		bool exist[VALUE_RANGE] = { false };
		size_t count = 0;
		for (int c : candies) {
			if (!exist[c + OFFSET]) {
				exist[c + OFFSET] = true;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_array_c);

static void BM_array_c_2(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		bool exist[VALUE_RANGE] = { false };
		size_t count = 0;
		for (int c : candies) {
			auto& loc = exist[c + OFFSET];
			if (!loc) {
				loc = true;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_array_c_2);

static void BM_bitset(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		bitset<VALUE_RANGE> exist;  // beat 31%
		size_t count = 0;
		for (int c : candies) {
			if (!exist.test(c + OFFSET)) {  // much faster to use test and set, than [], beat 89%
				exist.set(c + OFFSET, 1);
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
		bitset<VALUE_RANGE> exist;  // beat 31%
		size_t count = 0;
		for (int c : candies) {
			if (!exist[c + OFFSET]) {
				exist[c + OFFSET] = 1;
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
