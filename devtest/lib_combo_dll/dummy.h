#include <string>
// dll symbols are hidden unless WINDOWS_EXPORT_ALL_SYMBOLS=ON
// generate_export_header is needed to produce export header before compile

#ifdef CMAKE_DLL
#include "libcombo_dll_export.h"
#else
#define DLL_IMPORT_EXPORT
#endif

DLL_IMPORT_EXPORT void dummy();
void dummy2();

class DLL_IMPORT_EXPORT DummyClass
{
public:
    DummyClass();
    std::string dummy_test();
    void dummy_test(std::string&);
};

class DummyClass2
{
public:
    std::string dummy_test() { return "DummyClass2"; }
};

class DummyClass3
{
public:
    std::string dummy_test();
};