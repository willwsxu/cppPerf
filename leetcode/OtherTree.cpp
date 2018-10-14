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

#include "TreeNode.h"
#include "ListNode.h"

class List2Tree
{
	// len=2, midLen=0, mid index is 0, left len is 0, right len is 1
	// len=3, midLen=1, mid index is 1, left len is 1, right len is 1
	TreeNode *BST(ListNode* head, int len) {  // beat 50%
		if (len == 0)
			return nullptr;
		if (len == 1)
			return new TreeNode(head->val);
		ListNode *mid = head;
		int midLen = (len + 1) / 2-1;
		for (int i = 0; i < midLen; i++)
			mid = mid->next;
		TreeNode *left = BST(head, midLen);
		TreeNode *r = new TreeNode(mid->val);
		r->left = left;
		r->right = BST(mid->next, len - midLen-1);
		return r;
	}
public:
	TreeNode* sortedListToBST(ListNode* head) {
		int len = ListNode::count(head);
		return BST(head, len);
	}
};


struct RandomListNode {
	int label;
	RandomListNode *next, *random;
	RandomListNode(int x) : label(x), next(NULL), random(NULL) {}
};
class RandomList {
public:
	RandomListNode *copyRandomList(RandomListNode *head) {  // beat 82%
		unordered_map<RandomListNode *, RandomListNode*> old2new;
		old2new[nullptr] = nullptr;
		RandomListNode dummy(0);
		RandomListNode *cur = &dummy;
		while (head) {  // copy all nodes as is, first pass
			cur->next = new RandomListNode(head->label);
			cur = cur->next;
			cur->random = head->random;
			old2new[head] = cur;   // map old node to new node
			head = head->next;
		}
		cur = dummy.next;
		while (cur) {  // loop through new nodes
			cur->random = old2new[cur->random];  // update random pointer using map
			cur = cur->next;
		}
		return dummy.next;
	}
};

// 427. Construct Quad Tree
class Node {
public:
	bool val;
	bool isLeaf;
	Node* topLeft;
	Node* topRight;
	Node* bottomLeft;
	Node* bottomRight;

	Node() {}

	Node(bool _val, bool _isLeaf, Node* _topLeft, Node* _topRight, Node* _bottomLeft, Node* _bottomRight) {
		val = _val;
		isLeaf = _isLeaf;
		topLeft = _topLeft;
		topRight = _topRight;
		bottomLeft = _bottomLeft;
		bottomRight = _bottomRight;
	}
};
class QuadTree {
	// return -1 grid mixed value, 0 - grid all 0, 1 - grid all 1
	int eval(vector<vector<int>>& grid, int top, int left, int bottom, int right)
	{
		int topleft = grid[top][left];
		for (int r = top; r < bottom; r++) {
			for (int c = left; c < right; c++)
				if (grid[r][c] != topleft)
					return -1;
		}
		return topleft;
	}
	Node* construct(vector<vector<int>>& grid, int top, int left, int bottom, int right) {
		if (bottom - top <= 1 && right - left <= 1) {
			return new Node(grid[top][left] ? true : false, true, nullptr, nullptr, nullptr, nullptr);
		}
		int width = (right - left) / 2;
		int depth = (bottom - top) / 2;
		int val = eval(grid, top, left, bottom, right);
		Node *root = new Node();
		if (val == -1) {
			root->isLeaf = false;
			root->topLeft = construct(grid, top, left, top + depth, left + width);
			root->topRight = construct(grid, top, left + width, top + depth, right);
			root->bottomLeft = construct(grid, top + depth, left, bottom, left + width);
			root->bottomRight = construct(grid, top + depth, left + width, bottom, right);
		}
		else {
			root->isLeaf = true;
			root->val = val ? true : false;
			root->topLeft = nullptr;  // crash if nullptr were not assigned
			root->topRight = nullptr;
			root->bottomLeft = nullptr;
			root->bottomRight = nullptr;
		}
		return root;
	}

public:
	Node* construct(vector<vector<int>>& grid) {
		return construct(grid, 0, 0, grid.size(), grid.size());
	}

	// 558. Quad Tree Intersection, actually should union, logical or
	Node* intersect(Node* quadTree1, Node* quadTree2) {
		if (quadTree1 == nullptr)			return quadTree2;  //sanity
		if (quadTree2 == nullptr)			return quadTree1;
		if (quadTree1->isLeaf)
			return quadTree1->val ? quadTree1 : quadTree2;
		if (quadTree2->isLeaf)
			return quadTree2->val ? quadTree2 : quadTree1;

		auto tl = intersect(quadTree1->topLeft, quadTree2->topLeft);
		auto tr = intersect(quadTree1->topRight, quadTree2->topRight);
		auto bl = intersect(quadTree1->bottomLeft, quadTree2->bottomLeft);
		auto br = intersect(quadTree1->bottomRight, quadTree2->bottomRight);

		if (tl->isLeaf && tr->isLeaf && bl->isLeaf && br->isLeaf
			&& tl->val == tr->val && tr->val == bl->val && bl->val == br->val) {
			auto x = new Node(tl->val, true, nullptr, nullptr, nullptr, nullptr);
			delete tl;
			delete tr;
			delete bl;
			delete br;
			return x;
		}
		return new Node(false, false, tl, tr, bl, br);
	}
};
TEST_CASE("427. Construct Quad Tree", "[NEW]")
{
	Node *root = QuadTree().construct(vector<vector<int>>{ { 1, 1, 1, 1, 0, 0, 0, 0 }, { 1,1,1,1,0,0,0,0 }, { 1,1,1,1,1,1,1,1 }, { 1,1,1,1,1,1,1,1 }, { 1,1,1,1,0,0,0,0 }, { 1,1,1,1,0,0,0,0 }, { 1,1,1,1,0,0,0,0 }, { 1,1,1,1,0,0,0,0 } });
}
