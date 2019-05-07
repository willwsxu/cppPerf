#include <vector>
#include <algorithm>
using namespace std;

class Solution {
	void dfs(vector<vector<int>>& result, const vector<vector<int>>& grid, vector<vector<bool>>& visited, int r, int c, int color, int mycolor) {
		if (r < 0 || c < 0 || r >= grid.size() || c >= grid[0].size() || grid[r][c] != mycolor || visited[r][c])
			return;
		visited[r][c] = true;
		if (r == 0 || c == 0 || r == grid.size() - 1 || c == grid[0].size() - 1)  // border cell
			result[r][c] = color;
		else if (grid[r - 1][c] != mycolor || grid[r + 1][c] != mycolor || grid[r][c - 1] != mycolor || grid[r][c + 1] != mycolor)
			result[r][c] = color;
		dfs(result, grid, visited, r + 1, c, color, mycolor);
		dfs(result, grid, visited, r - 1, c, color, mycolor);
		dfs(result, grid, visited, r, c + 1, color, mycolor);
		dfs(result, grid, visited, r, c - 1, color, mycolor);
	}
public:
	vector<vector<int>> colorBorder(vector<vector<int>> & grid, int r0, int c0, int color) {
		if (grid[r0][c0] == color)
			return grid;
		vector<vector<int>> result = grid;
		vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));
		dfs(result, grid, visited, r0, c0, color, grid[r0][c0]);
		return result;
	}
};
int main()
{
	vector<vector<int>> grid{ {1,1},{1,2} };
	auto x = Solution().colorBorder(grid, 0, 0, 3);
	return 0;
}