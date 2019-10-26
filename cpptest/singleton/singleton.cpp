#include "singleton.h"
#include "static_lib.h"
using namespace std;

void use_singleton()
{
    cout << "dll call:" << Singleton::instance().get_name() << endl;
    cout << Singleton2::instance().get_name() << endl;
}
std::string& get_global()
{
    return global;
}
