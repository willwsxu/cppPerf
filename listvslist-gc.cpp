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
/* test with 1000000 iteration, repeat 4 time
   Baseline		259		263		256		258		259.0
   Atom Slist	366		381		380		353		370.0
   Atom queue	282		303		295		279		289.8
*/
string test = "AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDDEEEEEEEEEEFFFFFFFFFFGGGGGGGGGGHHHHHHHHHHIIIIIIIIIIJJJJJJJJJJ";
void producer1(int total)
{
	for (int i = 0; i < total; i++)
	{
		std::lock_guard<mutex> lock(m);
		queue.push_back(to_string(i) + " " + test);
	}
	cout << "producer1 count " << total << "\n";
}

template<typename Fun>
void benchmark_simple(Fun test, const char * fun_name, int total) {

	auto start = chrono::high_resolution_clock::now();

	int sleep_count = test(total);

	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> span = end - start;
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	std::cout << nanos.count() / total << endl;
	std::cout << fun_name << " sleep count " << sleep_count << " count=" << total << "\n";
}
void consumer1(int total)
{
	auto test = [](int total) {
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
			for (auto iter = queue2.begin(); iter != queue2.end(); iter++) {
				int id = atol(iter->c_str());
				assert(id == count);
				count++;
			}
		}
		return sleep_count;
	};
	benchmark_simple(test, "std double list consumer1", total);
}

template<typename Producer, typename Consumer>
void test_producer_consumer_single(Producer producer, Consumer consumer)
{
	static const long ITEMS = 1000000;
	thread prod(producer, ITEMS);
	thread cons(consumer, ITEMS);

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
	auto test = [](int total) {
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
		return sleep_count;
	};
	benchmark_simple(test, "MS slist consumer2", total);
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
	auto test = [](int total) {
		int count = 0;
		int sleep_count = 0;
		while (count < total && sleep_count < total) {
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
			}
			else {
				Sleep(0);
				sleep_count++;
			}
		}
		return sleep_count;
	};
	benchmark_simple(test, "atomic slist consumer3", total);
}


#include "RingBufferAtomic.h"
circular_queue<string, 8096> lfQ;
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
	auto test = [](int total) {
		int count = 0;
		int sleep_count = 0;
		while (count < total && sleep_count < total) {
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
		return sleep_count;
	};
	benchmark_simple(test, "atomic queue consumer4", total);
}

/*
 queue_len=8096*2, producer sleep count=1200, no consumer sleep count, time 90 ns
 queue_len=8096*16, producer o to some sleep count, consumer sleep count 1000, time 83 ns
 performance about same for int or pair of 64 bit int
*/
circular_queue<pair<int64_t,int64_t>, 8096*16> lfQi;
void producer5(int total)
{
	int sleep_count = 0;
	for (int i = 0; i < total; i++)
	{
		while (lfQi.push({ i + 1, i + 1 }) == false) {
			Sleep(0);
			sleep_count++;
		}
	}
	cout << "producer5 count " << total << " sleep count " << sleep_count << "\n";
}


void consumer5(int total)
{
	auto test = [](int total) {
		int count = 0;
		int sleep_count = 0;
		while (count < total && sleep_count < total) {
			auto x = lfQi.pop();
			if (x.second) {
				if (++count != x.first.first)
					std::cout << x.first.first << " expect " << count << "\n";
			}
			else {
				Sleep(0);
				sleep_count++;
			}
		}
		return sleep_count;
	};
	benchmark_simple(test, "atomic queue consumer5 long", total);
}
void testList1()  // std::list with mutex
{
	test_producer_consumer_single(producer1, consumer1);
}
void testSlist()  // SlistEx, with gc, lock free
{
	test_producer_consumer_single(producer2, consumer2);
}
void testAtomicSlist()  // atomic slist
{
	test_producer_consumer_single(producer3, consumer3);
}
void testAtomicQueue()  // atomic queue circular
{
	test_producer_consumer_single(producer4, consumer4);
}
void testAtomicQueueInt()  // atomic queue circular
{
	test_producer_consumer_single(producer5, consumer5);
}

void testBaseline()  // string allocation
{
	auto baseline = [](int total) {
		int count = 0;
		while (count < total ) {
			string x = to_string(count + 1) + " " + test;
			if (++count != stoi(x))
				std::cout << x << " expect " << count << "\n";
		}
		return 0;
	};
	benchmark_simple(baseline, "string op baseline", 1000000);  // 262 ns

	auto baseline_copy = [](int total) {
		int count = 0;
		while (count < total) {
			string x = to_string(count + 1) + " " + test;
			string y = x;
			if (++count != stoi(y))
				std::cout << x << " expect " << count << "\n";
		}
		return 0;
	};
	benchmark_simple(baseline_copy, "string op baseline+copy", 1000000);  // 367.5 ns

	auto baseline_move = [](int total) {
		int count = 0;
		while (count < total) {
			string x = to_string(count + 1) + " " + test;
			string y = move(x);
			if (++count != stoi(y))
				std::cout << x << " expect " << count << "\n";
		}
		return 0;
	};
	benchmark_simple(baseline_move, "string op baseline+move", 1000000);  // 265.5


	auto baseline_queue = [](int total) {
		int count = 0;
		array<string, 2> queue;
		while (count < total) {
			queue[0] = move(to_string(count + 1) + " " + test);
			if (++count != stoi(queue[0]))
				std::cout << queue[0] << " expect " << count << "\n";
		}
		return 0;
	};
	benchmark_simple(baseline_queue, "single thread queue push", 1000000);  // 267
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