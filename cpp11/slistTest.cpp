#include "..\catch.hpp"
#include <chrono>
#include <iostream>
#include "slist.h"
using slistSimple = slist<int>;
using namespace std;

TEST_CASE("slist single thread", "SLIST")
{
	slistSimple simple;
	simple.push_front(1);
	simple.push_front(2);
	simple.push_front(3);
	CHECK(*simple.peek() == 3);
	simple.pop_front();
	CHECK(*simple.peek() == 2);
}

struct MemoryTracker
{
	static int count;
	MemoryTracker() {
		count++;
	}
	MemoryTracker(const MemoryTracker&&) {
		count++;
	}
	~MemoryTracker()
	{
		--count;
	}
};
int MemoryTracker::count = 0;


/*
slist shared_ptr nano seconds: 136080100 count 2
slist unique_ptr nano seconds:  91674500 count 2
slist raw ptr nano seconds:     76840000 count 2
64 bit
slist shared_ptr nano seconds: 106155300 count 2
slist unique_ptr nano seconds:  81200800 count 2
slist raw ptr nano seconds:     67407500 count 2
*/

TEST_CASE("slist single thread memory tracker", "SLIST")
{
	using slistTracker = slist<MemoryTracker>;
	slistTracker simple;
	simple.push_front(MemoryTracker());  // copy constructor, move constructor
	simple.push_front(MemoryTracker());
	simple.push_front(MemoryTracker());
	CHECK(MemoryTracker::count == 3);
	simple.pop_front();
	CHECK(MemoryTracker::count == 2);

	long loops = 100000;
	auto perfTest = [](const char *name, auto func) {
		auto start = chrono::high_resolution_clock::now();
		long count = func();
		auto end = chrono::high_resolution_clock::now();
		auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
		cout << name << " nano seconds: " << nanos.count() << " count " << count << endl;

	};

	perfTest("slist shared_ptr", [loops]() {
		slistTracker simple;
		for (long i = 0; i < loops; i++) {
			simple.push_front(MemoryTracker());
			simple.pop_front();
		}
		return MemoryTracker::count;
	});

	using slistTrackerU = slist_u<MemoryTracker>;
	perfTest("slist unique_ptr", [loops]() {
		slistTrackerU simple;
		for (long i = 0; i < loops; i++) {
			simple.push_front(MemoryTracker());
			simple.pop_front();
		}
		return MemoryTracker::count;
	});

	using slistTrackerR = slist_r<false, MemoryTracker>;
	perfTest("slist raw ptr", [loops]() {
		slistTrackerR simple;
		for (long i = 0; i < loops; i++) {
			simple.push_front(MemoryTracker());
			simple.pop_front();
		}
		return MemoryTracker::count;
	});

	using slistTrackerRAtomic = slist_r<true, MemoryTracker>;
}
