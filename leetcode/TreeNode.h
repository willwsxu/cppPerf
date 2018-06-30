#pragma once

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
};
