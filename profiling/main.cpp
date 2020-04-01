//#include "simple_async.h"
#include <vector>
#include <string>
#include <thread>

#ifndef WIN32
#include "gperftools/heap-profiler.h"
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>      // std::ifstream
#include <ios>

#include <sys/types.h>
#include <unistd.h>
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

#ifndef WIN32
void GetMemoryUsage(size_t& size, size_t& resident, size_t& shared, size_t& text, size_t& data)
{
    static int page_size = getpagesize();
    std::ifstream stat_stream("/proc/self/statm", std::ios_base::in);

    size_t lib;   //unused
    stat_stream >> size >> resident >> shared >> text >> lib >> data;
    stat_stream.close();

    //  convert to bytes
    size *= page_size;
    resident *= page_size;
    shared *= page_size;
    text *= page_size;
}
void GetSystemStats(size_t& used, size_t& free)
{
    // Setup our pipe for reading and execute our command.
    FILE* pf = popen("free -m", "r");
    if (pf == NULL)
    {
        fprintf(stderr, " Error: popen\n");
        return;
    }
    /*
    Typical "free -m" output:
             total       used       free     shared    buffers     cached
Mem:         15949      15582        367          1         49      14800
-/+ buffers/cache:        732      15217
Swap:         4095         21       4074
    */
    // Read the output and parse it
    char l0[1024];
    char l1[1024];
    char l2[1024];
    char l3[1024];
    fgets(l0, sizeof l0, pf);
    fgets(l1, sizeof l1, pf);
    fgets(l2, sizeof l2, pf);
    fgets(l3, sizeof l3, pf);
    //printf("l0=%s", l0);
    printf("l1=%s", l1);
    //printf("l2=%s", l2);
    char dummy1[64];
    char dummy2[64];
    sscanf(l1, "%s %s %zu %zu", dummy1, dummy2, &used, &free);
    if (pclose(pf) != 0)
    {
        fprintf(stderr, " Error: Failed to close command stream \n");
    }
}
void LogStats(const int iteration, const char* msg)
{
    size_t size;
    size_t resident;
    size_t shared;
    size_t text;
    size_t data;
    printf("%s\n", "Iter Tag               Size   Resident Shared  Data    Used    Free");
    GetMemoryUsage(size, resident, shared, text, data);    //  convert to MBytes
    double size_d = ((double)size) / (1024 * 1024);
    double resident_d = ((double)resident) / (1024 * 1024);
    double shared_d = ((double)shared) / (1024 * 1024);
    double data_d = ((double)data) / (1024 * 1024);
    size_t used;
    size_t free;
    //  Returns the usage in MBytes - see GetSystemStats
    GetSystemStats(used, free);
    printf("%3d  %-16s %7.1f %7.1f %7.1f %7.1f %7.1f %7.1f \n", iteration, msg, size_d, resident_d, shared_d, data_d, (double)used, (double)free);
}
#endif

void testing()
{
#ifndef WIN32
    LogStats(0, "initial");
    HeapProfilerStart("/tmp/test1.hprof");
    HeapProfilerDump("initial");
#endif
    long *z = new long[100000000];
#ifndef WIN32
    char szTemp[32];
    snprintf(szTemp, sizeof(szTemp), "after %ul MB", sizeof(long[10000000])/1000000);
    LogStats(1, szTemp);
    HeapProfilerDump("peek1");
#endif
    auto x = test_memory<int>(10000000);
    auto y = test_memory<std::string>(10000000);
#ifndef WIN32
    LogStats(1, "end");
    HeapProfilerStop();
#endif
}

int main(int argc, char* argv[])
{
    testing();
    return 0;
}