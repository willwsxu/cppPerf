#pragma once
#include <mutex>
#include <future>
#include <deque>
#include <thread>
#include <condition_variable>

#include <sstream>
#include <iostream>
class SimpleAsync
{
public:
    SimpleAsync()
    {
        result_threead = std::thread([this]() {
            int call_count = 0;
            while (!stop_token) {
                std::future<void> result;
                {
                    std::unique_lock<std::mutex> g(m);
                    cv.wait(g, [this]() { return !task_results.empty();  });
                    result = std::move(task_results.front());
                    task_results.pop_front();
                }

                result.wait();
                //<<<<test code
                std::stringstream istr;
                istr << "async task " << ++call_count << " done\n";
                std::cout << istr.str();
                //>>>>>test code
            }
            std::cout << "SimpleAsync thread loop exit\n" << std::flush;
        });
    }
    template<typename func_void, typename ...Args>
    void submit(func_void&& func, Args&& ... args)
    {
        {
            std::lock_guard<std::mutex> lk(m);
            task_results.push_back(std::async(std::launch::async, std::forward<func_void>(func), std::forward<Args>(args)...));
        }
        cv.notify_one();
    }
    ~SimpleAsync()
    {
        stop_token = true;
        submit([]() { std::cout << "SimpleAsync last task\n" << std::flush;  });
        result_threead.join();
        std::cout << "task results in destructor " << task_results.size() << "\n" << std::flush;
    }
private:
    std::mutex m;
    std::condition_variable cv;
    std::thread result_threead;
    bool stop_token = false;

    std::deque<std::future<void>>  task_results;
};

#ifdef UNIT_TEST

#include <iostream>
#include <chrono>

using namespace std::chrono_literals;
template<typename ...Args> void logger(Args&& ...args)
{
    std::stringstream istr;
    istr << "tid=" << std::this_thread::get_id() << ": ";
    (istr << ... << args) << "\n";  // require c++17
    std::cout << istr.str() << std::flush;
}
void test()
{
    std::this_thread::sleep_for(200ms);
    logger("test function with no param");
}
void test2(int p1, int p2)
{
    std::this_thread::sleep_for(100ms);
    logger("test function with 2 param (", p1, ",", p2, ")");
}
void test_const_ref(const int& p) {
    std::this_thread::sleep_for(10ms);
    logger("test function with const ref param (", p, ")");
}
void test_ref(int& p) {
    std::this_thread::sleep_for(10ms);
    logger("test function with ref param (", p, ")");
}
void test_ref_share_ptr(std::shared_ptr<std::string>& p) {
    std::this_thread::sleep_for(10ms);
    logger("test function with ref shared_ptr param (", (p?*p:"nullptr"), ")");
}
void test_share_ptr(std::shared_ptr<std::string> p) {
    std::this_thread::sleep_for(10ms);
    logger("test function with shared_ptr param (", *p, ")");
}
struct TestClass
{
    void test(int p1, int p2) const
    {
        std::this_thread::sleep_for(75ms);
        logger("test class method with 2 param (", p1, ",", p2, ")");
    }
};

void async_unit_test()
{
    {
        (void)std::async(std::launch::async, test);
        std::cout << "destructor of returned std::future will wait till task complete\n" << std::flush;
        (void)std::async(std::launch::async, test2, 0, 1);
    }

    SimpleAsync async;
    std::cout << "\ntest no param will finish last with SimpleAsync, due to sleep\n" << std::flush;
    async.submit(test);
    async.submit(test2, 2, 3);
    TestClass t;
    async.submit(&TestClass::test, &t, 4, 5);

    async.submit([t](int p1, int p2) { t.test(p1, p2); }, 6, 7);

    async.submit([t](int p1, int p2) { t.test(p1, p2); }, 8, 9);  // prove async use thread_pool

    async.submit([t](int p1, int p2) { t.test(p1, p2); }, 10, 11);
    {
        auto x = [t](std::shared_ptr<int> p1, std::shared_ptr<int> p2) {
            t.test(*p1, *p2);
        };
        auto p1 = std::make_shared<int>(12);
        auto p2 = std::make_shared<int>(13);
        async.submit(x, p1, p2);
    }
    {
        int j = 14, k = 15;
        // task 8 to 12
        async.submit(test_const_ref, std::cref(j));       // show 140, surprising to me
        async.submit(test_ref, std::ref(k));              // undefined behavior, show 150
        j = 140;
        k = 150;

        auto str = std::make_shared<std::string>("shared_ptr string");
        async.submit(test_share_ptr, str);

        auto str2 = std::make_shared<std::string>("shared_ptr ref string");
        async.submit(test_ref_share_ptr, std::ref(str2));  // str2 is out of scope in async, invalid state, crash

        auto str3 = std::make_shared<std::string>("shared_ptr ref string with lambda");
        async.submit([str3]() mutable { test_ref_share_ptr(str3); }); // use lambda to keep shared_ptr alive
    }

    std::this_thread::sleep_for(250ms);  // wait to delay async destructor
}
#endif