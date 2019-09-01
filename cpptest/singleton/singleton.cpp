#include "singleton.h"
#include <iostream>
using namespace std;

void use_singleton()
{
    cout << "dll call:" << Singleton::instance().get_name() << endl;
}