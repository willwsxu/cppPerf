#pragma once
#include <string>
#include "singleton_dll_export.h"

static int count = 0;
class DLL_IMPORT_EXPORT Singleton {  // DLL_IMPORT_EXPORT is needed to export .lib file
public:
    static Singleton& instance() {
        static Singleton singleton;
        return singleton;
    }
    void do_nothing() {};
    std::string get_name() { return _name; }

private:
    Singleton() {
        _name.append(1, '0' + count++);
    }
    std::string _name = "singleton";
};
DLL_IMPORT_EXPORT void use_singleton();