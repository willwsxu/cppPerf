#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"

using namespace std;

class Solution {
public:
	// 797. All Paths From Source to Target
	// Given a directed acyclic graph of n nodes, find all paths from 0 to node n-1
	// input: nodes that each node is connected to
	void allPathsSourceTarget(vector<vector<int>>& graph, vector<vector<int>>& all, vector<int>&path, int node) {
		path.push_back(node);
		if (node == graph.size()-1) {
			all.push_back(path);
		}
		else {
			for (const auto& to : graph[node]) {
				allPathsSourceTarget(graph, all, path, to);
			}
		}
		path.pop_back();
	}
	vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {// beat 20, try dfs and dp later
		vector<vector<int>> all;
		vector<int> path{};
		allPathsSourceTarget(graph, all, path, 0);
		return all;
	}

	int count = 0;
	void dfs(const vector<vector<int>>& rooms, vector<int>& visited, int v)
	{
		if (visited[v])
			return;
		visited[v] = true;
		count++;
		for (int r : rooms[v]) {
			dfs(rooms, visited, r);
		}
	}
	// 841. Keys and Rooms, each room may contain keys that open other rooms
	bool canVisitAllRooms(vector<vector<int>>& rooms) {// a typical dfs, beat 100% after submit again
		int n = rooms.size();
		vector<int> visited(n, false);
		dfs(rooms, visited, 0);  // visit from room 0 only
		//return all_of(visited.begin(), visited.end(), [](bool v) { return v; });
		return count == n;
	}
};


class ConnectedComponents {
	vector<bool> visited;
	void dfs(vector<vector<int>>& M, int i)  // find all people share friendship
	{
		if (visited[i])
			return;
		visited[i] = true;
		for (size_t f = 0; f < M.size(); f++) {  // adjacency matrix define directed friendship
			if (M[i][f])
				dfs(M, f);
		}
	}
public:
	//547. Friend Circles
	int findCircleNum(vector<vector<int>>& M) {// connected components, beat 98%
		visited.resize(M.size());
		int connected = 0;
		for (size_t f = 0; f < M.size(); f++) {
			if (!visited[f]) {
				connected++;
				dfs(M, f);
			}
		}
		return connected;
	}
};

class DFS
{
	int n, m;
	int dfs(vector<vector<int>>& grid, int r, int c) {
		if (r < 0 || c < 0 || r >= n || c >= m)
			return 0;
		if (grid[r][c] == 0)
			return 0;
		int ans = 1;
		grid[r][c] = 0;
		static const vector<vector<int>> dir{ {-1, 0 }, {1,0}, {0,-1}, {0,1} };
		for (const auto& d : dir)
			ans += dfs(grid, r + d[0], c + d[1]);
		return ans;
	}
public:
	int maxAreaOfIsland(vector<vector<int>>& grid) {//beat 85% on 3rd submit
		n = grid.size();
		if (n == 0)
			return 0;
		m = grid[0].size();
		if (m == 0)
			return 0;
		int ans = 0;
		for (int r = 0; r < n; r++) {
			for (int c = 0; c < m; c++) {
				if (grid[r][c])
					ans = max(ans, dfs(grid, r, c));
			}
		}
		return ans;
	}
};

TEST_CASE("695. Max Area of Island", "[NEW]")
{
	CHECK(DFS().maxAreaOfIsland(vector<vector<int>>{ {1, 1, 0, 0, 0}, { 1, 1, 0, 0, 0 }, { 0, 0, 0, 1, 1 }, { 0, 0, 0, 1, 1 }})==4);
}
