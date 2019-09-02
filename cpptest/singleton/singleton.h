#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "singleton_dll_export.h"

class DLL_IMPORT_EXPORT Singleton {  // DLL_IMPORT_EXPORT is needed to export .lib file
public:
    static Singleton& instance() {
        static Singleton singleton;
        return singleton;
    }
    //void do_nothing() {};
    std::string get_name() { return _name; }

private:
    Singleton() {
        _name.append(begin(_name), end(_name));
        std::cout << "\nname addr " << &_name << std::endl;
    }
    std::string _name = "singleton_";
};
class Singleton2
{
public:
    // two instances are created if it is called within a dll and exe
    // e.g. it is called by DummyClass::dummy_test() (cpp compiled in a dll)
    // and called in main
    static Singleton2& instance()
    {
        static Singleton2 dm2;
        return dm2;
    }
    std::string dummy_test() {
        dummys.push_back(std::make_unique<int>((int)dummys.size() + 1));
        return "DummyClass2";
    }

    ~Singleton2()
    {
        std::cout << "DummyClass2 dtor, size=" << dummys.size() << " addr=" << &dummys << std::endl;
        dummys.clear();
    }
private:
    Singleton2() = default;
    std::vector<std::unique_ptr<int>> dummys;
};
DLL_IMPORT_EXPORT void use_singleton();