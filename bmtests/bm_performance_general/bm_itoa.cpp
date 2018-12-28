#include <benchmark/benchmark.h>
#include "digits-helper.h"

#include "bm_price.h"
//#include "bm_ftoa.h"
//#include "bm_itoa_fix.h"  // and ftoa_fix
//#include "bm_test_setup.h"
//#include "bm_tmg_itoa.h"
//#include "bm_count_digits.h"

// performance on quickbench.com for itoa_, itoa_new2, itoa_new, sprintf
// 5.9, 5.95, 31, 239  (same for 2 runs)
// benchmark on windows laptop is different
char* itoa_new(int val, char *szValue, int size)  // slow using reverse
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
/*
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

static void BM_itoa_best(benchmark::State& state)
{
	char szValue[12] = { 0 };
	char *pVal = szValue;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(pVal = itoa_(-1234567890, szValue, sizeof(szValue)));
	}
}
BENCHMARK(BM_itoa_best);
*/