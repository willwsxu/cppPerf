#include <vector>
#include <algorithm>
#include <deque>
#include <string>
#include <numeric>
#include <iostream>
using namespace std;


vector<string> braceExpansionII(const string& expression, int& idx) {
    vector<string> group;
    string current;
    auto add_current = [&] {
        if (current.empty())
            return;
        group.push_back(current);
        current.clear();
    };
    auto dot_product = [](vector<string>&& a, vector<string>&&b) {
        if (a.empty())
            return b;
        vector<string> p;
        p.reserve(a.size() * b.size());
        for (const string& s1 : a) {
            for (const string& s2 : b)
                p.push_back(s1 + s2);
        }
        return p;
    };
    bool product = false;
    while (idx < expression.size()) {
        switch (expression[idx]) {
        case '{':
        {
            add_current();
            vector<string> next_group = braceExpansionII(expression, ++idx);
            group = dot_product(move(group), move(next_group));
            product = true;
            break;
        }
        case '}':
            // case a{b{f,g}{m.n},c{d,e}}}
            add_current();
            idx++;
            sort(begin(group), end(group));
            return group;
        case ',':
            idx++;
            add_current();
            product = false;
            break;
        default: //a,b   {a,b}c
            current.push_back(expression[idx++]);
            break;
        }
    }
    sort(begin(group), end(group));
    return group;
}
vector<string> braceExpansionII(const string& expression)
{
    int idx = 0;
    return braceExpansionII(expression, idx);
}
#include <catch.hpp>

TEST_CASE("1096. Brace Expansion II", "[BS]")
{
    CHECK(braceExpansionII("{a,b}{c{d,e}}") == vector<string>{"acd", "ace", "bcd", "bce"});
    CHECK(braceExpansionII("{{a,z},a{b,c},{ab,z}}") == vector<string>{"a", "ab", "ac", "z"});
}