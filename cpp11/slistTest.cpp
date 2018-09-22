#include "..\catch.hpp"
#include <chrono>
#include <iostream>
#include "slist.h"
using slist_shared_ptr = slist<int, std::shared_ptr>;
using namespace std;

TEST_CASE("slist single thread shared_ptr", "SLIST")
{
	slist_shared_ptr simple;
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
	MemoryTracker(const MemoryTracker&) {
		count++;
	}
	MemoryTracker(MemoryTracker&&) {
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
	using slistTracker = slist<MemoryTracker, std::shared_ptr>;
	slistTracker simple;
	simple.push_front(MemoryTracker());  // copy constructor, move constructor
	simple.push_front(MemoryTracker());
	simple.push_front(MemoryTracker());
	CHECK(MemoryTracker::count == 3);
	simple.pop_front();
	CHECK(MemoryTracker::count == 2);

	using slistTrackerU = slist_u<MemoryTracker>;
	slistTrackerU unique;
	unique.push_front(MemoryTracker());
	unique.pop_front();
	
	using slistTrackerR = slist_r<MemoryTracker, bool*>;
	slistTrackerR raw;
	raw.push_front(MemoryTracker());
	raw.pop_front();
	
	using slistTrackerRAtomic = slist_r<MemoryTracker, std::atomic<bool>>;
	slistTrackerRAtomic x;
	x.push_front(MemoryTracker());
	x.pop_front();
}
