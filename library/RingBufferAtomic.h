#pragma once
#include <atomic>
#include <array>

using std::pair;
using std::array;
using std::atomic;

// single reader, single writer
template<typename T, size_t BUF_SIZE>
class RingBuffer
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
	pair<T,bool> pop() { // pop front
		size_t read_pos = read.load();
		if (write.load()==read_pos)
			return{ T{}, false };
		read.store((read_pos+1)%RING_SIZE);
		return{ std::move(_queue[read_pos]), true };
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
};

