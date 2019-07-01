#include <string>
// dll symbols are hidden unless WINDOWS_EXPORT_ALL_SYMBOLS=ON
// generate_export_header is needed to produce export header before compile

#include "libcombo_dll_export.h"

void dummy();
class DummyClass
{
public:
    DummyClass();
    std::string dummy_test();
    void dummy_test(std::string&);
};