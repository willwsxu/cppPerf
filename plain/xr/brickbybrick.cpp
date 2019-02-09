/*
  This cpp file builds without any dependency other than c++14 standard library
  It is built and test in visual studio 2015 and 2017, as well as g++ 8.2.0, C++14 standard
  Here is the instruction for visual studio:
  1. create an empty general c++ project
  2. copy this cpp file to project folder
  3. open project, right click on 'source files' and choose menu to add existing item
  3.1 browse file explorer to find this cpp file
  4 build release version
  5.run exe from release from a command window

  I send two zip files, stand alone cpp that you can try to build any c++ 14 compiler. Or use my visual studio project
*/
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <map>

using namespace std;

// valid tile patterns of size N, single row
void valid_tile(int N, int len, vector<int>&pattern, vector<vector<int>>& patterns)
{
    if (len >= N) {
        if (len==N)
            patterns.push_back(pattern);
        return;
    }
    pattern.push_back(len + 2);
    valid_tile(N, len + 2, pattern, patterns);
    pattern.back() += 1;
    valid_tile(N, len + 3, pattern, patterns);
    pattern.pop_back();
}

// pre-compute all patterns which did not form crack with a previous row
// similar to valid_tile, with additional checking for crack
void neighbor_pattern(const vector<int>& prev_pattern, int len, vector<int>&pattern, map<vector<int>, vector<vector<int>>>& patterns_no_crack) 
{
    if (len >= prev_pattern.back()) {
        if (len == prev_pattern.back())
            patterns_no_crack[prev_pattern].push_back(pattern);
        return;
    }
    for (int size=len+2; size <=len+3; size++) {  // try 2x1 and 3x1 bricks
        if (size== prev_pattern.back() || !binary_search(begin(prev_pattern), end(prev_pattern), size)) {  // row is complete, or no crack
            pattern.push_back(size);
            neighbor_pattern(prev_pattern, size, pattern, patterns_no_crack);
            pattern.pop_back();
        }
    }
}

// ways to build a wall row by row, from one pattern of first row
long long build_wall(const vector<int>& prev_pattern, int height, map<vector<int>, vector<vector<int>>>& patterns_no_crack, vector<map<vector<int>, long long>>& memo)
{
    if (height == 0)  // base case, last row
        return 1;
    auto found = memo[height].find(prev_pattern);
    if (found == end(memo[height])) {  // first time to see this pattern on this row
        const auto& patterns = patterns_no_crack[prev_pattern];
        long long result = 0;
        for (const auto& p : patterns) {
            result += build_wall(p, height - 1, patterns_no_crack, memo);
        }
        memo[height][prev_pattern] = result;
        return result;
    }
    return found->second;
}

// ways to build a wall
// 1. full search to find all valid patterns to build a single row
// 2. for each valid pattern of a row, find all possible valid patterns qualified to be next row without crack
// 3. given any first row, find ways to build a wall, memoize result of each pattern at each row
long long W(int width, int height) 
{
    vector<vector<int>> patterns;
    vector<int> p;
    valid_tile(width, 0, p, patterns);  // step 1
    map<vector<int>, vector<vector<int>>> patterns_no_crack;
    for (const auto& prev : patterns) {  // step 2
        neighbor_pattern(prev, 0, p, patterns_no_crack);
    }
    long long ways = 0;
    vector<map<vector<int>, long long>> memo(height);  // memoize ways to build a wall from certain row and a pattern
    for (const auto& prev : patterns) {  // step 3
        ways += build_wall(prev, height - 1, patterns_no_crack, memo);
    }
    return ways;
}

int main(int argc, char* argv[]) {
	//cout << W(9, 3) << "\n";
    cout << W(32, 10) << "\n";  // 806844323190414
    return 0;
}