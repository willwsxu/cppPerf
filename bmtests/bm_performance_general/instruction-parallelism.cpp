
#include <benchmark/benchmark.h>

using ULL = unsigned long long;
ULL atoui_1(const char *a)
{
	ULL ans = 0;
	while (*a) {
		ans = (ans * 10) + *a++ - '0';
	}
	return ans;
}

const static ULL pow10_[20] = { 10000000000000000000ul,
1000000000000000000ul,
100000000000000000ul,
10000000000000000ul,
1000000000000000ul,
100000000000000ul,
10000000000000ul,
1000000000000ul,
100000000000ul,
10000000000ul,
1000000000,
100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1 };
static const int size = sizeof(pow10_) / sizeof(pow10_[0]);
ULL atoui_2(const char *b, const char *e)
{
	if (b >= e)
		return 0;
	int i = size - (e - b);
	if (i < 0) {
		i = 0;  // string too long, use use 20 digits
		e = b + size;
	}
	ULL ans = 0;
	while (b < e) {
		//if ( (*b<'0' | *b>'9') )  // better than ||, due to parallelism
		//	return ans;
		auto d = (unsigned)(*b++) - '0';  // better than code above
		if (d >= 10)
			return ans;
		ans += pow10_[i++] * d;
	}
	return ans;
}

ULL atoui_3(const char *b, const char *e)
{
	if (b >= e)
		return 0;
	int i = size - (e - b);
	if (i < 0) {
		i = 0;  // string too long, use use 20 digits
		e = b + size;
	}
	ULL ans = 0;
	for (; e - b >= 4; b += 4, i+=4) {
		auto d1 = (unsigned)b[0] - '0';
		if (d1 >= 10)
			return ans;
		auto  s1 = pow10_[i+0] * d1;
		auto d2 = (unsigned)b[1] - '0';
		if (d2 >= 10)
			return ans;
		auto  s2 = pow10_[i+1] * d2;
		auto d3 = (unsigned)b[2] - '0';
		if (d3 >= 10)
			return ans;
		auto  s3 = pow10_[i+2] * d3;
		auto d4 = (unsigned)b[3] - '0';
		if (d4 >= 10)
			return ans;
		auto  s4 = pow10_[i+3] * d4;
		ans += (s1 + s2 + s3 + s4);
	}
	while (b < e) {
		auto d = (unsigned)(*b++) - '0';  // better than code above
		if (d >= 10)
			return ans;
		ans += pow10_[i++] * d;
	}
	return ans;
}

const char *number = "12345678901234567890";
const char *number_end = number + sizeof("12345678901234567890");
static void BM_atoui_1(benchmark::State& state) {  // shared_ptr, 40ns overhead
	ULL result = 0;
	for (auto _ : state) {
		result = atoui_1(number);
		//benchmark::DoNotOptimize(result);
	}
	state.SetItemsProcessed(result);
}
BENCHMARK(BM_atoui_1);

static void BM_atoui_2(benchmark::State& state) {  // shared_ptr, 40ns overhead
	ULL result = 0;
	for (auto _ : state) {
		result = atoui_2(number, number_end);
		//benchmark::DoNotOptimize(result);
	}
	state.SetItemsProcessed(result);
}
BENCHMARK(BM_atoui_2);

static void BM_atoui_3(benchmark::State& state) {  // loop unrolling, 19ns
	ULL result = 0;
	for (auto _ : state) {
		result = atoui_3(number, number_end);
		//benchmark::DoNotOptimize(result);
	}
	state.SetItemsProcessed(result);
}
BENCHMARK(BM_atoui_3);