#include "singleton.h"
#include "static_lib.h"

using namespace std;
int main(int argc, char* argv[])
{
    cout << "main call:" << Singleton::instance().get_name() << endl;

    cout << Singleton2::instance().get_name() << endl;

    use_singleton();

    std::cout << global << std::endl;
    std::cout << get_global() << std::endl;

    return 0;
}