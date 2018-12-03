#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
#include "UnionFind.h"
using namespace std;

class DirectGraph {
	vector<vector<vector<int>>>	adjList;
	vector<int>			distTo;  // dist from dest back to src
	int dfsMin(int s, int dest, int level, int K) {
		if (level > K)
			return INT_MAX / 2;
		if (s == dest)
			return 0;
		if (level == K) {
			return INT_MAX / 2;
		}
		//if (distTo[s] < INT_MAX / 2)  // dfs works but too slow because of these two lines are not working
		//	return distTo[s];
		int ans = INT_MAX / 2;
		for (auto item : adjList[s]) {
			ans = std::min(ans, item[2] + dfsMin(item[1], dest, level + 1, K));
		}
		distTo[s] = ans;
		return ans;
	}

public:
	int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int K) {
		adjList.resize(n);
		for (auto item : flights) {
			adjList[item[0]].push_back(item);
		}
		distTo.resize(n, INT_MAX / 2);
		//distTo[dst] = 0;
		//int ans= dfsMin(src, dst, 0, K+1);  // fails some cases
		//return ans >= INT_MAX / 2 ? -1 : ans;

		// modified bfs, or bellman-ford, loop K+1 times
		distTo[src] = 0;  // distance from src to current node
		unordered_set<int> q{ src };
		for (int i = 0; i <= K && !q.empty(); i++) {
			vector<int> oldAns(distTo);   // make a copy of last values, or it won't work
			vector<int> last_q(begin(q), end(q));
			q.clear();
			for (int s : last_q) {
				for (auto f : adjList[s]) {
					distTo[f[1]] = std::min(distTo[f[1]], oldAns[s] + f[2]);
					if (f[1] != dst)
						q.insert(f[1]);
				}
			}
			//print(distTo);
		}
		return distTo[dst] >= INT_MAX / 2 ? -1 : distTo[dst];
	}
};

