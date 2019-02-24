#include "..\catch.hpp"
#include <chrono>
#include <iostream>
#include <future>
#include "slist.h"
using slist_shared_ptr = slist<int>;
using namespace std;

TEST_CASE("slist single thread shared_ptr", "[QUEUE]")
{
	slist_shared_ptr simple;
	simple.push_front(1);
	simple.push_front(2);
	simple.push_front(3);
	CHECK(*simple.peek() == 3);
	simple.pop_front();
	CHECK(*simple.peek() == 2);
}

TEST_CASE("slist single thread unique_ptr", "[QUEUE]")
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

TEST_CASE("slist single thread memory tracker share_ptr", "[QUEUE]")
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
TEST_CASE("slist single thread memory tracker raw ptr", "[QUEUE]")
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
TEST_CASE("slist single thread memory tracker atomic raw ptr", "[QUEUE]")
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


#include "RingBufferAtomic.h"
TEST_CASE("atomic queue", "[QUEUE]")
{
	circular_queue<size_t, 3> lrQ; // lock free queue
	CHECK(lrQ.push(1));
	CHECK(lrQ.push(2));
	CHECK(lrQ.push(3));
	CHECK(lrQ.push(4)==false);
	CHECK(lrQ.size()==3);
	CHECK(lrQ.pop() == pair<size_t,bool>{1, true});
	CHECK(lrQ.push(4) == true);
	CHECK(lrQ.push(5) == false);
	CHECK(lrQ.size() == 3);
	CHECK(lrQ.pop() == pair<size_t, bool>{2, true});
	CHECK(lrQ.push(5) == true);  // wrap writer to position 0
	CHECK(lrQ.size() == 3);
	CHECK(lrQ.pop() == pair<size_t, bool>{3, true});
	CHECK(lrQ.pop() == pair<size_t, bool>{4, true});
	CHECK(lrQ.size() == 1);
	CHECK(lrQ.pop() == pair<size_t, bool>{5, true});
	CHECK(lrQ.empty() == true);
	CHECK(lrQ.size() == 0);
	CHECK(lrQ.pop() == pair<size_t, bool>{0, false});
	CHECK(lrQ.push(6) == true);
}

using QL3000 = circular_queue<size_t, 3000>;
void lock_free_Q_consumer(QL3000& lrq, size_t total, std::promise<int> ret)
{
	int error = 0;
	for (size_t i = 0; i < total; i++) {
		if (lrq.pop_wait()!=i)
			error++;
	}
	ret.set_value(error);
}
TEST_CASE("atomic queue test events", "[QUEUE]")
{
	QL3000 lrQ; // lock free queue
	std::promise<int> read_error_promise;
	std::future<int> read_error_future = read_error_promise.get_future();
	size_t total = 1000000;
	thread consumer(lock_free_Q_consumer, std::ref(lrQ), total, move(read_error_promise));
	for (size_t i = 0; i < total; i++)
		lrQ.push_wait(move(i));
	read_error_future.wait();
	CHECK(read_error_future.get() == 0);
	consumer.join();
}