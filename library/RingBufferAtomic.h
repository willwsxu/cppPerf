#pragma once
#include <atomic>
#include <array>
#include <mutex>
#include <condition_variable>

using std::pair;
using std::array;
using std::atomic;

// single reader, single writer
template<typename T, size_t BUF_SIZE>
class circular_queue
{
public:
	// push range
	// bool push(const T& val)
	bool push(T&& val)  // push back
	{
		size_t write_pos = write.load();
		size_t next_write = (write_pos+1) % RING_SIZE;
		if (next_write == read.load())
			return false;  // full
		_queue[write_pos] = move(val);
		write.store(next_write);
		return true;
	}
	bool full(size_t next) {
		return next == read.load();
	}
	void push_wait(T&& val)  // push back
	{
		size_t write_pos = write.load();
		size_t next_write = (write_pos + 1) % RING_SIZE;
		if (full(next_write)) {
			std::unique_lock<std::mutex> lock(event_lock);  // queue is full, wait
			event_signal.wait(lock, [next_write,this]() { return !this->full(next_write); });
		}
		_queue[write_pos] = move(val);
		write.store(next_write);
		event_signal.notify_one();  // in case pop is waiting
	}
	pair<T,bool> pop() { // pop front
		size_t read_pos = read.load();
		if (write.load()==read_pos)
			return{ T{}, false };
		read.store((read_pos+1)%RING_SIZE);
		return{ std::move(_queue[read_pos]), true };
	}
	bool empty(size_t read_pos)
	{
		return write.load() == read_pos;
	}
	T pop_wait() { // pop front, wait if empty
		size_t read_pos = read.load();
		if (empty( read_pos) ) {
			std::unique_lock<std::mutex> lock(event_lock);  // queue is empty, wait
			event_signal.wait(lock, [read_pos,this]() { return !this->empty( read_pos ); });
		}
		read.store((read_pos + 1) % RING_SIZE);
		event_signal.notify_one();  // in case push is waiting
		return std::move(_queue[read_pos]);
	}
	// pop all
	// pop range
	bool empty() const
	{
		return read.load() == write.load();
	}
	size_t size() const {
		auto r = read.load();
		auto w = write.load();
		return r <= w?w - r:w+RING_SIZE-r;
	}
	void clear()
	{

	}
private:
	static constexpr size_t RING_SIZE = BUF_SIZE + 1;
	atomic<size_t> read=0, write=0;
	array<T, RING_SIZE>	_queue{};  // extra slot so writer never catch up reader

	std::mutex event_lock;
	std::condition_variable event_signal;
};

