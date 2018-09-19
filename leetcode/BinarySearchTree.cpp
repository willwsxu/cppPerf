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

	bool BinarySearchTargetP2(TreeNode* root, TreeNode* p1, int p2) {
		if (root == nullptr)
			return false;
		if (root != p1 && root->val == p2)
			return true;
		if (p2<root->val && BinarySearchTargetP2(root->left, p1, p2))
			return true;
		return p2 > root->val && BinarySearchTargetP2(root->right, p1, p2);
	}
	TreeNode* origRoot;
	// 653. Two Sum IV - Input is a BST
	bool findTargetHelp(TreeNode* root, int k) {
		if (root == nullptr)
			return false;
		if (BinarySearchTargetP2(origRoot, root, k - root->val)) // pick root as part 1 of target, find target-p1
			return true;
		// try left and right
		return findTargetHelp(root->left, k) || findTargetHelp(root->right, k);
	}
	bool findTarget(TreeNode* root, int k) {  // beat 34%
		origRoot = root;  // save original tree root, then go through all node as first part of sum, find sum-p1 in the tree
		return findTargetHelp(root, k);  // O(NlogN)
	}

	int convertBSThelp(TreeNode *root, TreeNode * parent, int extra) {
		if (!root)
			return 0;
		if (root->left == nullptr && root->right == nullptr) { // leaf node
			if (parent && root->val > parent->val) { // rigth subtree
				root->val += extra;
				return extra + root->val;
			}
			else { // left subtree
				if (parent && root->val < parent->val)
					root->val += parent->val;
				root->val += extra;
				return extra;
			}
		}
		int greater = convertBSThelp(root->right, root, extra);
		(void)convertBSThelp(root->left, root, greater); //
		int new_greater = greater;
		if (parent && root->val > parent->val)
			new_greater += root->val;
		root->val += greater;
		return new_greater;
	}
	// 38. Convert BST to Greater Tree
	// add to each node of sum of all node whose values are greater than current node
	TreeNode* convertBST(TreeNode* root) {
		convertBSThelp(root, nullptr, 0);
		return root;
	}

};
TEST_CASE("38. Convert BST to Greater Tree", "[NEW]")
{
	TreeNode *r = TreeNode::CreateBinaryTree({ 5,2,13 });
	r = BST().convertBST(r);
}

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
