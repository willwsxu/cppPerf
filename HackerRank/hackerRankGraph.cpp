#include "..\catch.hpp"
//#include "eraseRemove.h"
#include <vector>
#include <set>
#include <deque>
#include <iterator>
#include <iostream>
using namespace std;

template<typename VEC>
void vector_erase(VEC& v, typename VEC::value_type& target)
{
	v.erase(find(begin(v), end(v), target));
}

class Graph {
	vector<vector<int>> adjList;
public:
	Graph(int N) :adjList(N + 1, vector<int>()) {}
	bool add_undirect_edge(int from, int to) {
		if (from >= (int)adjList.size() || to >= (int)adjList.size())
			return false;
		adjList[from].push_back(to);  // undirected
		adjList[to].push_back(from);
		return true;
	}
	int build_even_forests()
	{
		vector<set<int>> forests; // head and its forest
		deque<int> leaf_queue; // bfs queue of leafs
		for (int node = 1; node < (int)adjList.size(); node++) {
			if (adjList[node].size() == 1) {
				leaf_queue.push_back(node);
				forests.push_back({ node });
			}
		}
		int removed_edges = 0;
		while (!leaf_queue.empty()) {
			int old_size = leaf_queue.size();
			//cout << " bfs queue " << old_size << "\n";
			while (old_size--) {
				int left_node = leaf_queue.front();
				leaf_queue.pop_front();
				if (adjList[left_node].empty())  // node used by its neighbor
					continue;
				int right_node = 0;
				//cout << "forests " << forests.size() <<" left node " << left_node  << "\n";
				vector<set<int> *> removed;
				for (auto& s : forests) {
					if (!s.count(left_node))
						continue;
					right_node = adjList[left_node][0];
					adjList[left_node].clear();  // clear out used edge
					vector_erase(adjList[right_node], left_node);
					s.insert(right_node);
					for (auto &s2 : forests) { // if two sets sharing a node, merge
						if (&s != &s2 && s2.count(right_node)) {
							s.insert(begin(s2), end(s2));
							removed.push_back(&s2);
						}
					}
					break;
				}
				for (auto x : removed) // remove merged forest
					vector_erase(forests, *x);
				if (!adjList[left_node].empty()) { // edge from this node is not used
					removed_edges++;
					right_node = adjList[left_node][0];
					//cout << " edge not used:" << left_node << " " << right_node << "\n";
					vector_erase(adjList[right_node], left_node);
					adjList[left_node].clear();
					auto found = find_if(begin(forests), end(forests), [right_node](const auto& s) { return s.count(right_node) > 0; });
					if ( found== end(forests))  // edge case
						forests.push_back({ right_node });
				}
				if (adjList[right_node].size() == 1)
					leaf_queue.push_back(right_node);
			}
			// check if there is even forests after each round of leaf
			/*for (auto& s : forests) {
				copy(begin(s), end(s), ostream_iterator<int>(cout, " "));
				cout << "\n";
			}
			cout << "forests before removing even\n";*/
			auto evenForest = remove_if(begin(forests), end(forests), [this](const set<int> &s) {
				if  (s.size() % 2 != 0)
					return false;
				for (const auto &node : s) {
					if (adjList[node].size()>1)
					return false;
				}
				return true;
			});
			//cout << "even forests removed " << distance(evenForest, end(forests)) << "\n";
			forests.erase(evenForest, end(forests));
			/*for (auto& s: forests) {
				copy(begin(s), end(s), ostream_iterator<int>(cout, " "));
				cout<<"\n";
			}
			copy(begin(leaf_queue), end(leaf_queue), ostream_iterator<int>(cout, " "));
			cout << "forests size " << forests.size() << " leaf queue size " << leaf_queue.size()  << "\n";*/
		}
		return removed_edges;
	}
};
TEST_CASE("Hacker rank DP graph: even tree", "[NEW]")
{
	SECTION("test medium merge edge case") {
		Graph g(12);
		vector<pair<int, int>> edges{ { 1,2 },{ 1,6 },{ 1,3 },{ 4,6 },{ 3,5 },{ 6,7 },{ 6,8 },{ 1,9 },{ 9,10 },{ 10,11 },{ 11,12 } };
		for (const auto& e : edges)
			g.add_undirect_edge(e.first, e.second);
		REQUIRE(g.build_even_forests() == 4);
	}
	SECTION("test large") {
		Graph g(50);
		vector<pair<int, int>> edges{ { 2,1 },{ 3,1 },{ 4,2 },{ 5,2 },{ 6,4 },{ 7,6 },{ 8,5 },{ 9,1 },{ 10,9 },{ 11,4 },{ 12,6 },{ 13,12 },{ 14,1 },{ 15,5 },{ 16,2 },{ 17,8 },{ 18,7 },{ 19,3 },{ 20,18 },{ 21,3 },{ 22,9 },{ 23,6 },{ 24,18 },{ 25,13 },{ 26,11 },{ 27,18 },{ 28,27 },{ 29,13 },{ 30,25 },{ 31,30 },{ 32,24 },{ 33,12 },{ 34,11 },{ 35,12 },{ 36,3 },{ 37,31 },{ 38,21 },{ 39,27 },{ 40,12 },{ 41,22 },{ 42,14 },{ 43,42 },{ 44,33 },{ 45,1 },{ 46,24 },{ 47,22 },{ 48,30 },{ 49,22 },{ 50,43 } };
		for (const auto& e : edges)
			g.add_undirect_edge(e.first, e.second);
		REQUIRE(g.build_even_forests() == 12);
	}
	SECTION("test medium merge") {
		Graph g(12);
		vector<pair<int, int>> edges{ { 1,2 },{ 1,6 },{ 1,3 },{ 4,3 },{ 3,5 },{ 6,7 },{ 6,8 },{ 1,9 },{ 9,10 },{ 10,11 },{ 11,12 } };
		for (const auto& e : edges)
			g.add_undirect_edge(e.first, e.second);
		REQUIRE(g.build_even_forests() == 2);
	}
	SECTION("test simple merge") {
		Graph g(10);
		vector<pair<int, int>> edges{ {1,2},{1,6},{1,4},{2,3},{4,5},{1,7},{7,8},{7,9},{7,10} };
		for (const auto& e : edges)
			g.add_undirect_edge(e.first, e.second);
		REQUIRE(g.build_even_forests() == 3);
	}
}