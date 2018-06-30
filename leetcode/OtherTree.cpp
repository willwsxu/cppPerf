#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;


struct TreeLinkNode {
	int val;
	TreeLinkNode *left;
	TreeLinkNode *right;
	TreeLinkNode *next;
	TreeLinkNode(int x) : val(x), left(nullptr), right(nullptr), next(nullptr) {}
};

class TreeLink {
	void connectRight(TreeLinkNode *root, TreeLinkNode *left) {  //left child connect to right child
		if (!left)
			return;
		left->next = root->right;
		connectRight(left, left->left);
		connectLeft(left, left->right);
	}
	void connectLeft(TreeLinkNode *root, TreeLinkNode *right) { //right child connect to left child of next neighbor
		if (!right)
			return;
		if (!root->next)
			right->next = nullptr;
		else
			right->next = root->next->left;
		connectRight(right, right->left);
		connectLeft(right, right->right);
	}
public:
	// 116. Populating Next Right Pointers in Each Node
	// assume that it is a perfect binary tree
	void connect(TreeLinkNode *root) {  // beat 91%
		if (!root)
			return;
		root->next = nullptr;
		connectRight(root, root->left);
		connectLeft(root, root->right);
	}
	void connect1i(TreeLinkNode *root) {  // borrow idea, iterative, beat 91%
		if (!root)
			return;
		while (root->left) {
			TreeLinkNode *cur = root;
			while (cur) {  // cur loop all neighbors (->next)
				cur->left->next = cur->right;  // connect left child to right child
				if (cur->next)
					cur->right->next = cur->next->left;  // connect right child to left child of next neighbor
				cur = cur->next;
			}
			root = root->left;  // move the next level
		}
	}

	// 117. Populating Next Right Pointers in Each Node II, NOT a perfect binary tree
	void connect2(TreeLinkNode *root) {  // beat 99.65%
		while (root) {
			if (!root->left && !root->right) {  // find the first root with a child, at same level
				root = root->next;
				continue;
			}
			TreeLinkNode *cur = root;
			TreeLinkNode *conn = nullptr;   // node to connect
			while (cur) {
				if (cur->left) {
					if (conn)
						conn->next = cur->left;
					conn = cur->left;       // next to connect
				}
				if (cur->right) {
					if (conn)
						conn->next = cur->right;
					conn = cur->right;
				}
				cur = cur->next;
			}
			if (root->left)
				root = root->left;
			else if (root->right)
				root = root->right;
		}
	}
};

TEST_CASE("connect next neighbor", "[LINK]")
{
	TreeLinkNode *root = new TreeLinkNode(1);
	root->left = new TreeLinkNode(2);
	root->right = new TreeLinkNode(3);
	root->left->left = new TreeLinkNode(4);
	root->left->right = new TreeLinkNode(5);
	root->right->left = new TreeLinkNode(6);
	root->right->right = new TreeLinkNode(7);

	TreeLink t;
	t.connect(root);
	CHECK(root->left->right->next == root->right->left);
}

TEST_CASE("connect next neighbor 2", "[LINK]")
{
	TreeLinkNode *root = new TreeLinkNode(1);
	root->left = new TreeLinkNode(2);
	root->right = new TreeLinkNode(3);
	root->left->left = new TreeLinkNode(4);
	root->left->right = new TreeLinkNode(5);
	root->right->right = new TreeLinkNode(7);

	TreeLink t;
	t.connect2(root);
	CHECK(root->left->right->next == root->right->right);
}
