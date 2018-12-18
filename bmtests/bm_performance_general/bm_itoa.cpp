#include <benchmark/benchmark.h>
#include "digits-helper.h"


inline void itoa_fill(int val, char *pBack, int len)
{
	while (len--) {
		*pBack-- = val % 10 + '0';
		val /= 10;
	}
}
bool ftoa_fix(int64_t val, unsigned short dp, char *szValue, char *pEnd)
{
	if (dp >= 10)
		return false;
	const static int pow10_[10]={1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
	if (val < 0) {
		*szValue++ = '-';
		val = -val;
	}
	itoa_fill(val%pow10_[dp], pEnd - 1, dp); // data dependency
	*(pEnd-dp-1) = '.';
	itoa_pos_pad_left(static_cast<int>(val/pow10_[dp]), szValue, pEnd-dp-1);
	return true;
}
static void BM_ftoa_fix(benchmark::State& state)
{
	// long long value slow down a lot
	static const int64_t tests[] = { -1234567890, -12345678900ll, -123456789000ll , -1234567890000ll };
	char szValue[20] = { 0 };
	bool success = false;
	int64_t test = tests[state.range(1) - 2];
	int dp = static_cast<unsigned short>(state.range(1));  // use dp also slow down, ?
	char *szEnd = szValue + state.range(0);
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(success = ftoa_fix(-1234567890, 2, szValue, szEnd));
	}
}
BENCHMARK(BM_ftoa_fix)->Args({ 12,2 })->Args({ 14,3 })->Args({ 16,4 })->Args({ 18,5 });

char* itoa_new(int val, char *szValue, int size)
{
	size--; // reserve last char to nil
	int idx = 0;
	if (val == 0) {
		szValue[idx++] = '0';
	}
	else {
		if (val < 0) {
			szValue[idx++] = '-';
			val = -val;
		}
		while (val > 0 && idx < size) {
			szValue[idx++] = val % 10 + '0';
			val /= 10;
		}
		auto start = szValue[0] == '-' ? 1 : 0;
		std::reverse(szValue + start, szValue + idx);
	}
	szValue[idx++] = 0;
	return szValue;
}

static void BM_itoa(benchmark::State& state)
{
	char szValue[12] = { 0 };
	char *pVal = szValue;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(pVal = _itoa(-1234567890, szValue, 10));
	}
}
BENCHMARK(BM_itoa);

static void BM_snprintf(benchmark::State& state)
{
	char szValue[12] = { 0 };
	int count = 0;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(count = snprintf(szValue, sizeof(szValue), "%d", -1234567890));
	}
}
BENCHMARK(BM_snprintf);

static void BM_itoa_new(benchmark::State& state)
{
	char szValue[12] = { 0 };
	char *pVal = szValue;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(pVal = itoa_new(-1234567890, szValue, sizeof(szValue)));
	}
}
BENCHMARK(BM_itoa_new);

static void BM_itoa_new2(benchmark::State& state)
{
	char szValue[12] = { 0 };
	char *pVal = szValue;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(pVal = itoa_new2(-1234567890, szValue, sizeof(szValue)));
	}
}
BENCHMARK(BM_itoa_new2);