#include "dummy/dummy.h"
using namespace std;

void dummy() {
    cout << "dummy dll" << endl;
};

void dummy_no_export() {
    cout << "dummy 2 no export" << endl;
};

DummyClass::DummyClass()
{
    cout << "DummyClass constructed" << endl;
}
string DummyClass::dummy_test()
{
    cout << "DummyClass dummy_test" << endl;
    cout << DummyClass2::Instance().dummy_test() << endl;
    return "dummy_test";
}

void DummyClass::dummy_test(std::string& s)
{
    s.append(" add");
}

string DummyClass3::dummy_test()
{
    cout << "DummyClass3 dummy_test" << endl;
    return "dummy_test 3";
}