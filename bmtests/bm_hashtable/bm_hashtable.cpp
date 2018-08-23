#include <benchmark/benchmark.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "ConsoleLogger.h"
#include "HashTableBase.h"
#include "HashTableBucketHashing.h"
#include "HashTable.h"
#include <unordered_set>
using namespace std;

static void BM_stl_set_create(benchmark::State& state) {
	for (auto _ : state)
	{
		set<string> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
	}
}
static void BM_stl_set(benchmark::State& state) {
	set<string> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
	long found = 0;
	for (auto _ : state)
	{
		if (simple.find("OGB") != simple.end())
			++found;
		if (simple.find("OGBS") != simple.end())
			++found;
		if (simple.find("OBTP") != simple.end())
			++found;
		if (simple.find("OEM5") != simple.end())
			++found;
	}
	//	std::cout << found << std::endl;
}

static void BM_stl_unordered_set(benchmark::State& state) {
	unordered_set<string> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
	long found = 0;
	for (auto _ : state)
	{
		if (simple.find("OGB") != simple.end())
			++found;
		if (simple.find("OGBS") != simple.end())
			++found;
		if (simple.find("OBTP") != simple.end())
			++found;
		if (simple.find("OEM5") != simple.end())
			++found;
	}
	//	std::cout << found << std::endl;
}


static void BM_stl_myhash(benchmark::State& state) {
	using StrHash = HASH_TABLE_NEW<Console, string, FH_HASH_TABLE_BASE, FileNone, HashFun>;
	StrHash t(10);
	vector<const char*> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
	for (auto x : simple)
		t.insert(x);
	long found = 0;
	for (auto _ : state)
	{
		if (t.find("OGB"))
			++found;
		if (t.find("OGBS"))
			++found;
		if (t.find("OBTP"))
			++found;
		if (t.find("OEM5"))
			++found;
	}
	//	std::cout << found << std::endl;
}

static void BM_stl_buckethash(benchmark::State& state) {
	using StrHash = HASH_TABLE_NEW<Console, string, HASH_TABLE_BUCKET, FileNone, HashFun>;
	StrHash t(10);
	vector<const char*> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
	for (auto x : simple)
		t.insert(x);
	long found = 0;
	for (auto _ : state)
	{
		if (t.find("OGB")!=t.end())
			++found;
		if (t.find("OGBS") != t.end())
			++found;
		if (t.find("OBTP") != t.end())
			++found;
		if (t.find("OEM5") != t.end())
			++found;
	}
	//	std::cout << found << std::endl;
}
static void BM_wait(benchmark::State& state) {
	using namespace std::chrono_literals;
	for (auto _ : state)
		std::this_thread::sleep_for(300ms);
}

BENCHMARK(BM_stl_set_create);
BENCHMARK(BM_stl_set);
BENCHMARK(BM_stl_unordered_set);
BENCHMARK(BM_stl_myhash);
BENCHMARK(BM_stl_buckethash);
//BENCHMARK(BM_wait);
BENCHMARK_MAIN();
