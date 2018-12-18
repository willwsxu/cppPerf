#pragma once

// itoa_fix and ftoa_fix are much faster than itoa_experiment because of data parallelism and test 0
// a little slower than itoa_, 7.3 vs 6 ns 
int itoa_fix_experiment(int64_t val, char *szValue, char *pEnd)
{
	if (val < 0) {
		*szValue++ = '-';
		val = -val;
	}
	char *fill = pEnd - count_digits(val);
	while (pEnd-- > fill) {  // slower than test 0
		*pEnd = val % 10 + '0';
		val /= 10;
	}
	while ( szValue < fill)
		*szValue++ = '0';
	return (int)val;
}

#define VAL_SIZE 16
static void BM_itoa_fix(benchmark::State& state)  // 7.3 ns vs 63.8 ns experiment code
{
	//static const int64_t tests[] = { -1234567890, -12345678900ll, -123456789000ll , -1234567890000ll };
	char szValue[VAL_SIZE] = { 0 };
	int val = 0;
	//int64_t test = tests[state.range(1) - 2];
	char *szEnd = szValue + sizeof(szValue);// state.range(0);
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(val = itoa_fix(-1234567890, szValue, szEnd));
	}
}
BENCHMARK(BM_itoa_fix);// ->Arg(12)->Arg(14)->Arg(16);
//BENCHMARK(BM_itoa_fix)->Args({ 12,2 })->Args({ 14,3 })->Args({ 16,4 })->Args({ 18,5 });

static void BM_itoa_fix_experiment(benchmark::State& state)
{
	char szValue[VAL_SIZE] = { 0 };
	int val = 0;
	char *szEnd = szValue + sizeof(szValue);// state.range(0);
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(val = itoa_fix_experiment(-1234567890, szValue, szEnd));
	}
}
BENCHMARK(BM_itoa_fix_experiment);// ->Arg(12)->Arg(14)->Arg(16);

static void BM_ftoa_fix(benchmark::State& state)
{
	// long long value slow down a lot
	static const int64_t tests[] = { -1234567890, -12345678900ll, -123456789000ll , -1234567890000ll };
	char szValue[VAL_SIZE] = { 0 };
	bool success = false;
	//int64_t test = tests[state.range(1) - 2];
	//int dp = static_cast<unsigned short>(state.range(1));  // use dp also slow down, ?
	char *szEnd = szValue + sizeof(szValue);// state.range(0);
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(success = ftoa_fix(-1234567890, 2, szValue, szEnd));
	}
}
//BENCHMARK(BM_ftoa_fix)->Args({ 12,2 })->Args({ 14,3 })->Args({ 16,4 })->Args({ 18,5 });
BENCHMARK(BM_ftoa_fix);// ->Arg(12)->Arg(14)->Arg(16)->Arg(18);
