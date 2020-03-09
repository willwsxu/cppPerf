//#include "simple_async.h"
#include <vector>
#include <string>
#include <thread>

#ifndef WIN32
#include "gperftools/heap-profiler.h"
#endif

template<typename T>
std::vector<T> test_memory(int count)
{
    std::vector<T> test;
    for (int i = 0; i < count; i++) {
        test.push_back(T{ 50 });
        using namespace std::chrono_literals;
        //std::this_thread::sleep_for(1ms);
    }
    return test;
}

void testing()
{
#ifndef WIN32
    HeapProfilerStart("/tmp/test1.hprof");
#endif
    long *z = new long[10000000];
#ifndef WIN32
    HeapProfilerDump("peek1");
#endif
    auto x = test_memory<int>(10000000);
    auto y = test_memory<std::string>(10000000);
#ifndef WIN32
    HeapProfilerStop();
#endif
}

int main(int argc, char* argv[])
{
    testing();
    //async_unit_test();
    return 0;
}