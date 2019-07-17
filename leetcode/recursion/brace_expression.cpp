#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <iostream>
#include <iterator>
using namespace std;

// store string in group (using set so it is sorted)
// recursively process one {} at a time, do product with previous group, and then continue processing (no return)
// recursively process next group when next char is comma, do union, and return result 
set<string> braceExpansionII(const string& expression, int& idx) {
    set<string> group;
    auto dot_product = [](set<string>&& a, set<string>&& b) {
        if (a.empty())
            return b;
        set<string> p;
        for (const string& s1 : a) {
            for (const string& s2 : b)
                p.insert(s1 + s2);
        }
        return p;
    };
    while (idx < expression.size()) {
        switch (expression[idx]) {
        case '{':
        {
            set<string> next_group = braceExpansionII(expression, ++idx);
            group = dot_product(move(group), move(next_group));
            break;
        }
        case '}':
            idx++;
            return group;
        case ',':
        {
            set<string> next_group = braceExpansionII(expression, ++idx);
            move(begin(next_group), end(next_group), inserter(group, begin(group))); // union
            return group;
        }
        default:  // extract all contiguous letters
        {
            int last = idx + 1;
            auto special_char = [](char c) {
                switch (c) {
                case '{':       case '}':       case ',':
                    return true;
                default:
                    return false;
                }
            };
            while (last < expression.size() && !special_char(expression[last]))
                last++;
            if (idx > 0 && expression[idx - 1] == '}') { // special case {a,b}c
                group = dot_product(move(group), set<string>{expression.substr(idx, last - idx)});
            }
            else
                group.insert(expression.substr(idx, last - idx));
            idx = last;
            break;
        }
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


vector<bool> parseBoolExpr(const string& expression, int& pos) {
    vector<bool> group;
    while (pos < expression.size()) {
        switch (expression[pos]) {
        case '&':
        case '|':
        case '!':
        {
            int op = expression[pos];
            vector<bool> next_group = parseBoolExpr(expression, ++pos);
            if (op == '&')
                group.push_back(all_of(begin(next_group), end(next_group), [](bool x) { return x == true; }));
            else if (op == '|')
                group.push_back(any_of(begin(next_group), end(next_group), [](bool x) { return x == true; }));
            else
                group.push_back(!next_group[0]);
            break;
        }
        case '(':
        case ',':
            ++pos;
            break;
        case ')':
            ++pos;
            return group;
        case 't':
            ++pos;
            group.push_back(true);
            break;
        case 'f':
            ++pos;
            group.push_back(false);
            break;
        }
    }
    return group;
}
bool parseBoolExpr(string expression) {
    int idx = 0;
    auto x = parseBoolExpr(expression, idx);
    return x[0];
}

#include <catch.hpp>

TEST_CASE("1106. Parsing A Boolean Expression", "[RECUR]")
{
    CHECK(parseBoolExpr("|(&(t,f,t),!(t))") == false);
}
TEST_CASE("1096. Brace Expansion II", "[RECUR]")
{
    CHECK(braceExpansionII("{a,b}c{d,e}f") == vector<string>{"acdf", "acef", "bcdf", "bcef"});
    CHECK(braceExpansionII("{{a,z},a{b,c},{ab,z}}") == vector<string>{"a", "ab", "ac", "z"});
    CHECK(braceExpansionII("{a,b}{c{d,e}}") == vector<string>{"acd", "ace", "bcd", "bce"});
}