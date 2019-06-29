#include "liba.h"
#include "libb.h"
#include "boost/date_time.hpp"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    cout << testA() << endl;
    cout << testB() << endl;
    using namespace boost::gregorian;
    cout << day_clock::local_day()  << endl;
    return 0;
}