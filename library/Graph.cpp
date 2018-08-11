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
		for (int f = 0; f < M.size(); f++) {  // adjacency matrix define directed friendship
			if (M[i][f])
				dfs(M, f);
		}
	}
public:
	//547. Friend Circles
	int findCircleNum(vector<vector<int>>& M) {// connected components, beat 98%
		visited.resize(M.size());
		int connected = 0;
		for (int f = 0; f < M.size(); f++) {
			if (!visited[f]) {
				connected++;
				dfs(M, f);
			}
		}
		return connected;
	}
};
