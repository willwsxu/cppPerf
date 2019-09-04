#pragma once
#include <iostream>

class Informer
{
public:
    Informer() {
        std::cout << "Informer ctor " << this << std::endl;
    }
    ~Informer() {
        std::cout << "Informer dtor " << this << std::endl;
    }
};