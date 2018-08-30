#include <benchmark/benchmark.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "ConsoleLogger.h"
#include "HashTableBase.h"
#include "HashTableBucketHashing.h"
#include "HashTableProbing.h"
#include "HashTable.h"
#include <unordered_set>
using namespace std;

struct Large_Data
{
	char name[16];  // key
	char pass[16];
	uint64_t	id;
	uint64_t	values[3];
	Large_Data(const char *n) {
		strncpy_s(name, sizeof(name), n, _TRUNCATE);
	}
	Large_Data() {
		name[0]=0;
	}
};
bool operator==(const Large_Data &lhs, const Large_Data &rhs)
{
	return strcmp(lhs.name, rhs.name) == 0;
}
bool operator!=(const Large_Data &lhs, const Large_Data &rhs)
{
	return strcmp(lhs.name, rhs.name) != 0;
}
template<>
struct HashFun<Large_Data>
{
	size_t operator()(const Large_Data& t) {
		return std::hash<string>{}(t.name);
	}
};
namespace std
{
	template<>
	struct hash<Large_Data>
	{
		size_t operator()(const Large_Data& t) const {
			return std::hash<string>{}(t.name);
		}
	};
	template<>
	struct equal_to<Large_Data>
	{
		bool operator()(const Large_Data &lhs, const Large_Data &rhs) const {
			return lhs==rhs;
		}
	};

	template<>
	struct less<Large_Data> {
		bool operator()(const Large_Data &lhs, const Large_Data &rhs) const
		{
			return strcmp(lhs.name, rhs.name)<0;
		}
	};
}
static void BM_stl_set_create(benchmark::State& state) {
	for (auto _ : state)
	{
		set<Large_Data> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
	}
}
static void BM_stl_set(benchmark::State& state) {
	set<Large_Data> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
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
	unordered_set<Large_Data> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
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
	using StrHash = HASH_TABLE_NEW<Console, Large_Data, FH_HASH_TABLE_BASE, FileNone, HashFun>;
	StrHash t(10);
	vector<Large_Data> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
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
	using StrHash = HASH_TABLE_NEW<Console, Large_Data, HASH_TABLE_BUCKET, FileNone, HashFun>;
	StrHash t(10);
	vector<Large_Data> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
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

static void BM_stl_probinghash(benchmark::State& state) {
	using ProbeHash = HASH_TABLE_NEW<Console, Large_Data, HASH_TABLE_PROBING, FileNone, HashFun>;
	ProbeHash t(10);
	vector<Large_Data> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
	for (auto x : simple)
		t.insert(x);
	long found = 0;
	for (auto _ : state)
	{
		if (t.find("OGB") != t.end())
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
BENCHMARK(BM_stl_probinghash);
/*
BM_stl_set                   43 ns         44 ns   16000000
BM_stl_unordered_set        195 ns        195 ns    3200000
BM_stl_myhash               319 ns        321 ns    2240000
BM_stl_buckethash           165 ns        164 ns    4480000
*/