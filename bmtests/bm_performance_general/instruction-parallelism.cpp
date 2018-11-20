
#include <benchmark/benchmark.h>

using ULL = unsigned long long;
ULL atoui_1(const char *a)
{
	ULL ans = 0;
	while (*a++) {
		ans = (ans * 10) + *a - '0';
	}
	return ans;
}

ULL atoui_2(const char *b, const char *e)
{
	const static ULL pow10[20] = {	10000000000000000000ul, 
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
									100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
	static const int size = sizeof(pow10) / sizeof(pow10[0]);
	if (b >= e)
		return 0;
	int i = size - (e - b);
	if (i < 0) {
		i = 0;  // string too long, use use 20 digits
		e = b + size;
	}
	ULL ans = 0;
	while (b < e) {
		ans += pow10[i++] * (*b++ - '0');
	}
	return ans;
}

const char *number = "12345678901234567890";
const char *number_end = number + sizeof(number);
static void BM_atoui_1(benchmark::State& state) {  // shared_ptr, 40ns overhead
	ULL result = 0;
	for (auto _ : state) {
		benchmark::DoNotOptimize(result = atoui_1(number));
	}
}
BENCHMARK(BM_atoui_1);

static void BM_atoui_2(benchmark::State& state) {  // shared_ptr, 40ns overhead
	ULL result = 0;
	for (auto _ : state) {
		benchmark::DoNotOptimize(result = atoui_2(number, number_end));
	}
}
BENCHMARK(BM_atoui_2);