#include <string>
using namespace std;

#include "ObjectFactory.h"
#include "..\catch.hpp"

class TestBase
{
public:
	virtual ~TestBase() {}  // enable dynamic cast
};
class Test1 : public TestBase
{

};
TestBase *createTest1()
{
	return new Test1();;
}

class Test2 : public TestBase
{

};

using TestFactory = Factory<TestBase, int, TestBase * (*)(), DefaultError>;


namespace
{
	TestFactory& factory2 = TestFactory::Instance();
	const bool registered = factory2.Register(2, []()-> TestBase * {return new Test2(); });  // fail to compile without assignment??
};

TEST_CASE("Object Factory", "[FACT]")
{
	TestFactory& factory = TestFactory::Instance();
	factory.Register(1, &createTest1);
	CHECK(dynamic_cast<Test1*>(factory.CreateObject(1)) != nullptr);
	CHECK(dynamic_cast<Test2*>(factory.CreateObject(2)) != nullptr);
	CHECK(factory.CreateObject(100) == nullptr);
}

TEST_CASE("Object Factory, variadic", "[FACT]")
{
	using TestFactory = Factory<TestBase, int, TestBase * (*)(const string&), DefaultError>;
	TestFactory& factory = TestFactory::Instance();	
	
	const bool registered = factory.Register(2, [](const string&)-> TestBase * {return new Test2(); });
	CHECK(registered == true);
	CHECK(factory.CreateObject(2, "test") != nullptr);
}