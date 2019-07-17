//#include "libcombo_dll_export.h"

#include "liba/liba.h"
#include "libb/libb.h"
#include "dummy/dummy.h"

#include "boost/date_time.hpp"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    // functions are not exported when merging 2 static lib into a dynamic lib
    // but works with static lib
#ifdef DUMMY_STATIC_DEFINE
    cout << testA() << endl; 
    cout << testB() << endl;
#endif
    using namespace boost::gregorian;
    cout << day_clock::local_day()  << endl;
    dummy();
    DummyClass dum;
    cout << "dll test " << dum.dummy_test() << endl;

    string test("test");
    dum.dummy_test(test);
    cout << "dll test2 " << test << endl;

    // not exported, cannot compile
    //dummy2();
    DummyClass2 dm2;
    cout << "dll test 2 " << dm2.dummy_test() << endl;  // inline function seems exported by default
    DummyClass3 dm3;
    //cout << "dll test 3 " << dm3.dummy_test() << endl;
    return 0;
}