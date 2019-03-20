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
	int val;
	bool isInt;
public:
	// Constructor initializes an empty nested list.
	NestedInteger() :isInt(false), val(0) {}

	// Constructor initializes a single integer.
	NestedInteger(int value):isInt(true), val(value) {}

	// Return true if this NestedInteger holds a single integer, rather than a nested list.
	bool isInteger() const
	{
		return isInt;
	}

	// Set this NestedInteger to hold a single integer.
	void setInteger(int value) {
		val = value; isInt = true;	}
	
	// Set this NestedInteger to hold a nested list and adds a nested integer to it.
	void add(const NestedInteger &ni) {}

	// Return the single integer that this NestedInteger holds, if it holds a single integer
	// The result is undefined if this NestedInteger holds a nested list
	int getInteger() const
	{
		return val;
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

class MiniParser {  // #385
public:
	NestedInteger parsing(stringstream& iss) {  // recursion with loop
		NestedInteger ni;
		while (!iss.eof()) {
			char c = iss.peek();
			switch (c) {
			case '[':
				iss.ignore();
				if (iss.peek() == ']') {  // empty list
					iss.ignore();
					return ni;
				}
				ni.add(parsing(iss));
				break;
			case ']':
				iss.ignore();
				return ni;
			case ',':
				iss.ignore();
				ni.add(parsing(iss));
				break;
			case '-':
			default:
			{
				if (iss.peek() == '"')
					iss.ignore();
				int val;
				iss >> val;
				ni.setInteger(val);
				if (iss.peek() == '"')
					iss.ignore();
				return ni;
			}
			}
		}
		return ni;
	}
	// 385. Mini Parser, Given a nested list of integers represented as a string, implement a parser to deserialize it.
	NestedInteger deserialize(string s) {  //beat 100%
		stringstream iss(s);
		return parsing(iss);
	}
};


TEST_CASE("nested int parser", "[NEW]")
{
	MiniParser().deserialize("[[]]");	
	MiniParser().deserialize("[123,456,[788,799,833],[[]],10,[]]");
	MiniParser().deserialize("[123,[456,[789]]]");
	CHECK(MiniParser().deserialize("123").getInteger()==123);
}


// Below is the interface for Iterator, which is already defined for you.
// **DO NOT** modify the interface for Iterator.
class Iterator {
	struct Data;
	Data* data;
public:
	Iterator(const vector<int>& nums) {}
	Iterator(const Iterator& iter) {}
	virtual ~Iterator() {}
	// Returns the next element in the iteration.
	int next()
	{
		return 0;
	}
	// Returns true if the iteration has more elements.
	bool hasNext() const
	{
		return true;
	}
};


class PeekingIterator : public Iterator {  // beat 100%, 0 ms
	bool peeked;
	int  cache;
public:
	PeekingIterator(const vector<int>& nums) : Iterator(nums) {
		// Initialize any member here.
		// **DO NOT** save a copy of nums and manipulate it directly.
		// You should only use the Iterator interface methods.
		peeked = false;
	}

	// Returns the next element in the iteration without advancing the iterator.
	int peek() {
		if (!peeked) {
			peeked = true;
			cache = Iterator::next();
		}
		return cache;
	}

	// hasNext() and next() should behave the same as in the Iterator interface.
	// Override them if needed.
	int next() {
		if (peeked) {
			peeked = false;
			return cache;
		}
		return Iterator::next();
	}

	bool hasNext() const {
		return peeked || Iterator::hasNext();
	}
};
