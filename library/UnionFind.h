#pragma once
#include <vector>
#include <utility>
#include <algorithm>

class UnionFind
{
	std::vector<int> parents;
	std::vector<int> size;

public:
	UnionFind(int N) : parents(N + 1, -1), size(N + 1, 1) {}
	int find(int u) {
		if (parents[u] < 0)
			return u;
		return find(parents[u]);
	}
	bool union_(int u, int v)
	{
		int pu = find(u);
		int pv = find(v);
		if (pu == pv)  // find cycle
			return false;
		if (size[pu] > size[pv])
			std::swap(pu, pv);
		parents[pu] = pv;   // extend longer as it is currently shorter
		size[pu] += size[pv];
		return true;
	}
};
