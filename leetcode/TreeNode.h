#pragma once
#include <cassert>
#include <deque>
#include <iostream>
#include <string>
#include <algorithm>

using std::deque;
using std::cout;

struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int x) : val(x), left(NULL), right(NULL) {}

	static TreeNode *preorder(const vector<int>& nodes, int low, int high)  //O(n^2)
	{
		if (low > high)
			return nullptr;
		TreeNode *r = new TreeNode(nodes[low]);
		int right = high + 1;
		for (right = low; right <= high; right++)
			if (nodes[right] > nodes[low])  // find first number greater than root
				break;
		r->left = preorder(nodes, low + 1, right - 1);
		r->right = preorder(nodes, right, high);
		return r;
	}
	static TreeNode * createBST(const vector<int>& nodes, int order)
	{
		return preorder(nodes, 0, nodes.size() - 1);
	}
	static void preorderPrint(TreeNode *r)
	{
		if (r == nullptr)
			return;
		cout << r->val << " ";
		preorderPrint(r->left);
		preorderPrint(r->right);
	}
	static void test()
	{
		TreeNode *r = createBST(vector<int>{10, 5, 1, 7, 40, 50}, 0);
		preorderPrint(r);
	}
	static int heightL(TreeNode *r) {
		int h = 0;
		while (r) {
			++h;
			r = r->left;
		}
		return h;
	}
	static int heightR(TreeNode *r) {
		int h = 0;
		while (r) {
			++h;
			r = r->right;
		}
		return h;
	}

	static int getHeight(TreeNode* r)
	{
		return r == nullptr ? 0 : std::max(getHeight(r->left), getHeight(r->right)) + 1;
	}
	static TreeNode *ConstructBinaryTreePerLevel(const vector<string>& nodes) { // level traversal
		if (nodes.empty())
			return nullptr;
		TreeNode *root = new TreeNode(stoi(nodes[0]));
		deque<TreeNode*> level{ root }; // nodes from last level
		auto node_val = begin(nodes) + 1;
		auto create_node = [&level](const string& s, TreeNode*& child) {
			if (s != "null") {
				child = new TreeNode(stoi(s));
				level.push_back(child);
			}
		};
		while (!level.empty() && node_val!=end(nodes)) {
			int old_size = level.size();
			while (old_size--  && node_val != end(nodes) ) {
				create_node(*node_val, level.front()->left);		// left child
				if (++node_val != end(nodes)) {
					create_node(*node_val, level.front()->right);	// right child
					++node_val;
				}
				level.pop_front();
			}
		}
		return root;
	}

	// deprecated
	static TreeNode *CreateBinaryTree(const vector<int>& nodes) { // INT32_MIN as null node, level traversal
		if (nodes.empty())
			return nullptr;
		TreeNode *root = new TreeNode(nodes[0]);
		deque<TreeNode*> level{ root }; // nodes from last level
		size_t idx = 1;
		while (!level.empty() && idx<nodes.size()) {
			int old_size = level.size();
			for (int i=0; i<old_size && idx<nodes.size(); i++) {
				TreeNode * r = level.front();
				level.pop_front();
				if (nodes[idx++] != INT32_MIN) {
					r->left = new TreeNode(nodes[idx-1]);
					level.push_back(r->left);
				}
				if (idx < nodes.size()) {  // allow trailing null nodes
					if (nodes[idx++] != INT32_MIN) {
						r->right = new TreeNode(nodes[idx-1]);
						level.push_back(r->right);
					}
				}
			}
		}
		return root;
	}
	// 606. Construct String from Binary Tree
	// construct a string consists of parenthesis and integers from a binary tree with the preorder traversing way
	// remove all unnecessary ()
	static string tree2str(TreeNode* t) {
		if (!t)
			return "";
		if (!t->left && !t->right)
			return std::to_string(t->val);
		string result = std::to_string(t->val);
		result.append(1, '(').append(tree2str(t->left)).append(1, ')');
		if (t->right)
			result.append(1, '(').append(tree2str(t->right)).append(1, ')');
		return result;
	}

	// from all parent node in deque, fill all child node
	static void bfs(deque<TreeNode*>& dq, size_t oldSize)
	{
		for (size_t i = 0; i < oldSize; i++) {
			TreeNode * r = dq.front();
			if (r->left)
				dq.push_back(r->left);
			if (r->right)
				dq.push_back(r->right);
			dq.pop_front();
		}
	}
	// 102. Binary Tree Level Order Traversal
	static vector<vector<int>> levelOrder(TreeNode* root) {
		vector<vector<int>> ans;
		if (root == nullptr)
			return ans;
		deque<TreeNode*> dq{ root };
		while (!dq.empty()) {
			ans.push_back({});
			transform(begin(dq), end(dq), back_inserter(ans.back()), [](const TreeNode* n) { return n->val; });

			bfs(dq, dq.size());
		}
		return ans;
	}
};

// 108. Convert Sorted Array to Binary Search Tree (beat100% in 4th submission)
template<typename RandIter>
TreeNode * buildBSTinorder(RandIter first, RandIter last)  // input: sorted vector of int
{
	if (first == last)
		return nullptr;
	int mid = distance(first, last) / 2;
	TreeNode *r = new TreeNode(*(first + mid));
	r->left = buildBSTinorder(first, first + mid);
	r->right = buildBSTinorder(first + mid + 1, last);
	return r;
}