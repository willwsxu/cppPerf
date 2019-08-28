//#include "libcombo_dll_export.h"

#include "liba/liba.h"
#include "libb/libb.h"
#include "dummy/dummy.h"

#include "boost/date_time.hpp"
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
    //dummy_no_export();  // cannot compile

    DummyClass dum;
    cout << "export class test " << dum.dummy_test() << endl;

    string test("test");
    dum.dummy_test(test);
    cout << "export class test2 " << test << endl;

    cout << "no export test " << DummyClass2::Instance().dummy_test() << endl;  // inline function seems exported by default
    DummyClass3 dm3;
    dm3.dummy_noop();  // silence compiler
    //cout << "dll test 3 " << dm3.dummy_test() << endl;
    return 0;
}