#include "singleton.h"
using namespace std;

void static_call ()
{
    cout << "main call:" << Singleton::instance().get_name() << endl;

    cout << Singleton2::instance().get_name() << endl;
}