#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

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
};
// 515. Find Largest Value in Each Tree Row of a binary tree.
// BFS, similar to 102. Binary Tree Level Order Traversal, 199. Binary Tree Right Side View, 103.Binary Tree Zigzag Level Order Traversal,513.Find Bottom Left Tree Value
class Tree {
public:
	vector<int> largestValues(TreeNode* root) {
		vector<int> ans;
		queue<TreeNode*> q;
		if (root == nullptr)
			return ans;
		q.push(root);
		while (!q.empty()) {
			int oldSize = q.size();
			int large = INT_MIN;
			for (int i = 0; i < oldSize; i++) {
				TreeNode *tn = q.front();
				q.pop();
				if (large < tn->val)
					large = tn->val;
				if (tn->left)
					q.push(tn->left);
				if (tn->right)
					q.push(tn->right);
			}
			ans.push_back(large);
		}
		return ans;
	}

	// 230. Kth Smallest Element in a BST, 1 ≤ k ≤ BST's total elements
	int count(TreeNode* root) {
		if (root == nullptr)
			return 0;
		return 1 + count(root->left) + count(root->right);
	}
	int kthSmallest_bs(TreeNode* root, int k) {  //binary search, beat 97%
		int cnt = count(root->left); // left count
		if (cnt + 1 == k)
			return root->val;
		if (cnt >= k)   // target is in left side
			return kthSmallest(root->left, k);
		return kthSmallest(root->right, k - 1 - cnt);  // go to right side, adjust k (minus let count, minus root
	}
	int ans;
	int cnt;
	void inorder(TreeNode* root) {
		if (root == nullptr || cnt == 0)
			return;
		inorder(root->left);
		if (--cnt == 0)
			ans = root->val;
		inorder(root->right);
	}
	int kthSmallest(TreeNode* root, int k) // inorder traversal, same speed, beat 97%
	{
		cnt = k;
		inorder(root);
		return ans;
	}

	// 94. Binary Tree Inorder Traversal
	vector<int> vec;
	void inorderTraversalHelper(TreeNode* root)  // beat 97%
	{
		if (root == nullptr)
			return;
		inorderTraversal(root->left);
		vec.push_back(root->val);
		inorderTraversal(root->right);
	}
	vector<int> inorderTraversal(TreeNode* root) {// iterative solution beat 97%
		stack<TreeNode*> nodes;
		while (root) {
			nodes.push(root);
			root = root->left;  // traverse to left most leaf node
		}
		while (!nodes.empty()) {
			root = nodes.top();
			vec.push_back(root->val);
			nodes.pop();
			root = root->right;  // visit right subtree
			while (root) {
				nodes.push(root);
				root = root->left;  // traverse to left most leaf node
			}
		}
		return vec;
	}

	// 129. Sum Root to Leaf Numbers
	// Given a binary tree containing digits from 0-9 only, each root-to-leaf path could represent a number.

	int sumNumbers(TreeNode* root, int prev) {
		prev = prev * 10 + root->val;  // calculate value up to this node
		if (root->left == nullptr && root->right == nullptr)
			return prev;  //leaf
		int sum = 0;
		if (root->left)
			sum += sumNumbers(root->left, prev);  // sum of left
		if (root->right)
			sum += sumNumbers(root->right, prev); // sum of right
		return sum;
	}
	int sumNumbers(TreeNode* root) {
		if (root == nullptr)
			return 0;
		return sumNumbers(root, 0);
	}
};

void testTree()
{
	TreeNode *tn = new TreeNode(1);
	tn->left = new TreeNode(3);
	tn->right = new TreeNode(2);
	tn->left->left = new TreeNode(5);
	tn->left->right = new TreeNode(3);
	tn->right->right = new TreeNode(9);
	Tree t;
	vector<int> ans = t.largestValues(tn);
	print(ans);
}

TEST_CASE("kth smallest BST Components", "[BST]")
{
	TreeNode::test();
	Tree t;
	TreeNode * r = TreeNode::createBST(vector<int>{1, 2}, 0);
	TreeNode::preorderPrint(r);
	SECTION("edge case") {
		CHECK(t.kthSmallest(r, 2) == 2);
		REQUIRE(t.kthSmallest(r, 1) == 1);
	}
}

//449. Serialize and Deserialize BST
class Codec {  // beat 87%
	vector<int> v_preorder;
	void preorderBST(TreeNode* root)
	{
		if (root) {
			v_preorder.push_back(root->val);
			preorderBST(root->left);
			preorderBST(root->right);
		}
	}
public:

	// Encodes a tree to a single string.
	string serialize(TreeNode* root) {
		v_preorder.clear();
		preorderBST(root);
		std::ostringstream oss;
		copy(begin(v_preorder), end(v_preorder), ostream_iterator<int>(oss, " "));
		return oss.str();
	}

	// Decodes your encoded data to tree.
	TreeNode* deserialize(string data) {
		istringstream iss(data);
		vector<int> vi{ istream_iterator<int>(iss), istream_iterator<int>() };
		return TreeNode::preorder(vi, 0, vi.size() - 1);
	}
};

TEST_CASE("Serialize and Deserialize BST", "[NEW]")
{
	TreeNode * r = TreeNode::createBST(vector<int>{4,3,5,1,2,6}, 0);
	Codec c;
	string ser = c.serialize(r);
	CHECK(c.serialize(c.deserialize(ser))==ser);
}