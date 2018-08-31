#pragma once

#include <string>
#include "library/string_util.h"

struct Str
{
	//Str(string s) :_s(move(s)) {}
	Str(string&& s) :_s(move(s)) {}

	Str& operator=(Str&& s) {
		_s = move(s._s);
		return *this;
	}
private:
	string _s;
};
Str g_str("");
// use initilizer, use const when possible
static void BM_redundant_construction(benchmark::State& state) {
	size_t sz = (size_t)state.range(0);
	auto s = memset_char('x', sz);
	char *src = s.get();
	for (auto _ : state)
	{
		string t(src);
		Str s1(move(t));
//		g_str = move(s1); // move construct
	}
}
BENCHMARK(BM_redundant_construction)->Range(8, 8 << 10);

static void BM_initializer_construction(benchmark::State& state) {
	size_t sz = (size_t)state.range(0);
	auto s = memset_char('x', sz);
	char *src = s.get();
	for (auto _ : state)
	{
		Str s1(move(string(src)));
//		g_str = move(s1); // move construct
	}
}
BENCHMARK(BM_initializer_construction)->Range(8, 8 << 10);

static void BM_initializer_construction_cstr(benchmark::State& state) {
	size_t sz = (size_t)state.range(0);
	auto s = memset_char('x', sz);
	char *src = s.get();
	for (auto _ : state)
	{
		Str s1(src);
//		g_str = move(s1);
	}
}
BENCHMARK(BM_initializer_construction_cstr)->Range(8, 8 << 10);
/*
BENCHMARK(BM_redundant_construction);  // 51ns
BENCHMARK(BM_redundant_construction_append);  // 50ns
BENCHMARK(BM_initializer_construction_append); // 49ns
BENCHMARK(BM_initializer_construction_append_no_move); //55ns
BENCHMARK(BM_initializer_construction);// 68ns, extra malloc, // + is more costly then +=
*/
/*  redundant construction has small overhead
BM_redundant_construction/8                   42 ns         42 ns   16592593
BM_redundant_construction/64                 158 ns        160 ns    4480000
BM_redundant_construction/512                395 ns        392 ns    1792000
BM_redundant_construction/4096              2233 ns       2246 ns     320000
BM_redundant_construction/8192              4396 ns       4349 ns     154483
BM_initializer_construction/8                 41 ns         40 ns   16592593
BM_initializer_construction/64               158 ns        157 ns    4072727
BM_initializer_construction/512              390 ns        392 ns    1792000
BM_initializer_construction/4096            2233 ns       2246 ns     320000
BM_initializer_construction/8192            4366 ns       4395 ns     160000
BM_initializer_construction_cstr/8            35 ns         34 ns   19478261
BM_initializer_construction_cstr/64          157 ns        157 ns    4480000
BM_initializer_construction_cstr/512         384 ns        384 ns    1792000
BM_initializer_construction_cstr/4096       2248 ns       2246 ns     320000
BM_initializer_construction_cstr/8192       4327 ns       4297 ns     160000
*/