#pragma once
#include <string>
#include <iostream>
#include "singleton_dll_export.h"

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
        _name.append(begin(_name), end(_name));
        std::cout << "\nname addr " << &_name << std::endl;
    }
    std::string _name = "singleton_";
};
DLL_IMPORT_EXPORT void use_singleton();