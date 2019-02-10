#include <iostream>

using namespace std;

#include "..\catch.hpp"

// large malloc
// why virtual memory is slow
// difference of process vs thread
// false sharing: int a[2] one thread writes to a[0], another writes to a[1]
TEST_CASE("Jump memory test", "[TEST]")
{
#ifdef _WIN64
	size_t s = 32000000000LL;
	auto *large = malloc(s);
	CHECK(large == 0);
#endif
}

class Empty
{
	void empty() {}
};
class EmptyV
{
public:
	virtual ~EmptyV() {}
};
class EmptyD : public Empty, EmptyV
{
public:
	virtual ~EmptyD() {}
};

class EmptyV2
{
public:
	virtual ~EmptyV2() {}
};

class EmptyD2 : public EmptyV2, EmptyV
{
public:
	virtual ~EmptyD2() {}
};
TEST_CASE("class size test ", "[TEST]")
{
	CHECK(sizeof(Empty) == 1);  // class size cannot be zero
	CHECK(sizeof(EmptyV) == 4);
	CHECK(sizeof(EmptyD) == 4);
	CHECK(sizeof(EmptyD2) == 8); // 2 inheritance, 2 vtables
}

class A
{
	A() { cout << "construct A\n"; };
	int a;
};
class B : public A
{
};
class C: public A
{};
class D : public B, C
{};

class Bv : public virtual A
{
};
class Cv : public virtual A
{};
class Dv : public Bv, C  // one virtual base
{};
class Dvv : public Bv, Cv // birth virtual base
{};
TEST_CASE("multi inheritance", "[TEST]")
{
	CHECK(sizeof(D) == 8);
	CHECK(sizeof(Dv) == 12);  // 2 vtable, one base class A
	CHECK(sizeof(Dvv) == 12);
}