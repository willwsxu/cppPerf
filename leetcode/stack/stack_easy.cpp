#include <deque>
#include <string>
#include <algorithm>

using namespace std;
// remove 2 adjacent duplicate letters until none left
string removeDuplicates(string S) {
    deque<char> stk;
    auto start = begin(S);
    while (start != end(S)) {
        if (stk.empty() || stk.back() != *start) {
            stk.push_back(*start);
        }
        else {  // find duplciate letter
            stk.pop_back();
        }
        ++start;
    }
    S.assign(begin(stk), end(stk));
    return S;
}

#include "catch.hpp"
TEST_CASE("1047. Remove All Adjacent Duplicates In String", "[STK]")
{
    CHECK(removeDuplicates("aaaaaaaaa") == "a");
    CHECK(removeDuplicates("abbaca") == "ca");
}