#pragma once
#include <memory>

inline std::unique_ptr<char[]> memset_char(char x, int64_t c) {
	size_t count = static_cast<size_t>(c);
	auto buf = std::make_unique<char[]>(count);
	memset(buf.get(), x, count - 1);
	buf[count - 1] = 0;
	return buf;
}