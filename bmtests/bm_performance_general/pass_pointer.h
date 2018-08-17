#pragma once

//don't pass shared_ptr if no ownership is involved
static void BM_pass_shared_ptr_ref(benchmark::State& state) {
	auto d = make_shared<Derived>("test");
	auto bad = [](const shared_ptr<Base>& ) {};
	for (auto _ : state)
	{
		bad(d);
	}
}

static void BM_pass_shared_ptr_copy(benchmark::State& state) {
	auto d = make_shared<Derived>("test");
	auto bad = [](const shared_ptr<Base> ) {};
	for (auto _ : state)
	{
		bad(d);
	}
}
static void BM_pass_unique_ptr(benchmark::State& state) {
	auto d = make_unique<Derived>("test");
	auto good = [](unique_ptr<Derived> x) { return move(x); };
	for (auto _ : state)
	{
		d=good(move(d));
	}
}
static void BM_pass_raw_reference(benchmark::State& state) {
	auto d = make_shared<Derived>("test");
	auto good = [](const Base&) {};
	for (auto _ : state)
	{
		good(*d.get());
	}
}
static void BM_pass_raw_pointer(benchmark::State& state) {
	auto d = make_shared<Derived>("test");
	auto good = [](const Base*) {};
	for (auto _ : state)
	{
		good(d.get());
	}
}

BENCHMARK(BM_pass_shared_ptr_copy); // 15ns
BENCHMARK(BM_pass_shared_ptr_ref);  // 14ns
BENCHMARK(BM_pass_unique_ptr);		// 2ns
BENCHMARK(BM_pass_raw_reference);   // 1ns
BENCHMARK(BM_pass_raw_pointer);   // 1ns
