
#include <deque>
#include <vector>
#include <algorithm>
using namespace std;

// find shortest path from top left to bottom right
// each cell on path is connected and has value 0
// easy BFS, time pressure ( took 10 min, but shound in less than 5)
int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
    int N = grid.size();
    if (grid[0][0] > 0 || grid[N - 1][N - 1] > 0)  // edge case
        return -1;
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    int steps = 1;
    deque<pair<int, int>> bfs_q{ {0, 0} };
    visited[0][0] = true;
    while (!bfs_q.empty()) {
        int old_size = bfs_q.size();
        for (int i = 0; i < old_size; i++) {
            auto& front = bfs_q.front();
            const static vector<vector<int>> dir{ {0,1},{0,-1},{1,0},{-1,0},{1,1},{1,-1},{-1,1},{-1,-1} };
            for (const auto& d : dir) {
                int r = d[0] + front.first;
                int c = d[1] + front.second;
                if (r == N - 1 && c == N - 1)
                    return steps + 1;
                if (r >= 0 && r < N && c >= 0 && c < N && grid[r][c] == 0 && !visited[r][c]) {
                    visited[r][c] = true;
                    bfs_q.emplace_back(r, c);
                }
            }
            bfs_q.pop_front();
        }
        steps++;
    }
    return -1;
}

#include "catch.hpp"
TEST_CASE("1091. Shortest Path in Binary Matrix", "[BFS]")
{
    CHECK(shortestPathBinaryMatrix(vector<vector<int>>{ {0, 0, 0}, { 1, 1, 0 }, { 1, 1, 0 }}) == 4);
}