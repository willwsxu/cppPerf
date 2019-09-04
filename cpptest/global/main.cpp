#include "static.h"
#include "dynamic.h"
#include <iostream>

using namespace std;
int main(int argc, char* argv[])
{
    std::cout << &global << std::endl;
    std::cout << &get_global() << std::endl;

    return 0;
}