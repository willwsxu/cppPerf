#define UNIT_TEST
#include "simple_async.h"

#include <future>
#include <memory>
#include <functional>

int main()
{
    async_unit_test();

    using UserData = std::shared_ptr<void>;
    using Callback = std::function<void(UserData)>;
    using Callback_Ref = std::function<void(UserData&)>;
    UserData data;
    Callback cb = [](UserData) {};
    Callback_Ref cb_ref = [](UserData& dd) {};
    std::future<void>  x = std::async(cb, data);
    std::future<void>  y = std::async(cb_ref, std::ref(data));  // std::ref is not required by MSVC, but is required by gnu
}
// g++ -std=c++14 -pthread test.cpp