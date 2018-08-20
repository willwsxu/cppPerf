#pragma once

#include <string>
struct Str
{
	Str(string s) :_s(move(s)) {}
	//Str(string&& s) :_s(move(s)) {}
private:
	string _s;
};

// use initilizer, use const when possible
static void BM_redundant_construction(benchmark::State& state) {
	for (auto _ : state)
	{
		string t("test");
		t += "more";
		Str s(move(t));
	}
}
static void BM_redundant_construction_append(benchmark::State& state) {
	for (auto _ : state)
	{
		string t("test");
		t.append("more");
		Str s(move(t));
	}
}
static void BM_initializer_construction_append(benchmark::State& state) {
	for (auto _ : state)
	{
		Str s(move(string("test").append("more")));
	}
}

static void BM_initializer_construction(benchmark::State& state) {
	for (auto _ : state)
	{
		Str s(move(string("test") + "more")); // + is more costly then +=
	}
}

static void BM_initializer_construction_append_no_move(benchmark::State& state) {
	for (auto _ : state)
	{
		Str s(string("test").append("more"));
	}
}
BENCHMARK(BM_redundant_construction);  // 51ns
BENCHMARK(BM_redundant_construction_append);  // 50ns
BENCHMARK(BM_initializer_construction_append); // 49ns
BENCHMARK(BM_initializer_construction_append_no_move); //55ns
BENCHMARK(BM_initializer_construction);// 68ns, extra malloc
