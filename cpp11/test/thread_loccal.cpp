#include <iostream>
#include <atomic>
#include <thread>
#include <future>

struct ObjectCounter {
    static std::atomic<int> g_counter;
    static std::atomic<int> g_counter_dtor;
    ObjectCounter(): m_count (++g_counter)
    {

    }
    ~ObjectCounter() {
        m_count_dtor = ++g_counter_dtor;
    }
    const int m_count;
    int m_count_dtor=0;
};
std::atomic<int> ObjectCounter::g_counter{ 0 };
std::atomic<int> ObjectCounter::g_counter_dtor{ 0 };

class Test_Thread_Local {
public:
    void fire_test(std::promise<int>&& p)
    {
        thread_local ObjectCounter obj;  // initialize once per thread, destruct on thread exit, for all objects
        p.set_value(obj.m_count);
    }
};


class Test_Thread_Local_static {
public:
    void fire_test(std::promise<int>&& p)
    {
        // static key does not impact life time, only linkage
        static thread_local ObjectCounter obj;  // initialize once per thread, destruct on thread exit, for all objects
        p.set_value(obj.m_count);
    }
};

#include "..\catch.hpp"
TEST_CASE("Test_Thread_Local", "[LOCAL]")
{
    Test_Thread_Local test1, test2;
    SECTION("test 1: thread call test1") {
        std::promise<int> p1;
        auto f1 = p1.get_future();
        std::thread t1(&Test_Thread_Local::fire_test, &test1, std::move(p1));
        t1.join();
        REQUIRE(f1.get() == 1);

        REQUIRE(ObjectCounter::g_counter_dtor.load() == 1);
    }
    SECTION("test 2: thread call test2") {
        std::promise<int> p1;
        auto f1 = p1.get_future();
        std::thread t1(&Test_Thread_Local::fire_test, &test2, std::move(p1));
        t1.join();
        REQUIRE(f1.get() == 2);
        REQUIRE(ObjectCounter::g_counter_dtor.load() == 2);
    }

    SECTION("test 3: : thread call test1 2 times") {
        std::promise<int> p1;
        auto f1 = p1.get_future();
        std::thread t1([&test1](std::promise<int>&& p) {
            std::promise<int> p2;
            test1.fire_test(std::move(p2));
            test1.fire_test(std::move(p));
            }, std::move(p1));
        t1.join();
        REQUIRE(f1.get() == 3);
        REQUIRE(ObjectCounter::g_counter_dtor.load() == 3);
    }

    SECTION("test 4: one thread call test1 test2") {
        std::promise<int> p1;
        auto f1 = p1.get_future();
        std::thread t1([&test1, &test2](std::promise<int>&& p) {
            std::promise<int> p2;
            auto f2 = p2.get_future();
            test2.fire_test(std::move(p2));
            f2.wait();
            test1.fire_test(std::move(p));
            }, std::move(p1));
        t1.join();
        REQUIRE(f1.get() == 4);
        REQUIRE(ObjectCounter::g_counter_dtor.load() == 4);
    }
}

TEST_CASE("Test_Thread_Local static", "[TLS]")
{
    auto start_count = ObjectCounter::g_counter.load();
    Test_Thread_Local_static test1, test2;
    SECTION("test 1: one thread call test1 test2") {
        std::promise<int> p1;
        auto f1 = p1.get_future();
        std::thread t1([&test1, &test2](std::promise<int>&& p) {
            std::promise<int> p2;
            auto f2 = p2.get_future();
            test2.fire_test(std::move(p2));
            f2.wait();
            test1.fire_test(std::move(p));
            }, std::move(p1));
        t1.join();
        REQUIRE(f1.get() == start_count+1);
        REQUIRE(ObjectCounter::g_counter_dtor.load() == start_count + 1);
    }

}