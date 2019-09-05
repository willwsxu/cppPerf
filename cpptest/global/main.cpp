#include "static.h"
#include "dynamic.h"
#include <iostream>

using namespace std;
int main(int argc, char* argv[])
{
    // 3 constructor calls before main, because of static variable
    std::cout << "test global, construct/destruct twice\n";
    std::cout << &global << std::endl;
    std::cout << &get_global() << std::endl;

    std::cout << "test singleton, construct/destruct once\n";
    std::cout << &get_instance() << std::endl;
    std::cout << &get_global_lazy() << std::endl;

    std::cout << "test singleton with pre-initialization, , construct/destruct once\n";
    std::cout << &g_not_lazy << std::endl;
    std::cout << &get_global_not_lazy() << std::endl;

    return 0;
}