void testCheapestTicket()
{
	DirectGraph t[7];
	int ans = 0;

	ans = t[6].findCheapestPrice(5, vector < vector<int>>{ {0, 1, 5}, { 1,2,5 }, { 0,3,2 }, { 3,1,2 }, { 1,4,1 }, { 4,2,1 }}, 0, 2, 2); // 7
	cout << ans << endl;

	ans = t[5].findCheapestPrice(4, vector < vector<int>>{ {0, 1, 1}, { 0,2,5 }, { 1,2,1 }, { 2,3,1 }}, 0, 3, 1); // 6
	cout << ans << endl;

	ans = t[0].findCheapestPrice(3, vector < vector<int>>{ {0, 1, 100}, { 1,2,100 }, { 0,2,500 }}, 0, 2, 0);  // 500
	cout << ans << endl;

	ans = t[1].findCheapestPrice(3, vector < vector<int>>{ {0, 1, 100}, { 1,2,100 }, { 0,2,500 }}, 0, 2, 1); // 200
	cout << ans << endl;

	ans = t[2].findCheapestPrice(3, vector < vector<int>>{ {0, 1, 100}, { 1,2,100 }, { 0,2,500 }}, 1, 0, 1); // -1
	cout << ans << endl;

	ans = t[3].findCheapestPrice(4, vector < vector<int>>{ {0, 1, 100}, { 0,2,500 }}, 0, 3, 0); // -1
	cout << ans << endl;

	vector < vector<int>> flights{ { 3,4,4 },{ 2,5,6 },{ 4,7,10 },{ 9,6,5 },{ 7,4,4 },{ 6,2,10 },{ 6,8,6 },{ 7,9,4 },{ 1,5,4 },{ 1,0,4 },{ 9,7,3 },{ 7,0,5 },{ 6,5,8 },{ 1,7,6 },{ 4,0,9 },{ 5,9,1 },{ 8,7,3 },{ 1,2,6 },{ 4,1,5 },{ 5,2,4 },{ 1,9,1 },{ 7,8,10 },{ 0,4,2 },{ 7,2,8 } };
	ans = t[4].findCheapestPrice(10, flights, 6, 0, 7); // 14
	cout << ans << endl;
}

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

	// 947. Most Stones Removed with Same Row or Column
	// remove all stones in a component except last one
	// transform problem into: find total components
	int removeStones(vector<vector<int>>& stones) {
		int N = stones.size();
		UnionFind uf(stones.size());
		for (int i = 0; i < N - 1; i++) {
			for (int j = i + 1; j < N; j++) {
				if (stones[i][0] == stones[j][0] || stones[i][1] == stones[j][1])
					uf.union_(i, j);
			}
		}
		return N-(uf.get_components() - 1);
	}
	// 952. Largest Component Size by Common Factor, A[i]<=100000, A[i] is unique
	int largestComponentSize(vector<int>& A) {
		UnionFind uf(A.size() - 1);
		auto primes = make_primes(sqrt(100000)+1);
		unordered_map<int, int> factor_index;
		auto try_factor = [&factor_index, &uf](int p, int v) {
			auto found = factor_index.find(p);
			if (found == end(factor_index))  // first time, add factor to map
				factor_index[p] = v;
			else {
				uf.union_(v, found->second);
			}
		};
		for (int i = 0; i < A.size(); i++) {
			int copy = A[i];
			for (int p : primes) {
				if (p*p > copy)
					break;
				if (copy%p == 0) {
					while (copy%p == 0)
						copy /= p;
					try_factor(p, i);
				}
			}
			if (copy>1)
				try_factor(copy, i);
		}
		return uf.max_component_size();
	}
};
TEST_CASE("952. Largest Component Size by Common Factor", "[NEW]")
{
	CHECK(Graph().largestComponentSize(vector<int>{20, 50, 9, 63}) == 2);
	CHECK(Graph().largestComponentSize(vector<int>{4, 6, 15, 35}) == 4);
	CHECK(Graph().largestComponentSize(vector<int>{2, 3, 6, 7, 4, 12, 21, 39}) == 8);
}
TEST_CASE("947. Most Stones Removed with Same Row or Column", "[NEW]")
{
	CHECK(Graph().removeStones(vector<vector<int>>{ {0, 0}, { 0,1 }, { 1,0 }, { 1,2 }, { 2,1 }, { 2,2 }}) == 5);
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

	// 127. Word Ladder, change 1 letter to different word in dictionary, until it is end word
	int ladderLength(string beginWord, string endWord, vector<string>& wordList) {  // recursion is too slow, TLE, beat 76%
		if (beginWord == endWord)
			return 1;
		unordered_set<string> dict(begin(wordList), end(wordList));
		deque<string> transform_from{ beginWord };
		int len = 1;
		while (!transform_from.empty()) {
			int old_size = transform_from.size();
			for (int i = 0; i < old_size; i++) {
				string& word_to_change = transform_from.front();
				for (size_t i = 0; i < word_to_change.size(); i++) {
					for (char letter = 'a'; letter <= 'z'; letter++) {
						char old = word_to_change[i];
						word_to_change[i] = letter;
						if (dict.count(word_to_change)) { // valid transformed word
							if (word_to_change == endWord)
								return len + 1;
							dict.erase(word_to_change);  // each word will be transformed just once
							transform_from.push_back(word_to_change);
						}
						word_to_change[i] = old;
					}
				}
				transform_from.pop_front();
			}
			len++; // complete one round of transform
		}
		return 0;
	}
	// 909. Snakes and Ladders, a board is numbered 1 to N*N, from bottom left cell, 
	// from left to right, up a row, from right to left
	// at each move, cell number A can move to A+1, ..., A+6
	// If it landed a cell which is a snake ladder, it will jump to the cell as indicated by its value
	// find min moves to reach N*N cell
	int snakesAndLadders(vector<vector<int>>& board) {  // beat 46%
		int n = board.size();
		int NN = n*n;
		map<int, int> square_moves;  // number of moves at each square, 
		deque<int> squares{ 1 };
		square_moves[1] = 0;
		while (!squares.empty()) {
			for (int dice = 1; dice <= 6; dice++) {
				int next = dice + squares.front();
				int r = (next - 1) / n;
				int c = (next - 1) % n;
				if (r % 2 != 0)  // odd row
					c = n - c - 1;
				r = n - 1 - r; // row count from top down
				if (board[r][c] > 0)
					next = board[r][c];
				if (square_moves.count(next))  // square is reached brefore, don't make a move here
					continue;
				square_moves[next] = square_moves[squares.front()] + 1;
				if (next == NN)
					return square_moves[next];
				squares.push_back(next);
			}
			squares.pop_front();  // no need to pop, instead to get next
		}
		return -1;
	}

	// 934. Shortest Bridge, between 2 connected components, 4 directions
	int shortestBridge(vector<vector<int>>& A) {
		vector<vector<int>> dir{ { 0,-1 },{ -1,0 },{ 0,1 },{ 1,0 } };
		auto components=connectedComponents(A, dir);
		if (components.size() != 2)  // invalid input
			return 0;
		int rows = A.size();
		int cols = A[0].size();
		deque<int> q{ begin(components[0]), end(components[0])};
		int steps = 0;
		while (!q.empty()) {
			size_t old_size = q.size();
			for (size_t i = 0; i < old_size; i++) {
				int r = q.front() / cols;
				int c = q.front() % cols;
				for (const auto& d : dir) {  // check 4 directions
					int r1 = r + d[0];
					int c1 = c + d[1];
					if (r1 >= 0 && r1 < rows &&c1 >= 0 && c1 < cols) {
						int idx = r1*cols + c1;
						if (components[1].count(idx))  // found it in second component
							return steps;
						if (!components[0].count(idx)) {
							q.emplace_back(idx);
							components[0].insert(idx);
						}
					}
				}
				q.pop_front();
			}
			steps++;
		}
		return 0; // not possible
	}
};
TEST_CASE("934. Shortest Bridge", "[NEW]")
{
	vector<vector<int>> matrix{ { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,1,1,1,1,1,0,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0 },{ 0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,1,0,0,0,0,1,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } };
	CHECK(BFS().shortestBridge(matrix) == 1);
	CHECK(BFS().shortestBridge(vector<vector<int>>{ {0, 1}, { 1,0 }})==1);
	CHECK(BFS().shortestBridge(vector<vector<int>>{ {0, 1, 0}, { 0, 0, 0 }, { 0, 0, 1 }}) == 2);
	CHECK(BFS().shortestBridge(vector<vector<int>>{ {1, 1, 1, 1, 1}, { 1,0,0,0,1 }, { 1,0,1,0,1 }, { 1,0,0,0,1 }, { 1,1,1,1,1 }}) == 1);
}
TEST_CASE("127. Word Ladder", "[NEW]")
{
	vector<string> long_dict{ "si", "go", "se", "cm", "so", "ph", "mt", "db", "mb", "sb", "kr", "ln", "tm", "le", "av", "sm", "ar", "ci", "ca", "br", "ti", "ba", "to", "ra", "fa", "yo", "ow", "sn", "ya", "cr", "po", "fe", "ho", "ma", "re", "or", "rn", "au", "ur", "rh", "sr", "tc", "lt", "lo", "as", "fr", "nb", "yb", "if", "pb", "ge", "th", "pm", "rb", "sh", "co", "ga", "li", "ha", "hz", "no", "bi", "di", "hi", "qa", "pi", "os", "uh", "wm", "an", "me", "mo", "na", "la", "st", "er", "sc", "ne", "mn", "mi", "am", "ex", "pt", "io", "be", "fm", "ta", "tb", "ni", "mr", "pa", "he", "lr", "sq", "ye" };
	CHECK(BFS().ladderLength("qa", "sq", long_dict) == 5);
	CHECK(BFS().ladderLength("hit", "cog", vector<string>{"hot", "dot", "dog", "lot", "log"}) == 0);
	CHECK(BFS().ladderLength("hit", "cog", vector<string>{"hot", "dot", "dog", "lot", "log", "cog"}) == 5);
}
