#pragma once

int count_digits_basic(int val)
{
	int count = 1;
	while (val >= 10) {
		count++;
		val /= 10;
	}
	return count;
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

int count_digits3(unsigned int val) { // idea from Andrei Alexandrescu
	int count = 0;
	for (;;)
	{
		if (val < 10)
			return count + 1;
		else if (val<100)
			return count + 2;
		else if (val<1000)
			return count + 3;
		else if (val<10000)
			return count + 4;
		val /= 10000;
		count + 4;
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

static void BM_count_digits3(benchmark::State& state)
{
	int count = 0;;
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(count = count_digits3(1234567890));
	}
}
BENCHMARK(BM_count_digits3);
