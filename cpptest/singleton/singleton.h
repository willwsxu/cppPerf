#pragma once
#include <string>
#include "singleton_dll_export.h"

class DLL_IMPORT_EXPORT Singleton {  // DLL_IMPORT_EXPORT is needed to export .lib file
public:
    static Singleton& instance() {
        static Singleton singleton;
        return singleton;
    }
    void do_nothing() {};

private:
    Singleton() = default;
    std::string _name = "singleton";
};
DLL_IMPORT_EXPORT void use_singleton();