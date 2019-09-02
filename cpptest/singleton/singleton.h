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
    ~Singleton()
    {
        std::cout << "dtor, this=" << this << " name=" << _name << " addr=" << &_name << std::endl;
    }
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
    std::string get_name() { return _name; }

    ~Singleton2()
    {
        std::cout << "dtor, this=" << this << " name=" << _name << " addr=" << &_name << std::endl;
    }
private:
    Singleton2() {
        _name.append(begin(_name), end(_name));
        std::cout << "\nname addr " << &_name << std::endl;
    }
    std::string _name = "singleton2_";
};
static std::string global = "global";

DLL_IMPORT_EXPORT void use_singleton();