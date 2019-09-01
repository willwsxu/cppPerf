#include "singleton.h"

int main(int argc, char* argv[])
{
    use_singleton();
    Singleton::instance().do_nothing();
    return 0;
}