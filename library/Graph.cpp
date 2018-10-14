#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
#include "UnionFind.h"
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

	// 463. Island Perimeter
	int islandPerimeter(vector<vector<int>>& grid) { // no need to use hash table. simple 2D board issue, beat 62%
		if (grid.empty())
			return 0;
		int m = grid.size();
		if (grid[0].empty())
			return 0;
		int n = grid[0].size();
		int ans = 0;
		for (int r = 0; r < m; r++) {
			for (int c = 0; c < n; c++) {
				if (grid[r][c]) {
					ans += 4;
					if (r > 0 && grid[r - 1][c])
						ans--;
					if (r <m - 1 && grid[r + 1][c])
						ans--;
					if (c > 0 && grid[r][c - 1])
						ans--;
					if (c < n - 1 && grid[r][c + 1])
						ans--;
				}
			}
		}
		return ans;
	}
	void dfs(vector<vector<int>>& image, int sr, int sc, int oldColor, int newColor) {
		if (sr >= 0 && sc >= 0 && sr < (int)image.size() && sc < (int)image[0].size()) {
			if (image[sr][sc] != oldColor)
				return;
			image[sr][sc] = newColor;
			static const vector<vector<int>> dir{ { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 } };
			for (const auto& d : dir) {
				dfs(image, sr + d[0], sc + d[1], oldColor, newColor);
			}
		}
	}
	// 733. Flood Fill, replace cells connected to (sr,sc) in 4 directions (up, down, left, right) of same values to new value
	vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {  // beat 98% on 3rd submit
		if (image[sr][sc] == newColor)
			return image;
		dfs(image, sr, sc, image[sr][sc], newColor);
		return image;
	}
};

TEST_CASE("695. Max Area of Island", "[NEW]")
{
	CHECK(DFS().maxAreaOfIsland(vector<vector<int>>{ {1, 1, 0, 0, 0}, { 1, 1, 0, 0, 0 }, { 0, 0, 0, 1, 1 }, { 0, 0, 0, 1, 1 }})==4);
}


class Employee {
public:
	// It's the unique ID of each node.
	// unique id of this employee
	int id;
	// the importance value of this employee
	int importance;
	// the id of direct subordinates
	vector<int> subordinates;
};

class DfsMap{  // Dfs on map
	int dfsHelper(const map<int, Employee*>& employees, int id) {
		return accumulate(begin(employees.at(id)->subordinates), end(employees.at(id)->subordinates), employees.at(id)->importance, [&](int init, int id2) {
			return init + dfsHelper(employees, id2); });
	}
public:
	int getImportance(vector<Employee*> employees, int id) { // beat 66%, no need to keep if visited
		map<int, Employee*> emp_map;
		transform(begin(employees), end(employees), inserter(emp_map, emp_map.begin()), [](Employee*e)->pair<int, Employee*> { return { e->id, e }; });
		return dfsHelper(emp_map, id);
	}
};

TEST_CASE("690. Employee Importance", "[NEW]")
{
	CHECK(DfsMap().getImportance({ new Employee{ 2,5,{} } }, 2) == 5);
	CHECK(DfsMap().getImportance({ new Employee{1,2,{2}}, new Employee{2,3,{}} }, 2) == 3);
	CHECK(DfsMap().getImportance({ new Employee{ 1,5,{ 2,3 } }, new Employee{ 2,3,{} }, new Employee{ 3,3,{} } }, 1) == 11);
}

