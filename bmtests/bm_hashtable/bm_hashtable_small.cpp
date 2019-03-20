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
	char name[8];  // key
	char pass[8];
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

using BaseHash = HASH_TABLE_NEW<Console, Large_Data, FH_HASH_TABLE_BASE, FileNone, HashFun>;
BaseHash baseHash(10);
vector<Large_Data> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };

using BuckHash = HASH_TABLE_NEW<Console, Large_Data, HASH_TABLE_BUCKET, FileNone, HashFun>;
BuckHash buckHash(10);

using ProbeHash = HASH_TABLE_NEW<Console, Large_Data, HASH_TABLE_PROBING, FileNone, HashFun>;
ProbeHash probHash(10);

set<Large_Data> ordered{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
unordered_set<Large_Data> unordered{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
static void BM_stl_set_create(benchmark::State& state) {
	for (auto x : simple) {
		baseHash.insert(x);
		buckHash.insert(x);
		probHash.insert(x);
	}
	for (int i = 0; i < 5000; i++) {
		string key = "test" + to_string(i);
		baseHash.insert(Large_Data(key.c_str()));
		buckHash.insert(Large_Data(key.c_str()));
		probHash.insert(Large_Data(key.c_str()));
		ordered.insert(Large_Data(key.c_str()));
		unordered.insert(Large_Data(key.c_str()));
	}
	for (auto _ : state)
	{
		set<Large_Data> simple{ "XXX", "OEU", "OGB", "OGFX", "OGTI", "OEM1", "OEM2", "OEM3", "OEM4","OOAT","OBTP" };
	}
	cout << ordered.size() << " " << ordered.size() << " " << probHash.size() << " " << buckHash.size() << " " << baseHash.size()<<endl;
}	

static void BM_stl_set(benchmark::State& state) {
	long found = 0;
	for (auto _ : state)
	{
		if (ordered.find("OGB") != ordered.end())
			++found;
		if (ordered.find("OGBS") != ordered.end())
			++found;
		if (ordered.find("OBTP") != ordered.end())
			++found;
		if (ordered.find("OEM5") != ordered.end())
			++found;
	}
	state.SetItemsProcessed(found);
}

static void BM_stl_unordered_set(benchmark::State& state) {
	long found = 0;
	for (auto _ : state)
	{
		if (unordered.find("OGB") != unordered.end())
			++found;
		if (unordered.find("OGBS") != unordered.end())
			++found;
		if (unordered.find("OBTP") != unordered.end())
			++found;
		if (unordered.find("OEM5") != unordered.end())
			++found;
	}
	state.SetItemsProcessed(found);
}

static void BM_stl_myhash(benchmark::State& state) {
	long found = 0;
	for (auto _ : state)
	{
		if (baseHash.find("OGB"))
			++found;
		if (baseHash.find("OGBS"))
			++found;
		if (baseHash.find("OBTP"))
			++found;
		if (baseHash.find("OEM5"))
			++found;
	}
	state.SetItemsProcessed(found);
}

static void BM_stl_buckethash(benchmark::State& state) {
	long found = 0;
	for (auto _ : state)
	{
		if (buckHash.find("OGB")!= buckHash.end())
			++found;
		if (buckHash.find("OGBS") != buckHash.end())
			++found;
		if (buckHash.find("OBTP") != buckHash.end())
			++found;
		if (buckHash.find("OEM5") != buckHash.end())
			++found;
	}
	state.SetItemsProcessed(found);
}

static void BM_stl_probinghash(benchmark::State& state) {
	long found = 0;
	for (auto _ : state)
	{
		if (probHash.find("OGB") != probHash.end())
			++found;
		if (probHash.find("OGBS") != probHash.end())
			++found;
		if (probHash.find("OBTP") != probHash.end())
			++found;
		if (probHash.find("OEM5") != probHash.end())
			++found;
	}
	state.SetItemsProcessed(found);
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