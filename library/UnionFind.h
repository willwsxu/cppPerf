#pragma once
#include <vector>
#include <utility>
#include <algorithm>
#include <set>

class UnionFindSimple
{
	vector<int> parent;
	vector<int> size;
	int union_count = 0;
public:
	UnionFindSimple(int N) :parent(N), size(N, 0) {
		iota(begin(parent), end(parent), 0);
	}
	int find(int u) {
		while (parent[u] != u) {
			u = parent[u];
		}
		return u;
	}
	bool add_edge(int u, int v) {  // union
		int p_u = find(u - 1);
		int p_v = find(v - 1);
		if (p_u != p_v) {
			if (size[p_u] > size[p_v])
				swap(p_v, p_u);
			parent[p_u] = parent[p_v];  // merge small to big
			size[p_v] += size[p_u];
			union_count++;
			return true;
		}
		return false;
	}
	int components() const {
		return parent.size() - union_count;
	}
};

class UnionFind
{
	std::vector<int> parents;
	std::vector<int> size;
	int        nodes_added = 0;
public:
	UnionFind(int N) : parents(N + 1, -1), size(N + 1, 1) {}
	int find(int u) const {
		if (parents[u] < 0) {
			return u;
		}
		return find(parents[u]);
	}
	bool union_(int u, int v)
	{
		update_count(u);
		update_count(v);
		int pu = find(u);
		int pv = find(v);
		if (pu == pv)  // find cycle
			return false;
		if (size[pu] > size[pv])
			std::swap(pu, pv);
		parents[pu] = pv;   // extend longer as it is currently shorter
		size[pv] += size[pu];
		return true;
	}
	int get_size(int u) const
	{
		return size[find(u)] == 1 ? 0 : size[find(u)];
	}
	int get_count() const {
		return nodes_added;
	}
	void update_count(int u) {  // count nodes called from _union
		if (parents[u] >= 0 || size[u] > 1)
			return;
		nodes_added++;
	}
	int get_components() const { // not connected commponents
		set<int> components;
		for (size_t i = 0; i < parents.size(); i++) {
			int p = find(i);
			if (p < 0)
				components.insert(i);
			else
				components.insert(p);
		}
		return components.size();
	}
	int max_component_size() const {
		return *max_element(begin(size), end(size));
	}
	/*
	int get_count(set<int>& bad) const {
		for (int u : bad) {
			if (parents[u] >= 0 || size[u] > 1)
				bad.erase(u);
		}
		return nodes_added + bad.size();
	}*/
};