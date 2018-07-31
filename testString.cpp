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

const char *pTestCase = "TEST123456";
static void BM_strncpy(benchmark::State& state) {
	for (auto _ : state)
	{
		char dest[STR_SIZE];
		strncpy_s(dest, STR_SIZE, pTestCase, _TRUNCATE);
	}
}
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

BENCHMARK(BM_strncpy);						// 14ns
BENCHMARK(BM_strncpy_free_store);			// 76ns
BENCHMARK(BM_copy_string);					// 20ns
BENCHMARK(BM_create_shared_ptr_char_array); // 107ns
BENCHMARK(BM_copy_shared_ptr_char_array);	// 121ns