#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

#include "TreeNode.h"

// 173. Binary Search Tree Iterator, iterator will be initialized with the root node of a BST
// Calling next() will return the next smallest number in the BST
// no duplciate
class BSTIterator {   // beats 97%!!
	stack<TreeNode*> roots;
public:
	BSTIterator(TreeNode *root) {
		while (root) {
			roots.push(root);
			root = root->left;
		} // get to left most leaf node
	}

	/** @return whether we have a next smallest number */
	bool hasNext() {
		return !roots.empty();
	}

	/** @return the next smallest number */
	int next() {
		TreeNode *cur = roots.top();
		int ans = cur->val;
		roots.pop();
		cur = cur->right;
		while (cur) {
			roots.push(cur);
			cur = cur->left;
		}
		return ans;
	}
};

// 341. Flatten Nested List Iterator
// This is the interface that allows for creating nested lists.
// You should not implement it, or speculate about its implementation
class NestedInteger {
public:
	// Return true if this NestedInteger holds a single integer, rather than a nested list.
	bool isInteger() const
	{
		return true;
	}

	// Return the single integer that this NestedInteger holds, if it holds a single integer
	// The result is undefined if this NestedInteger holds a nested list
	int getInteger() const
	{
		return 1;
	}

	// Return the nested list that this NestedInteger holds, if it holds a nested list
	// The result is undefined if this NestedInteger holds a single integer
	const vector<NestedInteger> &getList() const
	{
		static vector<NestedInteger> test;
		return test;
	}
};
#include <stack>
#include <assert.h>

class NestedIterator {  // beat 99.95%
	typedef pair<vector<NestedInteger>::const_iterator, vector<NestedInteger>::const_iterator> iter_pair;  // getList returns const
	std::stack<iter_pair> nested;
	void findNextInt()
	{
		auto * t = &nested.top();
		while (!t->first->isInteger()) {
			auto& nlist = t->first->getList();
			if (!nlist.empty()) {
				nested.emplace(cbegin(nlist), cend(nlist));
			}
			else {
				seekNext();
				if (nested.empty())
					break;
			}
			t = &nested.top();   // update top pointer
		}
	}
	void seekNext()
	{
		while (!nested.empty() && ++nested.top().first == nested.top().second)  // check stack empty
			nested.pop();
	}
public:
	NestedIterator(vector<NestedInteger> &nestedList) {
		if (nestedList.empty())
			return;
		nested.emplace(cbegin(nestedList), cend(nestedList));
		findNextInt();
	}

	int next() {
		assert(nested.top().first->isInteger());
		int ans = nested.top().first->getInteger();
		seekNext();
		if (hasNext()) {
			findNextInt();
		}
		return ans;
	}

	bool hasNext() {
		return !nested.empty();
	}
};
