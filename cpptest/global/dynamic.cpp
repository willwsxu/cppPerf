#include "static.h"
#include "dynamic.h"

Informer& get_global()
{
    return global;
}

Informer& get_global_lazy()
{
    return get_instance();
}


Informer& get_global_not_lazy()
{
    return g_not_lazy;
}


Informer& get_global_fast()
{
    return g_fast;
}