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
