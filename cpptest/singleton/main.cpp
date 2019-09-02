#include "singleton.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    use_singleton();
    cout << "main call:" << Singleton::instance().get_name() << endl;

    cout << Singleton2::instance().dummy_test() << endl;

    return 0;
}