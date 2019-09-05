#pragma once
#include <iostream>
#include <string>

class Informer
{
public:
    Informer(const char *n):name(n) {
        std::cout << "Informer ctor " << this << " name=" << name << std::endl;
    }
    ~Informer() {
        std::cout << "Informer dtor " << this << " name=" << name << std::endl;
    }
private:
    std::string name;
};