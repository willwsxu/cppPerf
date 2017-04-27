#include "stdafx.h"
#include <thread>
#include <list>
#include <string>
#include <mutex>
#include <assert.h>
#include <chrono>
#include <iostream>

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


#include "SlistEx.h"
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
	while (count < total) {
		stringList.GetItems(getter);
		if (getter.empty()) {
			Sleep(0);
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
}

void testSlist()
{
	thread prod(producer2);
	thread cons(consumer2, ITEMS);

	prod.join();
	cons.join();
}
