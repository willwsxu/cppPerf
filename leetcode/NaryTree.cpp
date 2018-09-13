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

	void preorderHelper(Node* root, vector<int>&  preTrav) {
		if (!root)
			return;
		preTrav.push_back(root->val);
		for (Node *n : root->children)
			preorderHelper(n, preTrav);
	}

	// 589. N-ary Tree Preorder Traversal
	vector<int> preorder(Node* root) { // beat 23%
		vector<int>  preTrav;
		//preorderHelper(root, preTrav);
		if (!root)
			return preTrav;
		deque<Node*> nodes{ root };  // use deque instead of stack for easy algorithm
		while (!nodes.empty()) {
			root = nodes.back();
			preTrav.push_back(root->val);
			nodes.pop_back();
			copy(rbegin(root->children), rend(root->children), back_inserter(nodes));
		}
		return preTrav;
	}
	// N-ary Tree Postorder Traversal
	vector<int> postorder(Node* root) { // harder than preorder, beat 33%
		vector<int>  postTrav;
		if (!root)
			return postTrav;
		deque<pair<Node*, bool>> nodes{ {root,false} };  // need to mark if a node's children is visited
		while (!nodes.empty()) {
			auto& p = nodes.back();
			if (p.second) {  // already visited
				postTrav.push_back(p.first->val);
				nodes.pop_back();
			}
			else {
				transform(rbegin(p.first->children), rend(p.first->children), back_inserter(nodes), [](Node*n) {return make_pair( n,false ); });
				p.second = true;// mark as visited
			}
		}
		return postTrav;
	}
};
