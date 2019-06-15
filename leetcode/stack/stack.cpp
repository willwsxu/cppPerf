#include <deque>
#include <string>
#include <map>
#include <set>
using namespace std;

string smallestSubsequence(string text) {
    map<char, int> count;
    for (char c : text)
        count[c]++;
    deque<char> seq;
    set<char> inserted;
    for (char ch : text) {
        while (!seq.empty()) {
            char top = seq.back();
            if (count[top] == 0)  // last char, must use
                break;
            if (inserted.count(ch) > 0)  // char alreay chosen
                break;
            if (ch>top)  // next char is in order
                break;
            seq.pop_back();
            inserted.erase(top);
        }
        if (inserted.count(ch) == 0) { // insert new char to sequence
            seq.push_back(ch);
            inserted.insert(ch);
        }
        count[ch]--;
    }
    return string(begin(seq), end(seq));
}

#include "catch.hpp"  // don't put this file in stdafx.h

TEST_CASE("1081. Smallest Subsequence of Distinct Characters", "[STK]")
{
    CHECK(smallestSubsequence("cdadabcc") == "adbc");
    CHECK(smallestSubsequence("cdaabcc") == "cdab");
    CHECK(smallestSubsequence("cbaacabcaaccaacababa") == "abc");
}