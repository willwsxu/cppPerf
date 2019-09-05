#pragma once

#include "informer.h"
extern Informer global;

// singleton, lazy initialization
Informer& get_instance();  // multiple definition if function defined in header

// singleton, not lazy
namespace Singleton
{
    Informer& get_instance();
}
static Informer& g_not_lazy = Singleton::get_instance();


