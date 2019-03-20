#pragma once

// ftoa_ reuse ftoa_fix, with some redundant code but it is still much faster on quickbench.com
// 5 vs 54 vs 72 ns
// ftoa_ can be improved to reduce code redundancy
char* ftoa(int val, int dp, char *szValue, int size)
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
		while (dp > 0 && val % 10 == 0) {  // trim trailing 0
			val /= 10;
			dp--;
		}
		if (dp > 0) {
			while (dp>0 && val > 0 && idx < size) {
				szValue[idx++] = val % 10 + '0';
				val /= 10;
				dp--;
			}
			while (dp-->0)
				szValue[idx++] = '0';
			szValue[idx++] = '.';
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

char* ftoa2(int val, int dp, char *szValue, int size)
{
	if (val == 0) {
		szValue[0] = '0';
		szValue[1] = 0;
	}
	else {
		char *pEnd = szValue;

		if (val < 0) {
			szValue[0] = '-';
			val = -val;
			pEnd++;
		}
		while (dp > 0 && val % 10 == 0) {  // trim trailing 0
			val /= 10;
			dp--;
		}
		pEnd += count_digits(val) - 1;

		if (dp > 0) {
			pEnd++;
			pEnd[1] = 0;
			while (dp>0 && val > 0) {
				*pEnd-- = val % 10 + '0';
				val /= 10;
				dp--;
			}
			while (dp-->0)
				*pEnd-- = '0';
			*pEnd-- = '.';
		}
		else
			pEnd[1] = 0;
		while (val > 0) {
			*pEnd-- = val % 10 + '0';
			val /= 10;
		}
	}
	return szValue;
}

char* ftoa_(int val, int dp, char *szValue, int size)
{
	if (val < 0) {
		val = -val;
		*szValue++ = '-';
		size--;
	}
	int len = std::min(count_digits(val), size - 1);  // save last byte for nil
	ftoa_fix(val, dp, szValue, szValue+len);
	szValue[len] = 0;
	return szValue;
}

static void BM_ftoa_fast(benchmark::State& state)
{
	char szValue[14] = { 0 };
	char *pVal = szValue;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(pVal = ftoa_(-1234567890, 2, szValue, sizeof(szValue)));
	}
}
BENCHMARK(BM_ftoa_fast);

static void BM_ftoa2(benchmark::State& state)
{
	char szValue[14] = { 0 };
	char *pVal = szValue;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(pVal = ftoa2(-1234567890, 2, szValue, sizeof(szValue)));
	}
}
BENCHMARK(BM_ftoa2);

static void BM_ftoa(benchmark::State& state)
{
	char szValue[12] = { 0 };
	char *pVal = szValue;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(pVal = ftoa(-1234567890, 2, szValue, sizeof(szValue)));
	}
}
BENCHMARK(BM_ftoa);
