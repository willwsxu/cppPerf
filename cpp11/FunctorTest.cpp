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
	TestFun2(int id):m_id(id) 	{}
	int test(string i, double d)
	{
		cout << "test2" << endl;
		return m_id;
	}
private:
	int m_id;
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
	CHECK(cmd("4", 4.5) == true);

	Functor<bool, string, double> cmd2(&test3);  // must use function pointer
	CHECK(cmd2("4", 4.5) == true);;

	Functor<void, string, double> cmd3(&test4);    // return void
	Functor<string, string, double> cmd4(&test5);  // auto convert return type, from const char * to string
	using ssdFunc = Functor<string, string, double>;
	auto up = make_unique<ssdFunc>(cmd4);
	// ssdFunc cmd5(up);
												
	TestFun2 t(2);
	Functor<int, string, double> cmd5(&t, &TestFun2::test);  // member function pointer
	CHECK(cmd5("5", 5.5)==2);
}