struct UndirectedGraphNode {
	int label;
	vector<UndirectedGraphNode *> neighbors;
	UndirectedGraphNode(int x) : label(x) {};
};
class Graph {
public:
	// 684. Redundant Connection, undirected graph
	vector<int> findRedundantConnection(vector<vector<int>>& edges) {  // beat 34% with slow UnionFind
		UnionFind uf(edges.size());
		for (const auto& e : edges) {
			if (!uf.union_(e[0], e[1]))
				return e;
		}
		return{};  // should not reach
	}
	// 721. Accounts Merge, merge account if they share same email. output emails in sorted order
	vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) { // beat 70%
		UnionFind uf(accounts.size());
		unordered_map<string, int> email_index;
		for (size_t i = 0; i < accounts.size(); i++) {
			for (auto email = begin(accounts[i]) + 1; email != end(accounts[i]); ++email) {
				auto mergeAccount = email_index.find(*email);
				if (mergeAccount != email_index.end()) {
					uf.union_(i, mergeAccount->second);
				}
				else
					email_index[*email] = i;
			}
		}

		for (size_t i = 0; i < accounts.size(); i++) {
			auto parent = uf.find(i);
			if (parent == i)
				continue;
			move(begin(accounts[i]) + 1, end(accounts[i]), back_inserter(accounts[parent]));
			accounts[i].clear();
		}
		auto empty = remove_if(begin(accounts), end(accounts), [](const auto& v) { return v.empty(); });
		accounts.erase(empty, end(accounts));
		for (auto& a : accounts) {
			sort(begin(a) + 1, end(a));
			a.erase(unique(begin(a) + 1, end(a)), end(a));  // original account could have duplicate 
		}
		return accounts;
	}

	// 133. Clone Graph
	UndirectedGraphNode *cloneGraph(UndirectedGraphNode *node, map<int, UndirectedGraphNode*>& visited) {
		if (!node)
			return nullptr;
		if (visited.count(node->label))  // prevent infinite loop on circular
			return visited[node->label];
		UndirectedGraphNode *head = new UndirectedGraphNode(node->label);
		visited[node->label] = head;
		head->neighbors.resize(node->neighbors.size());
		for (size_t i = 0; i < node->neighbors.size(); i++)
			head->neighbors[i] = cloneGraph(node->neighbors[i], visited);
		return head;
	}
	UndirectedGraphNode *cloneGraph(UndirectedGraphNode *node)  // DFS, beat 100%, watch out for circular
	{
		map<int, UndirectedGraphNode*> visited;
		return cloneGraph(node, visited);
	}
	// 127. Word Ladder, change 1 letter to different word in dictionary, until it is end word
	int ladderLength(string beginWord, string& endWord, set<string>& dict)
	{
		if (beginWord == endWord)
			return 1;
		int ans = INT32_MAX / 2;
		for (size_t i = 0; i < beginWord.size(); i++) {
			for (char letter = 'a'; letter <= 'z'; letter++) {
				char old = beginWord[i];
				beginWord[i] = letter;
				if (dict.count(beginWord)) {
					dict.erase(beginWord);
					ans = min(ans, 1 + ladderLength(beginWord, endWord, dict));
					dict.insert(beginWord);
				}
				beginWord[i] = old;
			}
		}
		return ans;
	}
	int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
		set<string> dict(begin(wordList), end(wordList));
		int ans = ladderLength(beginWord, endWord, dict);
		return ans >= INT32_MAX / 2 ? 0 : ans;
	}
};
TEST_CASE("127. Word Ladder", "[NEW]")
{
	vector<string> long_dict{ "si", "go", "se", "cm", "so", "ph", "mt", "db", "mb", "sb", "kr", "ln", "tm", "le", "av", "sm", "ar", "ci", "ca", "br", "ti", "ba", "to", "ra", "fa", "yo", "ow", "sn", "ya", "cr", "po", "fe", "ho", "ma", "re", "or", "rn", "au", "ur", "rh", "sr", "tc", "lt", "lo", "as", "fr", "nb", "yb", "if", "pb", "ge", "th", "pm", "rb", "sh", "co", "ga", "li", "ha", "hz", "no", "bi", "di", "hi", "qa", "pi", "os", "uh", "wm", "an", "me", "mo", "na", "la", "st", "er", "sc", "ne", "mn", "mi", "am", "ex", "pt", "io", "be", "fm", "ta", "tb", "ni", "mr", "pa", "he", "lr", "sq", "ye" };
	//CHECK(Graph().ladderLength("qa", "sq", long_dict) == 0);  TLE
	CHECK(Graph().ladderLength("hit", "cog", vector<string>{"hot", "dot", "dog", "lot", "log"}) == 0);
	CHECK(Graph().ladderLength("hit", "cog", vector<string>{"hot", "dot", "dog", "lot", "log", "cog"}) == 5);
}

