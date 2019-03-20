#pragma once

// use state.range cost extra 55 ns,  BM_state_used vs BM_state_notused
// fast for loop is not really fast, BM_state_used_fast is not better than BM_state_used
// baseline for loop use 1.5 ns
#define VAL_SIZE 16
static void BM_state_used(benchmark::State& state)
{
	char szValue[VAL_SIZE] = { 0 };
	int val = 0;
	char *szEnd = szValue + static_cast<int>(state.range(0));
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(val = itoa_fix(-1234567890, szValue, szEnd));
	}
}
BENCHMARK(BM_state_used)->Arg(12)->Arg(14)->Arg(16);


static void BM_state_used_fast(benchmark::State& state)
{
	char szValue[VAL_SIZE] = { 0 };
	int val = 0;
	char *szEnd = szValue + static_cast<int>(state.range(0));
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(val = itoa_fix(-1234567890, szValue, szEnd));
	}
}
BENCHMARK(BM_state_used_fast)->Arg(12)->Arg(14)->Arg(16);

static void BM_state_notused(benchmark::State& state)
{
	char szValue[VAL_SIZE] = { 0 };
	int val = 0;
	char *szEnd = szValue + static_cast<int>(state.range(0));
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(val = itoa_fix(-1234567890, szValue, szValue + sizeof(szValue)));
	}
}
BENCHMARK(BM_state_notused)->Arg(12)->Arg(14)->Arg(16);

static void BM_baseline(benchmark::State& state)
{
	char szValue[VAL_SIZE] = { 0 };
	int val = 0;
	char *szEnd = szValue + static_cast<int>(state.range(0));
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(val = static_cast<int>(state.range(0)));
	}
}
BENCHMARK(BM_baseline)->Arg(12)->Arg(14)->Arg(16);
