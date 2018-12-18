#pragma once

// convert int to string, right justified, pad left with 0
int itoa_fix(int64_t val, char *szValue, char *pEnd)
{
	if (val < 0) {
		*szValue++ = '-';
		val = -val;
	}
	return itoa_pos_pad_left(val, szValue, pEnd);
}

int itoa_fix_experiment(int64_t val, char *szValue, char *pEnd)
{
	if (val < 0) {
		*szValue++ = '-';
		val = -val;
	}
	char *fill = pEnd - count_digits(val);
	while (pEnd-- > fill) {
		*pEnd = val % 10 + '0';
		val /= 10;
	}
	while (fill-- > szValue)
		*fill = '0';
	return (int)val;
}

static void BM_itoa_fix(benchmark::State& state)  // 7.3 ns vs 63.8 ns experiment code
{
	static const int64_t tests[] = { -1234567890, -12345678900ll, -123456789000ll , -1234567890000ll };
	char szValue[20] = { 0 };
	int val = 0;
	int64_t test = tests[state.range(1) - 2];
	char *szEnd = szValue + state.range(0);
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(val = itoa_fix(-1234567890, szValue, szEnd));
	}
}
BENCHMARK(BM_itoa_fix_experiment)->Arg(12)->Arg(14)->Arg(16)->Arg(18);
//BENCHMARK(BM_itoa_fix)->Args({ 12,2 })->Args({ 14,3 })->Args({ 16,4 })->Args({ 18,5 });

static void BM_itoa_fix_experiment(benchmark::State& state)
{
	char szValue[20] = { 0 };
	int val = 0;
	char *szEnd = szValue + state.range(0);
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(val = itoa_fix_experiment(-1234567890, szValue, szEnd));
	}
}
BENCHMARK(BM_itoa_fix_experiment)->Arg(12)->Arg(14)->Arg(16)->Arg(18);


bool ftoa_fix(int64_t val, unsigned short dp, char *szValue, char *pEnd)
{
	if (dp >= 10)
		return false;
	const static int pow10_[10] = { 1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000 };
	if (val < 0) {
		*szValue++ = '-';
		val = -val;
	}
	itoa_fill(val%pow10_[dp], pEnd - 1, dp); // data dependency
	*(pEnd - dp - 1) = '.';
	itoa_pos_pad_left(static_cast<int>(val / pow10_[dp]), szValue, pEnd - dp - 1);
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
