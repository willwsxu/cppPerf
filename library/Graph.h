#pragma once
#include <vector>
#include <deque>
#include <set>
using std::set;
using std::vector;
using std::deque;

class GraphUnweighted
{
public:
	GraphUnweighted(int N) :adjList(N + 1, vector<int>()) {}

	void undirectEdges(const vector<vector<int>>& edges)
	{
		for (const auto& e : edges) {
			adjList[e[0]].push_back(e[1]);
			adjList[e[1]].push_back(e[0]);
		}
	}
	vector<int> bfs(int s)
	{
		vector<int> weight(adjList.size(), -1);
		deque<int> q{ s };
		weight[s] = 0;
		while (!q.empty()) {
			for (int to : adjList[q.front()]) {
				if (weight[to] == -1) {
					weight[to] = weight[q.front()] + 1;
					q.push_back(to);
				}
			}
			q.pop_front();
		}
		return weight;
	}
private:
	vector<vector<int>>  adjList;
};

class Graph {
	vector<vector<int>> adjList;
public:
	Graph(int n) : adjList(n) {     // 0 based      
	}

	void add_edge(int u, int v) { //0 based
		adjList[u].push_back(v);
		adjList[v].push_back(u);
	}

	vector<int> shortest_reach(int start) { // 0 based
		vector<int> distTo(adjList.size(), -1);
		deque<int> bfs_q{ start };
		distTo[start] = 0;
		int dist = 0;
		while (!bfs_q.empty()) {
			dist += 6;
			int old_size = bfs_q.size();
			while (old_size--) {
				int from = bfs_q.front();
				bfs_q.pop_front();
				for (int to : adjList[from]) {
					if (distTo[to] >= 0)  // seen it before
						continue;
					distTo[to] = dist;
					bfs_q.push_back(to);
				}
			}
		}
		//distTo.erase(remove(begin(distTo), end(distTo), 0), end(distTo));
		return distTo;
	}

};

// Algorithms->Graph Theory->Kingdom Connectivity
// find all path lead from 1 to N
// if there is a cycle in a valid path, path is infinite
class GraphPath {
	vector<vector<int>>  adjList;
	vector<int>  ways;  // ways to reach dest at each node

public:
	GraphPath(int N) : adjList(N), ways(N, -1) {}
	void add_edge_direct(int u, int v) {
		adjList[u].push_back(v);
	}

	bool infinite = false;
	int countPaths(int u, int dest, set<int>& nodes_in_path) {
		//copy(begin(nodes_in_path), end(nodes_in_path), ostream_iterator<int>(cout, " "));
		//cout << " path set\n";
		if (u == dest)
			return 1;
		if (ways[u] >= 0)
			return ways[u];
		if (nodes_in_path.count(u) > 0) // cycle
			return -1;
		nodes_in_path.insert(u);
		bool cycle = false;
		long long count = 0;
		for (int v : adjList[u]) {
			int result = countPaths(v, dest, nodes_in_path);
			if (result < 0)
				cycle = true;
			else
				count += result;
		}
		nodes_in_path.erase(u);
		if (cycle && count>0)
			infinite = true;
		ways[u] = static_cast<int>(count % 1000000000);
		return  ways[u];
	}
};