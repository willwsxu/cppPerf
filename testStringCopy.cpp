#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <memory>
using namespace std;
#include "tests.h"

// string class performance
string copyString(const char *test)
{
	return string(test);
}

const int STR_SIZE = 11;
shared_ptr<char[STR_SIZE]> copyShare(const char *test)
{
	shared_ptr<char[STR_SIZE]> buf = make_shared<char[STR_SIZE]>();
	strncpy_s(*buf.get(), STR_SIZE, test, _TRUNCATE);
	return buf;
}

#include <benchmark/benchmark.h>

static void BM_memcpy(benchmark::State& state) {
	char* src = new char[state.range(0)];
	char* dst = new char[state.range(0)+1];
	memset(src, 'x', state.range(0));
	for (auto _ : state) {
		memcpy(dst, src, state.range(0));
		dst[state.range(0)] = 0;
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
	delete[] src;
	delete[] dst;
}
BENCHMARK(BM_memcpy)->Range(8, 8 << 10);
static void BM_strncpy(benchmark::State& state) {
	char* src = new char[state.range(0)];
	char* dst = new char[state.range(0) + 1];
	memset(src, 'x', state.range(0));
	for (auto _ : state) {
		strncpy_s(dst, state.range(0) + 1, src, _TRUNCATE);
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
	delete[] src;
	delete[] dst;
}
BENCHMARK(BM_strncpy)->Range(8, 8 << 10);

static void BM_strncpy_stack(benchmark::State& state) {
	char src[(8 << 10)];
	char dest[(8 << 10) + 1];
	memset(src, 'x', state.range(0));
	for (auto _ : state)
	{
		strncpy_s(dest, (8 << 10) + 1, src, state.range(0));
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_strncpy_stack)->Range(8, 8 << 10);

static void BM_strncpy_new_delete(benchmark::State& state) {
	char* src = new char[state.range(0)];
	memset(src, 'x', state.range(0));
	for (auto _ : state) {
		char* dst = new char[state.range(0) + 1];
		strncpy_s(dst, state.range(0) + 1, src, _TRUNCATE);
		delete[] dst;
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
	delete[] src;
}
BENCHMARK(BM_strncpy_new_delete)->Range(8, 8 << 10);

const char *pTestCase = "TEST123456";
static void BM_strncpy_free_store(benchmark::State& state) {
	for (auto _ : state)
	{
		char* dest=new char[STR_SIZE];
		strncpy_s(dest, STR_SIZE, pTestCase, _TRUNCATE);
	}
}
static void BM_copy_string(benchmark::State& state) {
	for (auto _ : state)
	{
		string temp = copyString(pTestCase);
	}
}

static void BM_create_shared_ptr_char_array(benchmark::State& state) {
	for (auto _ : state)
	{
		shared_ptr<char[STR_SIZE]> buf = make_shared<char[STR_SIZE]>();
	}
}
static void BM_copy_shared_ptr_char_array(benchmark::State& state) {
	for (auto _ : state)
	{
		auto temp2 = copyShare(pTestCase);
	}
}

//BENCHMARK(BM_strncpy_stack);						// 14ns
BENCHMARK(BM_strncpy_free_store);			// 76ns
BENCHMARK(BM_copy_string);					// 20ns
BENCHMARK(BM_create_shared_ptr_char_array); // 107ns
BENCHMARK(BM_copy_shared_ptr_char_array);	// 121ns
/*
BM_memcpy/8                                         6 ns          6 ns  112000000   1.24199GB/s
BM_memcpy/64                                        7 ns          7 ns  112000000   8.90096GB/s
BM_memcpy/512                                      31 ns         31 ns   22400000    15.191GB/s
BM_memcpy/4096                                    144 ns        148 ns    4977778    25.857GB/s
BM_memcpy/8192                                    210 ns        205 ns    3200000   37.2024GB/s
BM_strncpy/8                                       12 ns         12 ns   56000000   635.901MB/s
BM_strncpy/64                                      58 ns         57 ns   11200000   1067.07MB/s
BM_strncpy/512                                    360 ns        353 ns    1947826   1.35098GB/s
BM_strncpy/4096                                  3252 ns       3223 ns     213333   1.18371GB/s
BM_strncpy/8192                                  5496 ns       5469 ns     100000   1.39509GB/s
BM_strncpy_stack/8                                 16 ns         16 ns   44800000   475.543MB/s
BM_strncpy_stack/64                                80 ns         80 ns    8960000    760.87MB/s
BM_strncpy_stack/512                              531 ns        531 ns    1000000   919.118MB/s
BM_strncpy_stack/4096                            4136 ns       4143 ns     165926   942.761MB/s
BM_strncpy_stack/8192                            8255 ns       8196 ns      89600   953.191MB/s
BM_strncpy_new_delete/8                            89 ns         88 ns    7466667   86.8056MB/s
BM_strncpy_new_delete/64                          136 ns        138 ns    4977778   441.919MB/s
BM_strncpy_new_delete/512                         427 ns        433 ns    1659259   1.10079GB/s
BM_strncpy_new_delete/4096                       2807 ns       2825 ns     248889   1.35031GB/s
BM_strncpy_new_delete/8192                       5495 ns       5580 ns     112000   1.36719GB/s
BM_strncpy_free_store                              78 ns         78 ns    8960000
BM_copy_string                                     19 ns         18 ns   37333333
BM_create_shared_ptr_char_array                    96 ns         96 ns    7466667
BM_copy_shared_ptr_char_array                     109 ns        107 ns    6400000
*/