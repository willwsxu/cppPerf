#include <benchmark/benchmark.h>

int count_digits_basic(int val)
{
	int count = 1;
	while (val >= 10) {
		count++;
		val /= 10;
	}
	return count;
}
int count_digits(int64_t val) {
	if (val >= 10000) {
		if (val >= 1000000) {
			if (val >= 100000000) {
				if (val >= 1000000000)
					return 10;
				return 9;
			}
			else {
				if (val >= 10000000)
					return 8;
				return 7;
			}
		}
		else {
			if (val >= 100000)
				return 6;
			return 5;
		}
	}
	else {
		if (val >= 100) {
			if (val >= 1000)
				return 4;
			return 3;
		}
		else {
			if (val >= 10)
				return 2;
			return 1;
		}
	}
}

int count_digits2(int val) {
	int count = 0;
	while (val >= 10000) {
		count += 4;
		val /= 10000;
	}
	if (val >= 100) {
		if (val >= 1000)
			return 4;
		return 3;
	}
	else {
		if (val >= 10)
			return 2;
		return 1;
	}
}

static void BM_count_digits_basic(benchmark::State& state)
{
	int count = 0;;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(count = count_digits_basic(1234567890));
	}
}
BENCHMARK(BM_count_digits_basic);

static void BM_count_digits(benchmark::State& state)
{
	int count = 0;;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(count = count_digits(1234567890));
	}
}
BENCHMARK(BM_count_digits);

static void BM_count_digits2(benchmark::State& state)
{
	int count = 0;;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(count = count_digits2(1234567890));
	}
}
BENCHMARK(BM_count_digits2);


char* itoa_new2(int64_t val, char *szValue, int size)
{
	//assert size>12
	if (val == 0) {
		szValue[0] = '0';
		szValue[1] = 0;
	}
	else {
		char *pEnd = nullptr;
		if (val < 0) {
			szValue[0] = '-';
			val = -val;
			pEnd = szValue + count_digits(val);
		}
		else
			pEnd = szValue + count_digits(val) - 1;
		pEnd[1] = 0;
		while (val > 0) {
			*pEnd-- = val % 10 + '0';
			val /= 10;
		}
	}
	return szValue;
}

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

int itoa_fix_ori(int64_t val, char *szValue, char *pEnd)
{
	if (val < 0) {
		*szValue++ = '-';
		val = -val;
	}
	pEnd--;
	char *fill = pEnd - count_digits(val);
	while (val>0) {
		*pEnd-- = val % 10 + '0';
		val /= 10;
	}
	while (fill >= szValue)
		*fill-- = '0';
	return (int)val;
}

int itoa_fix(int64_t val, char *szValue, char *pEnd)
{
	if (val < 0) {
		*szValue++ = '-';
		val = -val;
	}
	//pEnd--;
	char *fill = pEnd - count_digits(val);
	while (val>0) {
		*--pEnd = val % 10 + '0';
		val /= 10;
	}
	while (fill > szValue)
		*--fill = '0';
	return (int)val;
}

static void BM_itoa_fix(benchmark::State& state)
{
	char szValue[12] = { 0 };
	int val = 0;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(val = itoa_fix(-1234567890, szValue, szValue + sizeof(szValue)));
	}
}
BENCHMARK(BM_itoa_fix);


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
	return val == 0 ? 0 : count_digits(val);
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
