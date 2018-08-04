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
	vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
		vector<vector<int>> all;
		vector<int> path{};
		allPathsSourceTarget(graph, all, path, 0);
		return all;
	}
};