TEST_CASE("133. Clone Graph", "[NEW]")
{
	UndirectedGraphNode *r2 = new UndirectedGraphNode(10);
	r2->neighbors.push_back(r2);  //circular
	CHECK(Graph().cloneGraph(r2)->label == 10);
	UndirectedGraphNode *root = new UndirectedGraphNode(1);
	root->neighbors.push_back(new UndirectedGraphNode(2));
	root->neighbors.push_back(new UndirectedGraphNode(3));
	root->neighbors[0]->neighbors.push_back(new UndirectedGraphNode(4));
	UndirectedGraphNode * clone = Graph().cloneGraph(root);
	CHECK(clone->neighbors[0]->neighbors[0]->label == 4);
	CHECK(Graph().cloneGraph(nullptr) == nullptr);
}
TEST_CASE("721. Accounts Merge", "[NEW]")
{
	CHECK(Graph().accountsMerge(vector<vector<string>>{ {"David", "David0@m.co", "David1@m.co"}, { "David","David3@m.co","David4@m.co" }, { "David","David4@m.co","David5@m.co" }, { "David","David2@m.co","David3@m.co" }, { "David","David1@m.co","David2@m.co" }})
		== vector<vector<string>>{ {"David", "David0@m.co", "David1@m.co", "David2@m.co", "David3@m.co", "David4@m.co", "David5@m.co"}});
	CHECK(Graph().accountsMerge(vector<vector<string>>{ {"Alex", "Alex5@m.co", "Alex4@m.co", "Alex0@m.co"}, { "Ethan","Ethan3@m.co","Ethan3@m.co","Ethan0@m.co" }, { "Kevin","Kevin4@m.co","Kevin2@m.co","Kevin2@m.co" }, { "Gabe","Gabe0@m.co","Gabe3@m.co","Gabe2@m.co" }, { "Gabe","Gabe3@m.co","Gabe4@m.co","Gabe2@m.co" }})
		== vector<vector<string>>{ {"Alex", "Alex0@m.co", "Alex4@m.co", "Alex5@m.co"}, { "Ethan","Ethan0@m.co","Ethan3@m.co" }, { "Kevin","Kevin2@m.co","Kevin4@m.co" }, { "Gabe","Gabe0@m.co","Gabe2@m.co","Gabe3@m.co","Gabe4@m.co" }});
	CHECK(Graph().accountsMerge(vector<vector<string>>{ {"John", "johnsmith@mail.com", "john_newyork@mail.com"}, { "John","johnsmith@mail.com","john00@mail.com" }, { "Mary","mary@mail.com" }, { "John","johnnybravo@mail.com" }})
		== vector<vector<string>>{ {"John", "john00@mail.com", "john_newyork@mail.com", "johnsmith@mail.com"}, { "Mary","mary@mail.com" }, { "John","johnnybravo@mail.com" }});
	CHECK(Graph().accountsMerge(vector<vector<string>>{}) == vector<vector<string>>{});
}

class BFS
{
public:
	// 130. Surrounded Regions    
	// Given a 2D board containing 'X' and 'O' (the letter O), capture all regions surrounded by 'X'
	// observation:  Any 'O' that is not on the border and it is not connected to an 'O' on the border will be flipped to 'X
	void solve(vector<vector<char>>& board) {  // BFS, beat 99% on 3rd submit
		if (board.empty() || board[0].empty())
			return;
		int rows = board.size();
		int cols = board[0].size();
		if (rows < 3 || cols < 3)
			return;
		deque<pair<int, int>> edge_dells;
		for (int c = 0; c < cols; c++) {  // add O cell from 4 edges,
			if (board[0][c] == 'O')
				edge_dells.emplace_back(0, c);
			if (board[rows - 1][c] == 'O')
				edge_dells.emplace_back(rows - 1, c);
		}
		for (int r = 1; r < rows - 1; r++) {  // don't add corner cell twice
			if (board[r][0] == 'O')
				edge_dells.emplace_back(r, 0);
			if (board[r][cols - 1] == 'O')
				edge_dells.emplace_back(r, cols - 1);
		}
		const static vector<pair<int, int>> dir{ { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 } };
		while (!edge_dells.empty()) {
			for (const auto& cell : edge_dells) // mark as don't flip
				board[cell.first][cell.second] = 'N';
			size_t old_size = edge_dells.size();
			for (size_t i = 0; i < old_size; i++) {
				for (const auto& d : dir) {
					const int r = edge_dells[i].first + d.first;
					const int c = edge_dells[i].second + d.second;
					if (r >= 0 && c >= 0 && r<rows && c<cols && board[r][c] == 'O')  // add O cells connected to edge
						edge_dells.emplace_back(r, c);
				}
			}
			edge_dells.erase(begin(edge_dells), begin(edge_dells) + old_size);
		}
		for (auto& row : board) {
			for (auto& cell : row) {
				if (cell == 'O')
					cell = 'X';
				else if (cell == 'N')
					cell = 'O';
			}
		}
	}

};