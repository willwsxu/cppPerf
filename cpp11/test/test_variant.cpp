#include <iostream>

#include <boost/variant.hpp>
int test_variant(bool toggle)
{
    struct T1 {
        int fire_test() const {
            std::cout << " T1\n";
            return 1;
        }
    };
    struct T2 {
        int fire_test() const {
            std::cout << " T2\n";
            return 2;
        }
    };
    class my_visitor : public boost::static_visitor<int>
    {
    public:
        int operator()(const T1& t) const
        {
            return t.fire_test();
        }

        int operator()(const T2& t) const
        {
            return t.fire_test();
        }
    };
    boost::variant<T1, T2> test;
    if (toggle)
        test = T1();
    else
        test = T2();
    return boost::apply_visitor(my_visitor(), test);
}

#include "..\catch.hpp"
TEST_CASE("variant tests", "[ALLOC]") {
    SECTION("variant test") {
        REQUIRE (test_variant(false)==2);
    }
}