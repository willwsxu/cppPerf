#pragma once
#include <memory>
#include <cassert>

inline std::unique_ptr<char[]> memset_char(char x, int64_t c) {
	size_t count = static_cast<size_t>(c);
	auto buf = std::make_unique<char[]>(count);
	memset(buf.get(), x, count - 1);
	buf[count - 1] = 0;
	return buf;
}

class StringBuilder
{
	std::unique_ptr<char[]>  buf;
	size_t capacity;
	size_t size=0;
public:
	StringBuilder(int sz) :buf(std::make_unique<char[]>(sz)), capacity(sz)
	{
	}
	StringBuilder& append(const char *app) {
		size_t more = strlen(app);
		assert(size + more < capacity);
		memcpy(buf.get() + size, app, more);
		size += more;
		buf.get()[size] = 0;
		return *this;
	}
	StringBuilder& append(int count, const char c) {
		assert(size + 1 < capacity);
		memcpy(buf.get() + size, &c, 1);
		size += 1;
		buf.get()[size] = 0;
		return *this;
	}
	void clear()
	{
		size = 0;
		buf.get()[0] = 0;
	}
};