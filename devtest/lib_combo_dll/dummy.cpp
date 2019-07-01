#include "dummy.h"
#include <iostream>
#include <string>
using namespace std;

void dummy() {
    cout << "dummy dll" << endl;
};

DummyClass::DummyClass()
{
    cout << "DummyClass constructed" << endl;
}
string DummyClass::dummy_test()
{
    cout << "DummyClass dummy_test" << endl;
    return "dummy_test";
}

void DummyClass::dummy_test(std::string& s)
{
    s.append(" add");
}