#pragma once
#include "Digits-Helper.h"

inline int16_t tmg_itoa_delta(char* dst, int64_t val, char* end, int64_t& check_sum) {
	if ((int64_t)val < 0) {
		val = -val;
		check_sum += static_cast<int64_t>('-') - static_cast<int64_t>(*dst);
		*dst++ = '-';
	}
	while (--end >= dst) {
		uint64_t rem = val % 10;
		char next = static_cast<char>(rem + '0');
		check_sum += static_cast<int64_t>(next) - static_cast<int64_t>(*end);
		*end = next;
		val /= 10;
	}
	return 0;
}

// quick-bench.com show 7:94 ns improvement, due to low data dependency
inline int16_t tmg_itoa_delta_xu(char* dst, int64_t val, char* end, int64_t& check_sum) {
	if ((int64_t)val < 0) {
		val = -val;
		check_sum += static_cast<int64_t>('-') - static_cast<int64_t>(*dst);
		*dst++ = '-';
	}
	int len = end - dst;
	int digits = count_digits(val);
	int fill_zero = len > digits ? len - digits : 0;
	end--;
	while (len--) {  // compare len to 0 is faster than compare pointer ==
		char next = '0';
		if (val > 0) {
			next = static_cast<char>(val % 10 + '0');
			val /= 10;
		}
		check_sum += static_cast<int64_t>(next) - static_cast<int64_t>(*end);
		*end-- = next;  // post fix decrement
	}
	return 0;
}

static void BM_tmg_itoa(benchmark::State& state)
{
	char szValue[12] = { 0 };
	int len = 0;
	int64_t check_sum = 0;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(len = tmg_itoa_delta(szValue, -1234567890, szValue + sizeof(szValue), check_sum));
	}
}
BENCHMARK(BM_tmg_itoa);

static void BM_tmg_itoa_xu(benchmark::State& state)
{
	char szValue[12] = { 0 };
	int len = 0;
	int64_t check_sum = 0;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(len = tmg_itoa_delta_xu(szValue, -1234567890, szValue + sizeof(szValue), check_sum));
	}
}
BENCHMARK(BM_tmg_itoa_xu);
