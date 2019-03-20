#pragma once


/* Implicitly-declared move constructor is created if
there are no user-declared copy constructors;
there are no user-declared copy assignment operators;
there are no user-declared move assignment operators;
there are no user-declared destructors;
i.e. rule of 5
Because the presence of a user-defined destructor, copy-constructor, or copy-assignment operator 
prevents implicit definition of the move constructor and the move assignment operator, 
any class for which move semantics are desirable, has to declare all five special member functions

Rule of zero
Don't implement custom Five unless resource management is needed
*/

// rule 0, don't disable move constructor by accident
struct Derived2 : public Base
{
	Derived2(string s) :Base(s) {}
	virtual ~Derived2() = default;  // destructor prevents implicit move constructor
	virtual void do_a_thing() override {}
};


static void BM_base_ctor(benchmark::State& state) {
	for (auto _ : state)
	{
		Base b("1234567890");
	}
}
static void BM_move_ctor(benchmark::State& state) {
	for (auto _ : state)
	{
		Base b("1234567890");
		Base b2(move(b));
	}
}

static void BM_copy_ctor(benchmark::State& state) {
	for (auto _ : state)
	{
		Base b("1234567890");
		Base b2(b);
	}
}

static void BM_derived_copy_ctor(benchmark::State& state) {
	for (auto _ : state)
	{
		Derived d("1234567890");
		Derived d2(d);
	}
}
static void BM_derived_move_ctor(benchmark::State& state) {
	static_assert(sizeof(Derived) >= 4, "inheritance with virtual methods"); // 28 in release mode
	for (auto _ : state)
	{
		Derived d("1234567890");
		Derived d2(move(d));
	}
}

static void BM_derived2_failed_move_ctor(benchmark::State& state) {
	for (auto _ : state)
	{
		Derived2 d("1234567890");
		Derived2 d2(move(d));
	}
}

BENCHMARK(BM_base_ctor);//33ns
BENCHMARK(BM_copy_ctor);//50ns
BENCHMARK(BM_move_ctor);//47ns
BENCHMARK(BM_derived_copy_ctor);//67ns
BENCHMARK(BM_derived_move_ctor);//62ns
BENCHMARK(BM_derived2_failed_move_ctor);//67ns
