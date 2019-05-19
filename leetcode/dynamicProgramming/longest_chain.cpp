#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int dfs(const vector < vector<int>>& adj_list, vector<int>& chains, int u) {
	if (chains[u] >= 0)
		return chains[u];
	int max_len = 0;
	for (int v : adj_list[u]) {
		max_len = max(max_len, 1 + dfs(adj_list, chains, v));
	}
	chains[u] = max_len;
	return max_len;
}

int longestStrChain(vector<string>& words) {
	vector < vector<int>> adj_list(words.size(), vector<int>());
	for (int i = 0; i < words.size() - 1; i++) {
		for (int j = i + 1; j < words.size(); j++) {
			int diff = static_cast<int>(words[i].size() - words[j].size());
			if (abs(diff) != 1)
				continue;
			string & a = diff < 0 ? words[i] : words[j];
			string & b = diff < 0 ? words[j] : words[i];
			vector<int> count(26, 0);
			for (char c : b)
				count[c - 'a']++;
			for (char c : a)
				count[c - 'a']--;
			bool good = all_of(begin(count), end(count), [](int c) { return c >= 0; });
			if (good) {
				if (diff < 0)
					adj_list[i].push_back(j);
				else
					adj_list[j].push_back(i);
			}
		}
	}
	vector<int> chains(words.size(), -1);
	for (int i = 0; i < words.size(); i++) {
		dfs(adj_list, chains, i);
	}
	return *max_element(begin(chains), end(chains))+1;
}

#include "catch.hpp"
TEST_CASE("1048. Longest String Chain", "[DYN]")
{
	CHECK(longestStrChain(vector<string>{"a", "b", "ba", "bca", "bda", "bdca"}) == 4);
}