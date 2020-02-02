#include <iostream>

#include <boost/variant.hpp>
void test_variant()
{
    struct T1 {
        void fire_test() const {
            std::cout << " T1\n";
        }
    };
    struct T2 {
        void fire_test() const {
            std::cout << " T2\n";
        }
    };
    class my_visitor : public boost::static_visitor<void>
    {
    public:
        void operator()(const T1& t) const
        {
            t.fire_test();
        }

        void operator()(const T2& t) const
        {
            t.fire_test();
        }
    };
    boost::variant<T1, T2> test;
    int toggle = 0;
    if (toggle)
        test = T1();
    else
        test = T2();
    boost::apply_visitor(my_visitor(), test);
}

#include "..\catch.hpp"
TEST_CASE("variant tests", "[ALLOC]") {
    SECTION("variant test") {
        test_variant();
    }
}