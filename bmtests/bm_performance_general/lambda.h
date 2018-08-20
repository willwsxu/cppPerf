#pragma once

// prefer lambda over bind/function object
string add(const string&lhs, const string&rhs)
{
	return lhs + rhs;
}

static void BM_lambda(benchmark::State& state) {
	const auto good = [](const string&b) {
		return add("Hello ", b);
	};
	for (auto _ : state)
	{
		good("World");
	}
}
static void BM_bind(benchmark::State& state) {
	const auto bad = bind(add, "Hello ", placeholders::_1);
	for (auto _ : state)
	{
		bad("World");
	}
}

static void BM_function(benchmark::State& state) {
	const function<string(const string&)> worse = bind(add, "Hello ", placeholders::_1);
	for (auto _ : state)
	{
		worse("World");
	}
}

BENCHMARK(BM_lambda); // 76ns
BENCHMARK(BM_bind);   // 106ns
BENCHMARK(BM_function);//118ns
