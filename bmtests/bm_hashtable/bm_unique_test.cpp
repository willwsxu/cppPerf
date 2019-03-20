#include <benchmark/benchmark.h>
#include <unordered_set>
#include <array>

vector<int> candies(10000);  // find unique number in vector, value range [-100000,100000]
static void BM_vector_random_gen(benchmark::State& state) {
	for (auto _ : state) {
		std::random_device rd;
		std::mt19937 g(rd());
		uniform_int_distribution<> dist(-100000, 100000);
		for (int &n : candies)
			n = dist(g);
	}
	state.SetItemsProcessed(candies.size());
}
BENCHMARK(BM_vector_random_gen);

static const int VALUE_RANGE = 200001;
static const int OFFSET = 100000;

template<typename Cont, typename T>
size_t count_unique(Cont& exist, T val)
{
	size_t count = 0;
	for (int c : candies) {
		if (!exist[c + OFFSET]) {
			exist[c + OFFSET] = val;
			count++;
		}
	}
	return count;
}
static void BM_vector_int_temp(benchmark::State& state) { // vector<int> is 10x slower than vector<bool>
	int ans = 0;
	for (auto _ : state) {
		vector<int> exist(VALUE_RANGE, 0);
		size_t count = count_unique(exist, (int)1);
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_vector_int_temp);

static void BM_vector_char_temp(benchmark::State& state) { // vector<int> is 10x slower than vector<bool>
	int ans = 0;
	for (auto _ : state) {
		vector<char> exist(VALUE_RANGE, 0);
		size_t count = count_unique(exist, (char)1);
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_vector_char_temp);

static void BM_vector_char(benchmark::State& state) { // vector<char> is 30% better than vector<bool>
	int ans = 0;
	for (auto _ : state) {
		vector<char> exist(VALUE_RANGE, 0);
		size_t count = 0;
		for (int c : candies) {
			if (!exist[c + OFFSET]) {
				exist[c + OFFSET] = 1;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_vector_char);

static void BM_vector_bool_temp(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		vector<bool> exist(VALUE_RANGE, 0);
		size_t count = count_unique(exist, true);
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_vector_bool_temp);

static void BM_vector_bool(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		vector<bool> exist(VALUE_RANGE, 0);
		size_t count = 0;
		for (int c : candies) {
			if (!exist[c + OFFSET]) {
				exist[c + OFFSET] = 1;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_vector_bool);

static void BM_vector_bool_at(benchmark::State& state) { // using at is 10% faster than [], 
	int ans = 0;
	for (auto _ : state) {
		vector<bool> exist(VALUE_RANGE, false);
		size_t count = 0;
		for (int c : candies) {
			if (!exist.at(c + OFFSET)) {
				exist.at(c + OFFSET) = true;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_vector_bool_at);

static void BM_array_cls_int_temp(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		std::array<int, VALUE_RANGE> exist{};  // array<bool> and arrray<char> is 2x faster than array<int>
		size_t count = count_unique(exist, (int)1);
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_array_cls_int_temp);

static void BM_array_cls_char_temp(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		std::array<char, VALUE_RANGE> exist{};
		size_t count = count_unique(exist, (char)1);
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_array_cls_char_temp);

static void BM_array_cls_bool_temp(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		std::array<bool, VALUE_RANGE> exist{};
		size_t count = count_unique(exist, true);
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_array_cls_bool_temp);

static void BM_array_cls_bool(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		std::array<bool, VALUE_RANGE> exist{};  // array<bool> is 2x faster than array<int>
		size_t count = 0;
		for (int c : candies) {
			if (!exist[c + OFFSET]) {
				exist[c + OFFSET] = true;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_array_cls_bool);

static void BM_array_cls_bool_at(benchmark::State& state) { // using at is slightly slower
	int ans = 0;
	for (auto _ : state) {
		std::array<bool, VALUE_RANGE> exist{};
		size_t count = 0;
		for (int c : candies) {
			if (!exist.at(c + OFFSET)) {
				exist.at(c + OFFSET) = true;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_array_cls_bool_at);

static void BM_array_c_temp(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		bool exist[VALUE_RANGE] = { false };
		size_t count = count_unique(exist, true);
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_array_c_temp);

static void BM_array_c_2(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		bool exist[VALUE_RANGE] = { false };
		size_t count = 0;
		for (int c : candies) {
			auto& loc = exist[c + OFFSET];  // minor performance trick don't seem to matter
			if (!loc) {
				loc = true;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_array_c_2);

static void BM_bitset(benchmark::State& state) {  // bitset is worse than vector<bool>, don't use either unless size if very large
	int ans = 0;
	for (auto _ : state) {
		bitset<VALUE_RANGE> exist;  // beat 31%
		size_t count = 0;
		for (int c : candies) {
			if (!exist.test(c + OFFSET)) {  // much faster to use test and set, than [], beat 89%
				exist.set(c + OFFSET, 1);
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_bitset);

static void BM_bitset_bracket(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		bitset<VALUE_RANGE> exist;  // beat 31%
		size_t count = 0;
		for (int c : candies) {
			if (!exist[c + OFFSET]) {
				exist[c + OFFSET] = 1;
				count++;
			}
		}
		ans = min(count, candies.size() / 2);
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_bitset_bracket);

static void BM_unordered_set(benchmark::State& state) {
	int ans = 0;
	for (auto _ : state) {
		ans = min(unordered_set<int>(begin(candies), end(candies)).size(), candies.size() / 2);;
	}
	state.SetItemsProcessed(ans);
}
BENCHMARK(BM_unordered_set);

/* test on Hp laptop, 4 core
BM_vector_int_temp         832909 ns     837054 ns        896   6.51042k items/s
BM_vector_char_temp         25788 ns      25670 ns      28000   6.79348k items/s
BM_vector_char              25531 ns      25495 ns      26353   7.26744k items/s
BM_vector_bool_temp         34464 ns      34254 ns      18246    7.8125k items/s
BM_vector_bool              40785 ns      40806 ns      17231   6.94444k items/s
BM_vector_bool_at           40646 ns      40807 ns      14933   8.01282k items/s
vector<char> is the best among all vectors, template function seems perform better when type is bool

BM_array_cls_int_temp       49461 ns      50000 ns      10000   9.76563k items/s
BM_array_cls_char_temp      25277 ns      25112 ns      28000   6.94444k items/s
BM_array_cls_bool_temp      25399 ns      25670 ns      28000   6.79348k items/s
BM_array_cls_char           25138 ns      25112 ns      28000   6.94444k items/s
BM_array_cls_bool           25320 ns      25112 ns      28000   6.94444k items/s
BM_array_cls_bool_at        26288 ns      26088 ns      26353   7.10227k items/s
array<char> and array<bool> are better than array<int>

BM_array_c                  25387 ns      25112 ns      28000   6.94444k items/s
BM_array_c_2                25391 ns      25670 ns      28000   6.79348k items/s
c array and class array performance is about same, vector<char> also does well

BM_bitset                   52649 ns      53013 ns      11200   8.22368k items/s
BM_bitset_bracket          127244 ns     125558 ns       5600   6.94444k items/s

BM_unordered_set          1672872 ns    1650799 ns        407   7.26744k items/s
*/
/*  test at work 8 core
BM_vector_int_temp         263993 ns
BM_vector_char_temp         20246 ns
BM_vector_char              20292 ns
BM_vector_bool_temp         28216 ns
BM_vector_bool              30810 ns
BM_vector_bool_at           28711 ns

BM_array_cls_int_temp       39698 ns
BM_array_cls_char_temp      20498 ns
BM_array_cls_bool_temp      20388 ns
BM_array_cls_bool           20359 ns
BM_array_cls_bool_at        21075 ns

BM_array_c_temp             20306 ns
BM_array_c_2                20276 ns

BM_bitset                   42620 ns
BM_bitset_bracket          101382 ns

BM_unordered_set          1329005 ns
*/