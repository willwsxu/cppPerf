
class static_member
{
private:
    int a = 0;
public:
    int get() { return a; }
    static void incement(static_member& m);  // equavalent to friend in term of accessing member
};
void static_member::incement(static_member& m)
{
    m.a++;
}

#include "..\catch.hpp"
TEST_CASE("static acceess member", "[ACCESS]")
{
    static_member a;
    static_member::incement(a);
    CHECK(a.get() == 1);
}