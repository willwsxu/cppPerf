#pragma once

const int MAX_SIZE = 8<<9;
static void BM_post_increment(benchmark::State& state) {
	int a[MAX_SIZE];
	int loop = static_cast<int>(state.range(0));
	for (auto _ : state)
	{
		int i = 0;
		while (i < loop )
			a[i++] = 0;
	}
	//std::cout << a[0] << std::endl;
}

static void BM_post_increment_int64(benchmark::State& state) {
	int a[MAX_SIZE];
	int64_t loop = state.range(0);
	for (auto _ : state)
	{
		int i = 0;
		while (i < loop)
			a[i++] = 0;
	}
}
BENCHMARK(BM_post_increment_int64)->Range(1, MAX_SIZE);  // loop counter int64 is slower

static void BM_post_increment_range(benchmark::State& state) {
	int a[MAX_SIZE];
	for (auto _ : state)
	{
		int i = 0;
		while (i < state.range(0))
			a[i++] = 0;
	}
}
BENCHMARK(BM_post_increment_range)->Range(1, MAX_SIZE);

static void BM_pre_increment(benchmark::State& state) {
	int a[MAX_SIZE];
	int loop = static_cast<int>(state.range(0))-1;
	for (auto _ : state)
	{
		int i = -1;
		while (i < loop)
			a[++i] = 0;
	}
}

static void BM_pre_increment2(benchmark::State& state) {
	int a[MAX_SIZE];
	int b[MAX_SIZE];
	int loop = static_cast<int>(state.range(0));
	for (auto _ : state)
	{
		int i = -1;
		int j = -1;
		while (i < loop) {
			a[++i] = 0;
			b[++j] = 0;
		}
	}
}

static void BM_post_increment2(benchmark::State& state) {
	int a[MAX_SIZE];
	int b[MAX_SIZE];
	int loop = static_cast<int>(state.range(0))-1;
	for (auto _ : state)
	{
		int i = 0;
		int j = 0;
		while (i < loop) {
			a[i++] = 0;
			b[j++] = 0;
		}
	}
}

BENCHMARK(BM_post_increment)->Range(1, MAX_SIZE);  //1
BENCHMARK(BM_pre_increment)->Range(1, MAX_SIZE);   //6
BENCHMARK(BM_post_increment2)->Range(1, MAX_SIZE); //1
BENCHMARK(BM_pre_increment2)->Range(1, MAX_SIZE);  //7

/* 
Post increment is efficient due to prefetch?
int64 type is slower, avoid it in loop counter
access array of int64 is more slower
 */