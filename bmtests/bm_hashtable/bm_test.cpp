
#include "bm_hashtable_large.cpp"


static void BM_wait(benchmark::State& state) {
	using namespace std::chrono_literals;
	for (auto _ : state)
		std::this_thread::sleep_for(300ms);
}
//BENCHMARK(BM_wait);
BENCHMARK_MAIN();