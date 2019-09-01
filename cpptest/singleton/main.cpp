#include "singleton.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    use_singleton();
    Singleton::instance().do_nothing();
    cout << "main call:" << Singleton::instance().get_name() << endl;
    return 0;
}