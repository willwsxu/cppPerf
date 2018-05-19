#include "stdafx.h"
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include "CriticalSectionLock.h"

using namespace std;

/*
count nano seconds :                 0         count 10000000
atomic count nano seconds :          62000000  count 10000000
atomic post++ count nano seconds :   61000000  count 20000000
mutex count nano seconds :           468000000 count 20000000
criticalsection count nano seconds : 209000000 count 30000000
*/
struct thread_data
{
	int count;
	std::mutex m;
	int loops;
};

void worker(thread_data *pData)
{
	for (int i = 0; i < pData->loops; i++)
	{
		std::lock_guard<mutex> lock(pData->m);
		pData->count++;
	}
}
void testLocks()
{
	long loops = 1000000;
	auto perfTest = [](const char *name, auto func) {
		auto start = chrono::high_resolution_clock::now();
		long count = func();
		auto end = chrono::high_resolution_clock::now();
		auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
		cout << name << " nano seconds: " << nanos.count() << " count " << count << endl;

	};

	perfTest("regular long", [loops]() {
		long count = 0;
		for (long i = 0; i < loops; i++)
			++count;
		return count;
	});

	perfTest("thread_local", [loops]() {
		thread_local long local(0);
		for (long i = 0; i < loops; i++)
			++local;
		return local;
	});

	perfTest("atomic long pre++", [loops]() {
		std::atomic<long> internalId(0);
		for (long i = 0; i < loops; i++)
			++internalId;
		return internalId.load();
	});

	perfTest("atomic long post++", [loops]() {
		std::atomic<long> internalId(0);
		for (long i = 0; i < loops; i++)
			internalId++;
		return internalId.load();
	});

	perfTest("atomic memory_order_relaxed", [loops]() {
		std::atomic<long> internalId(0);
		for (long i = 0; i < loops; i++)
			internalId.fetch_add(1, memory_order_relaxed);
		return internalId.load();
	});

	perfTest("atomic memory_order_seq_cst", [loops]() {
		std::atomic<long> internalId(0);
		for (long i = 0; i < loops; i++)
			internalId.fetch_add(1, memory_order_seq_cst);
		return internalId.load();
	});

	perfTest("InterlockedIncrement", [loops]() {
		long count = 0;
		for (long i = 0; i < loops; i++)
			(void)InterlockedIncrement(&count);
		return count;
	});

	perfTest("criticalsection", [loops]() {
		long count = 0;
		tal::threading::CriticalSectionLock cs;
		for (long i = 0; i < loops; i++) {
			tal::threading::CriticalSectionLock::scope lock(cs);
			++count;
		}
		return count;
	});

	perfTest("mutex", [loops]() {
		long count = 0;
		std::mutex m;
		for (long i = 0; i < loops; i++) {
			std::lock_guard<mutex> lock(m);
			++count;
		}
		return count;
	});

	auto start = chrono::high_resolution_clock::now();
	thread_data data;
	data.count = 0; data.loops = loops / 2;
	thread w1(worker, &data);
	thread w2(worker, &data);

	w1.join();
	w2.join();
	auto end = chrono::high_resolution_clock::now();
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "mutex thread count nano seconds: " << nanos.count() << " count " << data.count << endl;
}