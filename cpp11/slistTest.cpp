#include "..\catch.hpp"
#include <chrono>
#include <iostream>
#include "slist.h"
using slist_shared_ptr = slist<int>;
using namespace std;

TEST_CASE("slist single thread shared_ptr", "NEW")
{
	slist_shared_ptr simple;
	simple.push_front(1);
	simple.push_front(2);
	simple.push_front(3);
	CHECK(*simple.peek() == 3);
	simple.pop_front();
	CHECK(*simple.peek() == 2);
}

TEST_CASE("slist single thread unique_ptr", "[NEW]")
{
	using slist_unique_ptr = slist<int, false>;
	slist_unique_ptr simple;
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
	const int myCount;
	MemoryTracker():myCount(++count){	
	}
	MemoryTracker(const MemoryTracker&) :myCount(++count) {	
	}
	MemoryTracker(MemoryTracker&& x) :myCount(x.myCount) { 
		count++; 
	}  // move ctor take the ID, old temp still calls destructor after move
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

TEST_CASE("slist single thread memory tracker share_ptr", "[NEW]")
{
	using slistTracker = slist<MemoryTracker>;
	slistTracker simple;
	MemoryTracker mt;  // 
	simple.push_front(mt);  // pass by ref
	CHECK(simple.peek()->myCount == 2);
	simple.push_front(MemoryTracker());  // move
	CHECK(simple.peek()->myCount == 3);
	simple.push_front(MemoryTracker());
	CHECK(MemoryTracker::count == 4);
	CHECK(simple.peek()->myCount == 4);
	simple.pop_front();
	CHECK(simple.peek()->myCount == 3);
	CHECK(MemoryTracker::count == 3);
}
TEST_CASE("slist single thread memory tracker unique_ptr", "[NEW]")
{
	using slistTrackerU = slist<MemoryTracker, false>;
	slistTrackerU unique;
	MemoryTracker mt;
	unique.push_front(mt);  // pass by ref
	unique.push_front(MemoryTracker());  // move
	unique.push_front(MemoryTracker());
	CHECK(MemoryTracker::count == 4);
	CHECK(unique.peek()->myCount == 4);
	unique.pop_front();
	CHECK(unique.peek()->myCount == 3);
	CHECK(MemoryTracker::count == 3);
}
TEST_CASE("slist single thread memory tracker raw ptr", "[NEW]")
{
	using slistTrackerR = slist_r<MemoryTracker, bool*>;
	slistTrackerR raw;
	MemoryTracker mt;
	raw.push_front(mt);  // pass by ref
	raw.push_front(MemoryTracker());  // move
	raw.push_front(MemoryTracker());
	CHECK(MemoryTracker::count == 4);
	CHECK(raw.peek()->myCount == 4);
	raw.pop_front();
	CHECK(raw.peek()->myCount == 3);
	CHECK(MemoryTracker::count == 3);
}
TEST_CASE("slist single thread memory tracker atomic raw ptr", "[NEW]")
{
	using slistTrackerRAtomic = slist_r<MemoryTracker, std::atomic<bool>>;
	slistTrackerRAtomic x;
	MemoryTracker mt;
	x.push_front(mt);  // pass by ref
	x.push_front(MemoryTracker());  // move
	x.push_front(MemoryTracker());
	CHECK(MemoryTracker::count == 4);
	CHECK(x.peek()->myCount == 4);
	x.pop_front();
	CHECK(x.peek()->myCount == 3);
	CHECK(MemoryTracker::count == 3);
}
