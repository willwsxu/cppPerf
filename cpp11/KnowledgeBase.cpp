#include <iostream>
#include <memory>

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

class AException {
	virtual void test() noexcept {}
	virtual void test2() throw (...) {}
};

class BException : public AException
{
	//void test() throw(out_of_range) override  {}  // exceptions in override function should be same or more restrictive
	void test2() noexcept override;
};
// prevailing practice is to not write exception spec on function as it provides few benefits to compiler
// noexcept is the only case that has some benefitsS

enum Unscoped { A=0,B=0,C,D};
enum class Scoped {A=0, B=0, C, D};
TEST_CASE("enum test", "[TEST]")
{
	CHECK(A == B);
	CHECK(int(Scoped::C) == C);
}

class SharedB;
class SharedA
{
public:
	//shared_ptr<SharedB> pB;
	weak_ptr<SharedB>	  pB;  // avoid cycle
	~SharedA()
	{
	}
};
class SharedB
{
public:
	//shared_ptr<SharedA> pA;
	weak_ptr<SharedA> pA;
	~SharedB()
	{
	}
};

TEST_CASE("shared pointer cyclic dependency, use weak_ptr test", "[TEST]")
{
	shared_ptr<SharedA> a(new SharedA());
	shared_ptr<SharedB> b(new SharedB());
	a->pB = b;
	b->pA = a;
}

// exit vs abort
// unique_lock  lock on two mutex, without deadlock deadlock