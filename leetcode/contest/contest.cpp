#include <vector>
#include <algorithm>
#include <deque>
#include <string>
#include <numeric>
#include <set>
#include <iostream>
#include <iterator>
using namespace std;


set<string> braceExpansionII(const string& expression, int& idx) {
    set<string> group;
    string current;
    auto add_current = [&] {
        if (current.empty())
            return;
        group.insert(move(current));
        current.clear();
    };
    auto dot_product = [](set<string>&& a, set<string>&&b) {
        if (a.empty())
            return b;
        set<string> p;
        for (const string& s1 : a) {
            for (const string& s2 : b)
                p.insert(s1 + s2);
        }
        return p;
    };
    bool product = false;
    while (idx < expression.size()) {
        switch (expression[idx]) {
        case '{':
        {
            add_current();
            set<string> next_group = braceExpansionII(expression, ++idx);
            group = dot_product(move(group), move(next_group));
            product = true;
            break;
        }
        case '}':
            // case a{b{f,g}{m.n},c{d,e}}}
            add_current();
            idx++;
            return group;
        case ',':
        {
            add_current();
            set<string> next_group = braceExpansionII(expression, ++idx);
            move(begin(next_group), end(next_group), inserter(group, begin(group)));
            product = false;
            return group;
        }
        default: //a,b   {a,b}c
            current.push_back(expression[idx++]);
            break;
        }
    }
    return group;
}
vector<string> braceExpansionII(const string& expression)
{
    int idx = 0;
    auto res = braceExpansionII(expression, idx);
    return vector<string>(begin(res), end(res));
}
#include <catch.hpp>

TEST_CASE("1096. Brace Expansion II", "[BS]")
{
    CHECK(braceExpansionII("{a,b}c{d,e}f") == vector<string>{"acdf", "acef", "bcdf", "bcef"});
    CHECK(braceExpansionII("{{a,z},a{b,c},{ab,z}}") == vector<string>{"a", "ab", "ac", "z"});
    CHECK(braceExpansionII("{a,b}{c{d,e}}") == vector<string>{"acd", "ace", "bcd", "bce"});
}