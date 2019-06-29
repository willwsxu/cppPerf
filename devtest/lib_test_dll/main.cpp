//#include "libcombo_dll_export.h"

#include "liba.h"
#include "libb.h"
#include "dummy.h"

#include "boost/date_time.hpp"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    // functions are not exported when merging 2 static lib into a dynamic lib
    //cout << testA() << endl;
    //cout << testB() << endl;
    using namespace boost::gregorian;
    cout << day_clock::local_day()  << endl;
    dummy();
    return 0;
}