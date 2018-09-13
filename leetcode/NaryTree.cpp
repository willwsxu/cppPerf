#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

class Node {
public:
	int val;
	vector<Node*> children;

	Node() {}

	Node(int _val, vector<Node*> _children) {
		val = _val;
		children = _children;
	}
};
class NaryTree {
public:
	// 559. Maximum Depth of N-ary Tree
	int maxDepth(Node* root) { // beat 10-40%
		if (!root)
			return 0;
		int depth = 0;
		for (Node * n : root->children)
			depth = max(depth, maxDepth(n));
		return 1 + depth;
	}
	// 429. N-ary Tree Level Order Traversal
	vector<vector<int>> levelOrder(Node* root) {
		vector<vector<int>> ans;
		if (root == nullptr)
			return ans;
		deque<Node*> dq{ root };
		while (!dq.empty()) {
			ans.push_back({});
			transform(begin(dq), end(dq), back_inserter(ans.back()), [](const Node* n) { return n->val; });

			size_t oldSize = dq.size();
			for (size_t i = 0; i < oldSize; i++) {
				for (Node * n : dq.front()->children)
					dq.push_back(n);
				dq.pop_front();
			}
		}
		return ans;
	}
};
