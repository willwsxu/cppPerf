
class static_member
{
private:
    int a = 0;
public:
    int get() { return a; }
    static void incement(static_member& m);
};
void static_member::incement(static_member& m)
{
    m.a++;
}

void test()
{
}

#include "..\catch.hpp"
TEST_CASE("static acceess member", "[ACCESS]")
{
    static_member a;
    static_member::incement(a);
    CHECK(a.get() == 1);
}