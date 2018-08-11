#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
using namespace std;

class MyCircularDeque {  // beat 96%
	vector<int>	dq;
	int front, size, full;
	inline int rearIndex() {
		return (front + size - 1) % full;
	}
public:
	/** Initialize your data structure here. Set the size of the deque to be k. */
	MyCircularDeque(int k) :full(k) {
		dq.resize(k);
		front = 0;
		size = 0;
	}

	/** Adds an item at the front of Deque. Return true if the operation is successful. */
	bool insertFront(int value) {
		if (isFull())
			return false;
		if (isEmpty())
			return insertLast(value);
		else {
			front = (front - 1 + full) % full;
			dq[front] = value;
			size++;
			return true;
		}
	}

	/** Adds an item at the rear of Deque. Return true if the operation is successful. */
	bool insertLast(int value) {
		if (isFull())
			return false;
		dq[(front + size++) % full] = value;
		return true;
	}

	/** Deletes an item from the front of Deque. Return true if the operation is successful. */
	bool deleteFront() {
		if (isEmpty())
			return false;
		front = (front + 1) % full;
		size--;
		return true;
	}

	/** Deletes an item from the rear of Deque. Return true if the operation is successful. */
	bool deleteLast() {
		if (isEmpty())
			return false;
		size--;
		return true;
	}

	/** Get the front item from the deque. */
	int getFront() {
		if (isEmpty())
			return -1;
		return dq[front];
	}

	/** Get the last item from the deque. */
	int getRear() {
		if (isEmpty())
			return -1;
		return dq[rearIndex()];
	}

	/** Checks whether the circular deque is empty or not. */
	bool isEmpty() {
		return size == 0;
	}

	/** Checks whether the circular deque is full or not. */
	bool isFull() {
		return size == full;
	}
};

TEST_CASE("641. Design Circular Deque", "[NEW]")
{
	MyCircularDeque d(3);
	CHECK(d.insertLast(1) == true);
	CHECK(d.insertLast(2) == true);
	CHECK(d.insertFront(3) == true);
	CHECK(d.insertFront(4) == false);
	CHECK(d.getRear() == 2);
	CHECK(d.isFull());
	CHECK(d.deleteLast());
	CHECK(d.insertFront(4));
	CHECK(d.getFront() == 4);
}
TEST_CASE("641. Design Circular Deque test2", "[NEW]")
{
	MyCircularDeque d(3);
	CHECK(d.insertFront(10) == true);
	CHECK(d.insertLast(8) == true);
	CHECK(d.insertLast(2) == true);
	CHECK(d.getFront() == 10);
	CHECK(d.deleteLast());
	CHECK(d.getRear() == 8);
	CHECK(d.insertFront(11) == true);
	CHECK(d.deleteFront());
	CHECK(d.getRear() == 8);
	CHECK(d.getFront() == 10);
	CHECK(d.insertLast(2) == true);
	CHECK(d.isFull());
}
TEST_CASE("641. Design Circular Deque test3", "[NEW]")
{
	MyCircularDeque d(4);
	CHECK(d.insertFront(9) == true);
	CHECK(d.deleteLast());
	CHECK(d.getRear() == -1);
	CHECK(d.getFront() == -1);
	CHECK(d.deleteFront() == false);
	CHECK(d.insertFront(6) == true);
	CHECK(d.insertLast(5) == true);
	CHECK(d.insertFront(9) == true);
	CHECK(d.getFront() == 9);
	CHECK(d.insertFront(6) == true);
}

// 622. Design Circular Queue
class MyCircularQueue {  // same impl as MyCircularDeque, beat 97%
	vector<int>	dq;
	int front, size, full;
	inline int rearIndex() {
		return (front + size - 1) % full;
	}
public:
	/** Initialize your data structure here. Set the size of the queue to be k. */
	MyCircularQueue(int k) :full(k) {
		dq.resize(k);
		front = 0;
		size = 0;
	}

	/** Insert an element into the circular queue. Return true if the operation is successful. */
	bool enQueue(int value) {  // insert from rear
		if (isFull())
			return false;
		dq[(front + size++) % full] = value;
		return true;
	}

	/** Delete an element from the circular queue. Return true if the operation is successful. */
	bool deQueue() {  // delete from front
		if (isEmpty())
			return false;
		front = (front + 1) % full;
		size--;
		return true;
	}

	/** Get the front item from the queue. */
	int Front() {
		if (isEmpty())
			return -1;
		return dq[front];
	}

	/** Get the last item from the queue. */
	int Rear() {
		if (isEmpty())
			return -1;
		return dq[rearIndex()];
	}

	/** Checks whether the circular queue is empty or not. */
	bool isEmpty() {
		return size == 0;
	}

	/** Checks whether the circular queue is full or not. */
	bool isFull() {
		return size == full;
	}
};
