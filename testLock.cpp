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
	auto start = chrono::high_resolution_clock::now();
	long loops = 1000000;
	long count = 0;
	for (long i = 0; i < loops; i++)
		++count;
	auto end = chrono::high_resolution_clock::now();
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "count nano seconds: " << nanos.count() << " count " << count << endl;

	std::atomic<int> internalId(0);
	start = chrono::high_resolution_clock::now();
	for (long i = 0; i < loops; i++)
		++internalId;
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "atomic count nano seconds: " << nanos.count() << " count " << internalId << endl;

	start = chrono::high_resolution_clock::now();
	for (long i = 0; i < loops; i++)
		internalId++;
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "atomic post++ count nano seconds: " << nanos.count() << " count " << internalId << endl;

	start = chrono::high_resolution_clock::now();
	for (long i = 0; i < loops; i++)
		internalId.fetch_add(1, memory_order_relaxed);
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "atomic memory_order_relaxed count nano seconds: " << nanos.count() << " count " << internalId << endl;

	start = chrono::high_resolution_clock::now();
	for (long i = 0; i < loops; i++)
		internalId.fetch_add(1, memory_order_seq_cst);
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "atomic memory_order_seq_cst count nano seconds: " << nanos.count() << " count " << internalId << endl;

	count = 0;
	start = chrono::high_resolution_clock::now();
	for (long i = 0; i < loops; i++)
		(void)InterlockedIncrement(&count);
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "InterlockedIncrement count nano seconds: " << nanos.count() << " count " << count << endl;

	thread_local long local(0);
	start = chrono::high_resolution_clock::now();
	for (long i = 0; i < loops; i++)
		++local;
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "thread_local count nano seconds: " << nanos.count() << " count " << local << endl;

	std::mutex m;
	start = chrono::high_resolution_clock::now();
	for (long i = 0; i < loops; i++) {
		std::lock_guard<mutex> lock(m);
		++count;
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "mutex count nano seconds: " << nanos.count() << " count " << count << endl;

	tal::threading::CriticalSectionLock cs;
	start = chrono::high_resolution_clock::now();
	for (long i = 0; i < loops; i++) {
		tal::threading::CriticalSectionLock::scope lock(cs);
		++count;
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "criticalsection count nano seconds: " << nanos.count() << " count " << count << endl;

	start = chrono::high_resolution_clock::now();
	thread_data data;
	data.count = 0; data.loops = loops / 2;
	thread w1(worker, &data);
	thread w2(worker, &data);

	w1.join();
	w2.join();
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "mutex thread count nano seconds: " << nanos.count() << " count " << data.count << endl;
}