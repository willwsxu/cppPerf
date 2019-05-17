#include <vector>

using namespace std;
// 1042 Given N gardens, 4 types of flowers, there exist a path between some gardens
// no garden has more than 3 paths in or out
// find a way to plan gardens. It is guaranteed an answer exists.
void dfs(vector<vector<int>>& adj_list, vector<int>& garden, int u) {
	if (garden[u] > 0)  // visited already
		return;
	vector<bool> types_avail(5,true);  // index 0 is reserved for no flower
	for (const auto& to : adj_list[u]) {
		types_avail[garden[to]] = false; // check flower type of all its neighbors with path leading
	}
	for (auto t = 1; t < types_avail.size(); t++) {
		if (types_avail[t]) {
			garden[u] = t;  // choose the first available flower type
			break;
		}
	}
	for (const auto& to : adj_list[u])
		dfs(adj_list, garden, to);
}
vector<int> gardenNoAdj(int N, vector<vector<int>>& paths) {
	vector<int> garden(N, 0); // valid flower type is [1,4], 0 means not visited
	vector<vector<int>> adj_list(N, vector<int>());
	for (const auto& p : paths) {
		int u = p[0] - 1;  // convert garden # to 0 based
		int v = p[1] - 1;
		adj_list[u].push_back(v);
		adj_list[v].push_back(u);
	}
	for (int i = 0; i < N; i++) {
		if (garden[i] == 0)
			dfs(adj_list, garden, i);
	}
	return garden;
}

#include "catch.hpp"
TEST_CASE("1042 flower garden", "[DFS]")
{
	CHECK(gardenNoAdj(4, vector<vector<int>>{ {1, 2}, { 3,4 }}) == vector<int>{1,2,1,2});
	CHECK(gardenNoAdj(4, vector<vector<int>>{ {1, 2}, { 2,3 }, { 3, 4 }, { 4,1 }, { 1,3 }, { 2,4 }}) == vector<int>{1, 2, 3, 4});
}