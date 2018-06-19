#include "..\catch.hpp"

#include "FunctorX.h"
struct TestFun1
{
	bool operator()(string i, double d)
	{
		cout << "test1" << endl;
		return true;
	}
};


struct TestFun2
{
	void test(string i, double d)
	{
		cout << "test2" << endl;
	}
};

bool test3(string i, double d)
{
	return true;
}

void test4(string i, double d)
{
}

const char * test5(string i, double d)
{
	return "test";
}

TEST_CASE("Functor", "[FUNC]")
{
	TestFun1 f;
	Functor<bool, string, double> cmd(f);
	cmd("4", 4.5);

	Functor<bool, string, double> cmd2(&test3);  // must use function pointer
	cmd2("4", 4.5);

	Functor<void, string, double> cmd3(&test4);    // return void

	Functor<string, string, double> cmd4(&test5);  // auto convert return type, from const char * to string

												   //	Functor<bool, string, double> cmd5(&TestFun2::test);  // member function pointer
}