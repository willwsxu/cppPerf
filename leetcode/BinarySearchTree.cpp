#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;
#include "TreeNode.h"

class BST
{
	int firstMin;
	int firstMax;
	bool isValidBST(TreeNode* root, int minV, int maxV) {
		if (!root)
			return true;
		if (root->val == INT_MIN) {  // watch out for specail case!!
			if (firstMin)
				return false;
			else
				firstMin = true;
			if (root->left)
				return false;
			if (root->right && root->right->val == INT_MIN)
				return false;
			if (root->val < minV)
				return false;
		}
		else if (root->val == INT_MAX) { // watch out for specail case!!
			if (firstMax)
				return false;
			else
				firstMax = true;
			if (root->right)
				return false;
			if (root->left && root->left->val == INT_MAX)
				return false;
			if (root->val > maxV)
				return false;
		}
		else if (root->val >= maxV || root->val <= minV)
			return false;
		if (!isValidBST(root->left, minV, root->val))
			return false;
		if (!isValidBST(root->right, root->val, maxV))
			return false;
		return true;
	}
	bool isValidBST(TreeNode* root, TreeNode *parent)
	{
		if (!root)
			return true; // in order
		if (!isValidBST(root->left, root))
			return false;
		//if (root->right && )
	}
	bool isValidBST(TreeNode* root, int64_t minV, int64_t maxV) {
		if (!root)
			return true;
		if (root->val >= maxV || root->val <= minV)
			return false;
		return isValidBST(root->left, minV, (int64_t)root->val) && isValidBST(root->right, (int64_t)root->val, maxV);
	}
public:
	// 98. Validate Binary Search Tree
	bool isValidBST(TreeNode* root) { // beat 72%, special cases is very tricky
		firstMin = firstMax = false;
		if (!root || (!root->left && !root->right)) // special case, return true if tree is null
			return true; // special case, single node with value MIN or MAX
		return isValidBST(root, INT_MIN, INT_MAX);
	}
	bool isValidBST2(TreeNode* root) { // use int64 to avoid INT_MIN, INT_MAX tricky boundary problem
		return isValidBST(root, LLONG_MIN, LLONG_MAX);  // beat 99%
	}

	// borrowed idea, inorder
	bool validate(TreeNode* node, TreeNode* &prev) {
		if (node == NULL) return true;
		if (!validate(node->left, prev)) return false;
		if (prev != NULL && prev->val >= node->val) return false;
		prev = node;
		return validate(node->right, prev);
	}
	bool isValidBST3(TreeNode* root) {
		TreeNode* prev = NULL;
		return validate(root, prev);
	}

	// 669. Trim a Binary Search Tree
	TreeNode* trimBST(TreeNode* root, int L, int R) { // beat 27%
		if (root == nullptr)
			return root;
		if (root->val > R)
			return trimBST(root->left, L, R);
		if (root->val < L)
			return trimBST(root->right, L, R);
		root->left = trimBST(root->left, L, R);
		root->right = trimBST(root->right, L, R);
		return root;
	}
};

TEST_CASE("BST valid", "[BST]")
{
	BST b;
	TreeNode *r4 = new TreeNode(3);
	r4->left = new TreeNode(2);
	r4->right = new TreeNode(INT_MAX);
	r4->left->left = new TreeNode(INT_MIN);
	CHECK(b.isValidBST(r4) == true);

	TreeNode *r2 = new TreeNode(INT_MIN);
	r2->right = new TreeNode(INT_MAX);
	CHECK(b.isValidBST(r2) == true);

	TreeNode *r5 = new TreeNode(INT_MIN);
	r5->right = new TreeNode(INT_MAX);
	r5->right->left = new TreeNode(INT_MIN);
	CHECK(b.isValidBST(r5) == false);

	TreeNode *r = new TreeNode(10);
	r->left = new TreeNode(5);
	r->right = new TreeNode(15);
	r->right->left = new TreeNode(6);
	r->right->right = new TreeNode(20);
	CHECK(b.isValidBST(r) == false);

	TreeNode *r3 = new TreeNode(3);
	r3->left = new TreeNode(2);
	r3->left->left = new TreeNode(INT_MAX);
	r3->left->right = new TreeNode(INT_MIN);
	CHECK(b.isValidBST(r3) == false);

	TreeNode *r6 = new TreeNode(3);
	r6->left = new TreeNode(2);
	r6->left->left = new TreeNode(1);
	r6->left->right = new TreeNode(INT_MIN);
	CHECK(b.isValidBST(r6) == false);
}
