#include <map>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;
int compute(map<char, int>& counters, int choose)
{
    if (choose == 1) {
        int kinds = count_if(begin(counters), end(counters), [](const auto & c) { return c.second > 0; });
        return kinds;
    }
    int answer = 0;
    for (auto& entry : counters) {
        if (entry.second == 0)
            continue;
        entry.second--;
        answer += compute(counters, choose - 1);
        entry.second++;
    }
    return answer;
}

// find all unique sequence of letters
// idea: try sequence of 1 .. N, count unique letters
// for each sequence, pick each unique letter, then recursively pick next till done
// back tracking to restore counter after each try
int numTilePossibilities(string tiles) {
    map<char, int> count;
    for (char c : tiles)
        count[c]++;
    int answer = 0;
    for (int i = 0; i < tiles.size(); i++) {
        answer += compute(count, i + 1);
        //cout << answer << endl;
    }
    return answer;
}


#include "catch.hpp"  // don't put this file in stdafx.h

TEST_CASE("1079. Letter Tile Possibilities", "[BT]")
{
    CHECK(numTilePossibilities("AAB") == 8);
    CHECK(numTilePossibilities("AAABBC") == 188);
}