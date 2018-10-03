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

/* performance comparison
   ITEMS	10000	100000	1000000	10000000
   SlistEx	342		389		497		cannot finish
   slist_r	388		418		383		382
   circ_queue				292		294
*/
static const long ITEMS = 10000000;
string test = "AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEEFFFFFFFFFFGGGGGGGGGGHHHHHHHHHHIIIIIIIIIIJJJJJJJJJJ";
void producer1()
{
	for (int i = 0; i < ITEMS; i++)
	{
		std::lock_guard<mutex> lock(m);
		queue.push_back(to_string(i) + " " + test);
	}
	cout << "producer1 count " << ITEMS << "\n";
}

void consumer1(int total)
{
	auto start = chrono::high_resolution_clock::now();
	int count = 0;
	int sleep_count = 0;
	while (count < total) {
		list<string>  queue2;
		{
			std::lock_guard<mutex> lock(m);
			if (queue.empty()) {
				sleep_count++;
				continue;
			}
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
	cout << nanos.count()/ count << endl;
	cout << "std double list consumer1 sleep count " << sleep_count << "\n";
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
void producer2(int total)
{
	for (int i = 0; i < total; i++)
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
	cout << "producer2 count " << total << "\n";
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
		getter.clear();
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> span = end - start;
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << nanos.count()/count << endl;
	cout << "MS slist consumer2 sleep count " << sleep_count << "\n";
}

void testSlist()
{
	thread prod(producer2, ITEMS);
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
		atomic_str_list.push_front( to_string(i+1) + " " + test);
	}
	cout << "producer3 count " << total << "\n";
}

void consumer3(int total)
{
	auto start = chrono::high_resolution_clock::now();
	int count = 0;
	int sleep_count = 0;
	while (count < total && sleep_count<total) {
		if (atomic_str_list.peek()) {
			//auto x=atomic_str_list.pop_front();  // lost some items
			auto *head = atomic_str_list.pop_all();
			head = slist_r<string, atomic<bool*>>::reverse(head).first;
			auto *node = head;
			while (node) {
				if (++count != stoi(node->data))
					cout << node->data << " expect " << count << "\n";
				node = node->next;
			}
			delete head;
		} else {
			Sleep(0);
			sleep_count++;
		}
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> span = end - start;
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << nanos.count()/count << " ns\n";
	cout << "atomic slist consumer3 sleep count " << sleep_count << " count=" << count << "\n";
}

void testAtomicSlist()
{
	thread prod(producer3, ITEMS);
	thread cons(consumer3, ITEMS);

	prod.join();
	cons.join();
}

#include "RingBufferAtomic.h"
RingBuffer<string, 8096> lfQ;
void producer4(int total)
{
	int sleep_count = 0;
	for (int i = 0; i < total; i++)
	{
		while (lfQ.push(to_string(i + 1) + " " + test) == false) {
			Sleep(0);
			sleep_count++;
		}
	}
	cout << "producer4 count " << total << " sleep count " << sleep_count<< "\n";
}


void consumer4(int total)
{
	auto start = chrono::high_resolution_clock::now();
	int count = 0;
	int sleep_count = 0;
	while (count < total && sleep_count<total) {
		auto x = lfQ.pop();
		if (x.second) {
			if (++count != stoi(x.first))
				std::cout << x.first << " expect " << count << "\n";
		}
		else {
			Sleep(0);
			sleep_count++;
		}
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> span = end - start;
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	std::cout << nanos.count() / count << " ns\n";
	std::cout << "atomic queue consumer4 sleep count " << sleep_count << " count=" << count << "\n";
}

void testAtomicQueue()
{
	thread prod(producer4, ITEMS);
	thread cons(consumer4, ITEMS);

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