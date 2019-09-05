#include "static.h"

using namespace std;

Informer global("global");

Informer& get_instance() {
    static Informer g_informer("lazy singleton");
    return g_informer;
}
namespace Singleton
{
    Informer& get_instance() {
        static Informer g_informer("singleton not lazy");
        return g_informer;
    }
}