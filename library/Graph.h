#pragma once
#include <vector>
#include <deque>
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