#include "stdafx.h"
#include "SlistEx.h"
#include <thread>
#include <list>
#include <string>
#include <mutex>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <numeric>

using namespace std;
list<string>  queue;
std::mutex m;

static const long ITEMS = 10000;
string test = "AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEEFFFFFFFFFFGGGGGGGGGGHHHHHHHHHHIIIIIIIIIIJJJJJJJJJJ";
void producer1()
{
	for (int i = 0; i < ITEMS; i++)
	{
		std::lock_guard<mutex> lock(m);
		queue.push_back(to_string(i) + " " + test);
	}
}

void consumer1(int total)
{
	auto start = chrono::high_resolution_clock::now();
	int count = 0;
	while (count < total) {
		list<string>  queue2;
		{
			std::lock_guard<mutex> lock(m);
			if (queue.empty())
				continue;
			queue2.assign(begin(queue), end(queue));
			queue.clear();
		}
		for (auto iter=queue2.begin(); iter != queue2.end(); iter++) {
			int id = atol(iter->c_str());
			assert(id == count);
			count++;
		}
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> span = end - start;
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << span.count() << endl;
	cout << nanos.count() << endl;
}

void testList1()
{
	thread prod(producer1);
	thread cons(consumer1, ITEMS);

	prod.join();
	cons.join();
}


const unsigned int cMaxElements = 1000000;
typedef SListItem<string>  MySListItem;
typedef MemoryRecycler<string, cMaxElements / 4, cMaxElements> MyRecycler;
typedef SListEx<string, MyRecycler> MySlist;
typedef std::list<MySListItem *> MySlistGetter;

MySlist stringList;
void producer2()
{
	for (int i = 0; i < ITEMS; i++)
	{
		MySListItem *pItem = stringList.getGC().GetNewItem();
		if (!pItem) {
			Sleep(0);
			continue;
		}
		pItem->data = to_string(i) + " " + test;
		stringList.PushItem(pItem);
		pItem = nullptr;
	}
}

void consumer2(int total)
{
	auto start = chrono::high_resolution_clock::now();
	MySlistGetter getter;
	int count = 0;
	int sleep_count = 0;
	while (count < total) {
		stringList.GetItems(getter);
		if (getter.empty()) {
			Sleep(0);
			sleep_count++;
			continue;
		}
		for (auto riter = getter.rbegin(); riter != getter.rend(); riter++)
		{
			MySListItem *pItem = *riter;
			int id = atol(pItem->data.c_str());
			assert(id == count);
			count++;
		}
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> span = end - start;
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << span.count() << endl;
	cout << nanos.count() << endl;
	cout << "consumer2 sleep count " << sleep_count << "\n";
}

void testSlist()
{
	thread prod(producer2);
	thread cons(consumer2, ITEMS);

	prod.join();
	cons.join();
}

#include "..\cpp11\slist.h"
slist_r<string, atomic<bool*>> atomic_str_list;
void producer3(int total)
{
	for (int i = 0; i < total; i++)
	{
		atomic_str_list.push_front( to_string(i) + " " + test);
	}
	cout << "producer3 count " << total << "\n";
}

void consumer3(int total)
{
	auto start = chrono::high_resolution_clock::now();
	int count = 0;
	int sleep_count = 0;
	while (count < total) {
		if (atomic_str_list.peek()) {
			(void)atomic_str_list.pop_front();
			count++;
		} else {
			Sleep(0);
			sleep_count++;
			continue;
		}
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> span = end - start;
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << nanos.count()/count << " ns\n";
	cout << "consumer3 sleep count " << sleep_count << "\n";
}

void testAtomicSlist()
{
	thread prod(producer3, ITEMS);
	thread cons(consumer3, ITEMS);

	prod.join();
	cons.join();
}


long long millisec[32] = { 0 };
void worker(int id, int loops)
{
	auto start = chrono::high_resolution_clock::now();
	long ans = 0;
	for (int i = 1; i <= loops; i++) {
		long *t = new long;
		*t = i*i;
		ans ^= *t;
	}
	auto end = chrono::high_resolution_clock::now();
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	millisec[id] = nanos.count() / 1000000;
	cout << "worker thread count milli seconds: " << nanos.count()/1000000 << " loops " << loops << " ans " << ans << endl;
}

// On a machine with 8 cores, no difference to
// 32 thread 1406 msec
// 16 threads 1472 msec
// 8  threads 1381
void testThreads()
{
	unsigned long loops = 200000000;
	int cpu = 8;
	thread *t[32];
	for (int i = 0; i < cpu; i++) {
		t[i] = new thread(worker, i, loops / cpu);
	}
	for (int i = 0; i < cpu; i++)
		t[i]->join();
	long long total = std::accumulate(millisec, millisec + cpu, (long long)0);
	cout << " total msec " << total << endl;
}