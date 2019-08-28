#include <string>
#include <vector>
#include <iostream>
#include <memory>

// dll symbols are hidden unless WINDOWS_EXPORT_ALL_SYMBOLS=ON
// generate_export_header is needed to produce export header before compile

#if (defined(CMAKE_DLL) || defined(DUMMY_STATIC_DEFINE))
#include "libcombo_dll_export.h"
#else
#define DLL_IMPORT_EXPORT
#endif

DLL_IMPORT_EXPORT void dummy();
void dummy_no_export();

class DLL_IMPORT_EXPORT DummyClass
{
public:
    DummyClass();
    std::string dummy_test();
    void dummy_test(std::string&);
};

// no export, all methods must be inline
class DummyClass2
{
public:
    // two instances are created if it is called within a dll and exe
    // e.g. it is called by DummyClass::dummy_test() (cpp compiled in a dll)
    // and called in main
    static DummyClass2& Instance()
    {
        static DummyClass2 dm2;
        return dm2;
    }
    std::string dummy_test() {
        dummys.push_back(std::make_unique<int>((int)dummys.size() + 1));
        return "DummyClass2"; 
    }

    ~DummyClass2()
    {
        std::cout << "DummyClass2 dtor, size=" << dummys.size() << " addr=" << &dummys << std::endl;
        dummys.clear();
    }
private:
    DummyClass2() = default;
    std::vector<std::unique_ptr<int>> dummys;
};

// no export, method is not inline, not usable
class DummyClass3
{
public:
    void dummy_noop() {}
    std::string dummy_test();
};