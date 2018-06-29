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

	template<typename InIt>
	tuple<int, int> treePartition(int rootVal, InIt InFirst, InIt InStart, InIt InEnd, const unordered_map<int, int>& pos_map)
	{
		auto pos = pos_map.find(rootVal)->second;
		int left_size = distance(InStart, InFirst + pos);
		int right_size = distance(InFirst + pos + 1, InEnd);
		return{ left_size , right_size };
	}
	template<typename PreIt, typename InIt>
	TreeNode* buildTree(PreIt preFirst, PreIt preStart, PreIt preEnd, InIt InFirst, InIt InStart, InIt InEnd, const unordered_map<int, int>& pos_map)
	{
		TreeNode * r = new TreeNode(*preStart);  // first value in preorder vec is the root
		int left_size, right_size;
		tie(left_size, right_size) = treePartition(*preStart, InFirst, InStart, InEnd, pos_map);
		r->left = left_size ? buildTree(preFirst, preStart + 1, preStart + 1 + left_size, InFirst, InStart, InStart + left_size, pos_map) : nullptr;
		r->right = right_size ? buildTree(preFirst, preStart + 1 + left_size, preEnd, InFirst, InStart + left_size + 1, InEnd, pos_map) : nullptr;
		return r;
	}

	TreeNode* buildTree(vector<int>& pporder, vector<int>& inorder, bool post) {
		if (pporder.empty())
			return nullptr;
		unordered_map<int, int> pos_map;
		for (unsigned int i = 0; i < inorder.size(); i++)  // find position of inorder value
			pos_map.emplace(inorder[i], i);
		return post ? buildTreePost(pporder.rbegin(), pporder.rbegin(), pporder.rend(), inorder.begin(), inorder.begin(), inorder.end(), pos_map) :
			buildTree(pporder.begin(), pporder.begin(), pporder.end(), inorder.begin(), inorder.begin(), inorder.end(), pos_map);
	}

	// 105. Construct Binary Tree from Preorder and Inorder Traversal, no dup
	TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {  // beat 92%
		return buildTree(preorder, inorder, false);
	}

	template<typename PostRIT, typename InIt>
	TreeNode* buildTreePost(PostRIT pFirst, PostRIT pStart, PostRIT pEnd, InIt InFirst, InIt InStart, InIt InEnd, const unordered_map<int, int>& pos_map)
	{
		TreeNode * r = new TreeNode(*pStart);  // first value from back in postorder vec is the root
		int left_size, right_size;
		tie(left_size, right_size) = treePartition(*pStart, InFirst, InStart, InEnd, pos_map);

		// code is same as inorder buildTree, except how to calculate left and right side for postorder
		r->left = left_size ? buildTreePost(pFirst, pStart + 1 + right_size, pEnd, InFirst, InStart, InStart + left_size, pos_map) : nullptr;
		r->right = right_size ? buildTreePost(pFirst, pStart + 1, pStart + 1 + right_size, InFirst, InStart + left_size + 1, InEnd, pos_map) : nullptr;
		return r;
	}

	// 106. Construct Binary Tree from Inorder and Postorder Traversal, no dup
	// inorder = [9, 3, 15, 20, 7]
	// postorder = [9, 15, 7, 20, 3]
	TreeNode* buildTreePost(vector<int>& inorder, vector<int>& postorder) {  // beat 88%
		return buildTree(postorder, inorder, true);  // beat 84% after refactoring out common code between preorder and postorder
	}

	int getHeight(TreeNode* r)
	{
		return r == nullptr ? 0 : max(getHeight(r->left), getHeight(r->right)) + 1;
	}
	// 655. Print Binary Tree
	void printTree(TreeNode* root, int level, vector<vector<string>>&ans, int L, int R) {
		if (root == nullptr)	return;
		int center = (L + R) / 2;
		ans[level][center] = to_string(root->val);
		printTree(root->left, level + 1, ans, L, center - 1);
		printTree(root->right, level + 1, ans, center + 1, R);
	}
	vector<vector<string>> printTree(TreeNode* root) {  // beat 63%
		if (root == nullptr)
			return vector<vector<string>>{};
		size_t h = getHeight(root);  // rows
		size_t w = size_t(pow(2, h) - 1);
		vector<vector<string>> ans{ h, vector<string>{w, ""} };
		printTree(root, 0, ans, 0, w - 1);
		return ans;
	}

	template<typename RandIt>
	TreeNode* constructMaximumBinaryTree(RandIt b, RandIt e) {
		if (b == e)
			return nullptr;
		auto m = max_element(b, e);
		TreeNode *r = new TreeNode(*m);
		r->left = constructMaximumBinaryTree(b, m);
		r->right = constructMaximumBinaryTree(m + 1, e);
		return r;
	}
	//654. Maximum Binary Tree
	TreeNode* constructMaximumBinaryTree(vector<int>& nums) {  // beat 89%
		return constructMaximumBinaryTree(nums.begin(), nums.end());
	}

	//814. Binary Tree Pruning
	// The binary tree will have at most 100 nodes
	TreeNode* pruneTree(TreeNode* root) {
		if (root == nullptr)
			return nullptr;
		if (root->val == 0 && root->left == nullptr && root->right == nullptr) {  // prune node if its value is 0 and both children are null
			delete root;
			return nullptr;
		}
		TreeNode *left = pruneTree(root->left);
		TreeNode *right = pruneTree(root->right);
		if (root->val == 0 && left == nullptr && right == nullptr) {  // prune again after recursive call
			delete root;
			return nullptr;
		}
		root->left = left;
		root->right = right;
		return root;
	}

	unordered_map<int, int> sum_count;
	int max_count;
	int findFrequentTreeSumHelp(TreeNode* root) {
		if (root == nullptr)
			return 0;
		int sum = root->val;
		sum += findFrequentTreeSumHelp(root->left);
		sum += findFrequentTreeSumHelp(root->right);
		if (sum_count.find(sum) != sum_count.end()) {
			sum_count[sum]++;
		}
		else
			sum_count[sum] = 1;
		max_count = max(sum_count[sum], max_count);
		return sum;
	}
	// 508. Most Frequent Subtree Sum
	// refactor to use data member, instead of passing arguments, speed up to 95%, from 5%
	vector<int> findFrequentTreeSum(TreeNode* root) {
		max_count = 0;
		auto res = findFrequentTreeSumHelp(root);
		vector<int> ans;
		for (auto& p : sum_count) {
			if (p.second == max_count)
				ans.push_back(p.first);
		}
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

	// 144. Binary Tree Preorder Traversal
	vector<int> preorderTraversal(TreeNode* root) {  // beats 96%
		stack<TreeNode*> nodes;
		if (root)
			nodes.push(root);
		vector<int> ans;
		while (!nodes.empty()) {
			root = nodes.top();
			ans.push_back(root->val); // preorder, add root first
			nodes.pop();  // remove root from stack
			if (root->right)
				nodes.push(root->right);  // push right first so left will pop visited before right
			if (root->left)
				nodes.push(root->left);
		}
		return ans;
	}

	void addOneRow(TreeNode *root, int v, int depth, int level)
	{
		if (root == nullptr)
			return;
		if (level == depth - 1) {
			TreeNode *nL = new TreeNode(v);  // create two new nodes at depth, child of depth-1 node
			TreeNode *nR = new TreeNode(v);
			nL->left = root->left;           // left node gets left subtree
			root->left = nL;
			nR->right = root->right;         // right node gets right subtree
			root->right = nR;
		}
		else if (level < depth - 1) {
			addOneRow(root->left, v, depth, level + 1);
			addOneRow(root->right, v, depth, level + 1);
		}
	}
	// 623. Add One Row to Tree
	TreeNode* addOneRow(TreeNode* root, int v, int d) {  // beat 97%
		if (d == 1) { // special case, create new root and add old root as left child
			TreeNode*newRoot = new TreeNode(v);
			newRoot->left = root;
			return newRoot;
		}
		addOneRow(root, v, d, 1);
		return root;
	}

	unordered_map<TreeNode*, int> dp[2];  // need to store calculation for true and false cases (bug source!!!)
	int rob(TreeNode* root, bool pick) {   // beat 33%
		if (root == nullptr)
			return 0;
		auto found = dp[pick].find(root); // TLE without dp solution
		if (found != dp[pick].end())
			return found->second;
		int left = rob(root->left, !pick);
		if (!pick)
			left = max(left, rob(root->left, false)); // not pick can be consecutive (bug source!!!)
		int right = rob(root->right, !pick);
		if (!pick)
			right = max(right, rob(root->right, false));
		int total = left + right + (pick ? root->val : 0);
		dp[pick][root] = total;
		return total;
	}

	pair<int, int> rob2(TreeNode* root) { // borrowed idea, beat 70%
		if (root == nullptr)
			return{ 0,0 };
		auto left = rob2(root->left);
		auto right = rob2(root->right);
		pair<int, int> res;  // first exclude root, second include root
		res.first = max(left.first, left.second) + max(right.first, right.second);
		res.second = root->val + left.first + right.first;
		return res;
	}
	// 337. House Robber III
	int rob(TreeNode* root) {
		auto res = rob2(root);
		return max(res.first, res.second);
		/*
		int total = rob(root, true);
		dp[0].clear();
		dp[1].clear();
		return max(total, rob(root, false));*/
	}


	// 129. Sum Root to Leaf Numbers
	// Given a binary tree containing digits from 0-9 only, each root-to-leaf path could represent a number.
	int sumNumbers(TreeNode* root, int prev) {  // beat 81%
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

	// 662. Maximum Width of Binary Tree
	// length between the end-nodes (the leftmost and right most non-null nodes in the level
	int widthOfBinaryTree(TreeNode* root) { // slow but works, need to find a quicker solution
		if (root == nullptr)
			return 0;
		deque<TreeNode*> nodes[2];
		size_t maxW = 1;
		int inUse = 0;
		nodes[inUse].push_back(root);
		while (!nodes[inUse].empty()) {
			auto oldSize = nodes[inUse].size();
			for (size_t i = 0; i < oldSize; i++) {
				if (nodes[inUse].at(i) == nullptr) {
					nodes[1 - inUse].push_back(nullptr);
					nodes[1 - inUse].push_back(nullptr);
				}
				else {
					nodes[1 - inUse].push_back(nodes[inUse].at(i)->left);
					nodes[1 - inUse].push_back(nodes[inUse].at(i)->right);
				}
			}
			nodes[inUse].clear();
			inUse = 1 - inUse;  //switch to other deque
			while (!nodes[inUse].empty() && nodes[inUse].front() == nullptr)  // trim front and back
				nodes[inUse].pop_front();
			while(!nodes[inUse].empty() && nodes[inUse].back()==nullptr)
				nodes[inUse].pop_back();
			maxW = max(maxW, nodes[inUse].size());
		}
		return maxW;
	}

	// 450. Delete Node in a BST
	void deleteNodeHelper(TreeNode* del) {
		TreeNode* parent = del;
		if (del->left) { // find the largest node in left subtree
			TreeNode* largest = del->left;
			while (largest->right) {
				parent = largest;
				largest = largest->right;
			}
			del->val = largest->val; // replace deleted node with largest value on left sub tree
			if (parent == del)
				parent->left = largest->left;
			else
				parent->right = largest->left;
			delete largest;
		}
		else if (del->right) {  // find the smallest node in right subtree
			TreeNode* smallest = del->right;
			while (smallest->left) {
				parent = smallest;
				smallest = smallest->left;
			}
			del->val = smallest->val; // replace deleted node with largest value on left sub tree
			if (parent == del)
				parent->right = smallest->right;
			else
				parent->left = smallest->right;
			delete smallest;
		}
	}
	TreeNode* deleteNode(TreeNode* root, int key) { // beat 61%
		if (!root)
			return nullptr;
		TreeNode *parent = root;
		TreeNode *target = root;
		while (target && target->val != key) {
			parent = target;
			if (key < target->val)
				target = target->left;
			else
				target = target->right;
		}
		if (target == nullptr) // not found
			return root;
		if (!target->left && !target->right) { // target is leaf node
			delete target;
			if (target == root) {
				return nullptr;
			}
			else if (parent->left == target)
				parent->left = nullptr;
			else
				parent->right = nullptr;
			return root;
		}
		deleteNodeHelper(target);
		return root;
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

TEST_CASE("Serialize and Deserialize BST", "[SER]")
{
	TreeNode * r = TreeNode::createBST(vector<int>{4,3,5,1,2,6}, 0);
	Codec c;
	string ser = c.serialize(r);
	CHECK(c.serialize(c.deserialize(ser))==ser);
}

TreeNode *createTreeByLevel(vector<int> v, int i) {
	return nullptr;
}
TEST_CASE("tree width", "[NEW]")
{
	TreeNode *root = new TreeNode(1);
	root->left = new TreeNode(3);
	root->right = new TreeNode(2);
	root->left->left = new TreeNode(5);
	root->left->right = new TreeNode(3);
	root->right->right = new TreeNode(9);
	Tree t;
	CHECK(t.widthOfBinaryTree(root)==4);
}

TEST_CASE("BST delete", "[NEW]")
{
	TreeNode * r = TreeNode::createBST(vector<int>{3,2,1,4}, 0);
	Tree t;
	Codec c;
	CHECK(c.serialize(t.deleteNode(r, 3))=="2 1 4 ");
}

class FlattenTree
{
	vector<TreeNode*> ans;
	unordered_map<string, int> count;
	string findDuplicateSubtreesFlatten(TreeNode* root) { // beat 33%
		if (root == nullptr)
			return "";
		string subtree;
		subtree.reserve(100);  // 100 seem the best value to reserve
		subtree.append(to_string(root->val)).append(",L,").append(findDuplicateSubtreesFlatten(root->left)).append("R,").append(findDuplicateSubtreesFlatten(root->right)); // mark Left and right
		auto found = count.find(subtree);
		if (found == count.end())
			count[subtree] = 1;
		else {
			auto& val = count[subtree];  // avoid calling mapping 2 extra times
			if (val == 1)
				ans.push_back(root);
			val = (val + 1);
		}
		return subtree;
	}

public:
	// 652. Find Duplicate Subtrees
	// Given a binary tree, return all duplicate subtrees.For each kind of duplicate subtrees, you only need to return the root node of any one of them.
	vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
		(void)findDuplicateSubtreesFlatten(root);
		return ans;
	}

	TreeNode* flattenHelp(TreeNode* root) {  // beat 22%
		if (root == nullptr || root->left == nullptr && root->right == nullptr)
			return root;
		// single child, flatten it
		if (!root->left)
			return flattenHelp(root->right);
		TreeNode* left = flattenHelp(root->left);
		// both children, flatten left first
		left->right = root->right;		// append right to left
		root->right = root->left;       // move left subtree to right
		root->left = nullptr;           // set left to null (bug source!!)
		if (!left->right)
			return left;
		return flattenHelp(left->right);
	}

	// borrow idea
	TreeNode *prev;
	void flattenPost(TreeNode* root) {  // beat 43%
		if (root == nullptr)
			return;
		flattenPost(root->right);
		flattenPost(root->left);
		root->right = prev;
		root->left = nullptr;
		prev = root;
	}

	// 114. Flatten Binary Tree to Linked List, in place
	void flatten(TreeNode* root) {
		prev = nullptr;
		flattenPost(root);
		/*
		while (root) {  // iterative version is a little worse than recursive
		if (root->left && root->right) {
		TreeNode* n = root->left;
		while (n->right)  // find the right most node of left subtree
		n = n->right;
		n->right = root->right; // stick right subtree to bottom of left subtree
		}
		if (root->left) {  // move left to right
		root->right = root->left;
		root->left = nullptr;
		}
		root = root->right;
		}
		*/
	}
};

TEST_CASE("flatten tree", "[FLAT]")
{
	TreeNode *r = new TreeNode(1);
	r->left = new TreeNode(2);
	r->right = new TreeNode(5);
	r->left->left = new TreeNode(3);
	r->left->right = new TreeNode(4);
	r->right->right = new TreeNode(6);
	FlattenTree t;
	t.flatten(r);
}

class TreePath
{
	// 112. Path Sum, if the tree has a root-to-leaf path such that adding up all the values along the path equals the given sum
	bool pathSum(TreeNode* root, int sum, int target) { // beat 99%
		if (!root)
			return false;
		sum += root->val;
		if (!root->left && !root->right)
			return sum == target;
		if (root->left && pathSum(root->left, sum, target))
			return true;
		if (root->right && pathSum(root->right, sum, target))
			return true;
		return false;
	}
public:
	bool hasPathSum(TreeNode* root, int sum) {
		return pathSum(root, 0, sum);
	}

	int pathSum3(TreeNode* root, int sum, int target) {
		if (!root)
			return 0;
		sum += root->val;
		int count = sum == target ? 1 : 0;
		count += pathSum3(root->left, sum, target);
		count += pathSum3(root->right, sum, target);
		return count;
	}
	int pathSum3Help(TreeNode* root, int sum) {
		if (!root)
			return 0;
		return (root->val == sum ? 1 : 0) + pathSum3Help(root->left, sum - root->val) + pathSum3Help(root->right, sum - root->val);
	}
	// 437. Path Sum III, The path does not need to start or end at the root or a leaf, but it must go downwards
	int pathSum3(TreeNode* root, int sum) {
		if (!root)
			return 0;
		return pathSum3(root, 0, sum) + pathSum3(root->left, sum) + pathSum3(root->right, sum);
	}

	unordered_map<int, int> mii_count;
	int pathSum3PrefixSum(TreeNode* root, int sum, int target) {  // beat 91%
		if (!root)
			return 0;
		sum += root->val;
		auto found = mii_count.find(sum - target);
		int res = found == mii_count.end() ? 0 : found->second;
		found = mii_count.find(sum);
		auto & currMapCount = mii_count[sum];
		currMapCount = found == mii_count.end() ? 1 : found->second + 1;
		res += pathSum3PrefixSum(root->left, sum, target) + pathSum3PrefixSum(root->right, sum, target);

		currMapCount = currMapCount - 1;  // roll back 1 as this path is complete, don't count toward other branches!!!
		return res;
	}
	int pathSum3PrefixSum(TreeNode* root, int sum) {
		mii_count[0] = 1;  // add sum=0 as default to calculate diff of prefix sum
		return pathSum3PrefixSum(root, 0, sum);
	}

	vector<vector<int>> paths;
	vector<int> one;
	void pathSum2x(TreeNode* root, int sum, int target) {  // beat 84%
		if (!root)
			return;
		sum += root->val;
		one.push_back(root->val);
		if (!root->left && !root->right) {
			if (sum == target)
				paths.push_back(one);
		}
		else {
			if (root->left)
				pathSum2x(root->left, sum, target);
			if (root->right)
				pathSum2x(root->right, sum, target);
		}
		one.pop_back();
	}
	void pathSum2(TreeNode* root, int sum, int target, vector<int> p) {  // first attemp, slow, too much vector copy
		if (!root)
			return;
		sum += root->val;
		p.push_back(root->val);
		if (!root->left && !root->right) {
			if (sum == target)
				paths.push_back(p);
			return;
		}
		if (root->left)
			pathSum2(root->left, sum, target, p);
		if (root->right)
			pathSum2(root->right, sum, target, p);
	}
	// 113. Path Sum II, 
	// Given a binary tree and a sum, find all root - to - leaf paths where each path's sum equals the given sum
	vector<vector<int>> pathSum(TreeNode* root, int sum) {
		//pathSum2(root, 0, sum, vector<int>{});
		pathSum2x(root, 0, sum);
		return paths;
	}
};

TEST_CASE("Path sum", "[PATH]")
{
	TreeNode *r = new TreeNode(1);
	r->left = new TreeNode(2);
	TreePath t;
	CHECK(t.hasPathSum(r, 1) == false);
	CHECK(t.hasPathSum(nullptr, 1) == false);
}

TEST_CASE("Path sum 3", "[PATH]")
{
	TreeNode *r = new TreeNode(10);
	r->left = new TreeNode(5);
	r->right = new TreeNode(-3);
	r->left->left = new TreeNode(3);
	r->left->right = new TreeNode(2);
	r->right->right = new TreeNode(11);
	r->left->left->left = new TreeNode(3);
	r->left->left->right = new TreeNode(-2);
	r->left->right->right = new TreeNode(1);

	TreePath t;
	t.pathSum3(r, 8);
}

TEST_CASE("Path sum 2", "[PATH]")
{
	TreeNode *r = new TreeNode(5);
	r->left = new TreeNode(4);
	r->right = new TreeNode(8);
	r->left->left = new TreeNode(11);
	r->right->left = new TreeNode(13);
	r->right->right = new TreeNode(4);
	r->left->left->left = new TreeNode(7);
	r->left->left->right = new TreeNode(2);
	r->right->right->left = new TreeNode(5);
	r->right->right->right = new TreeNode(1);

	TreePath t;
	CHECK(t.pathSum(r, 22) == vector<vector<int>>{ {5, 4, 11, 2}, { 5,8,4,5 }});
}


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

TEST_CASE("connect next neighbor", "[NEW]")
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

TEST_CASE("connect next neighbor 2", "[NEW]")
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


class LCA
{
	vector<TreeNode *> lcaCur;
	int match = 0;
	vector<TreeNode *> lca[2]; // store the path, then find the common part
	void LcaHelp(TreeNode* root, TreeNode* p, TreeNode* q)
	{
		if (!root || match == 2)
			return;
		lcaCur.push_back(root);
		if (root == p || root == q) {
			lca[match++] = lcaCur;
		}
		LcaHelp(root->left, p, q);
		LcaHelp(root->right, p, q);
		lcaCur.pop_back();
	}

public:
	// 236. Lowest Common Ancestor of a Binary Tree
	// All of the nodes' values will be unique
	TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {  // beat 86%
		LcaHelp(root, p, q);
		int minSize = min(lca[0].size(), lca[1].size());
		int i = 0;
		for (; i < minSize; i++) {
			if (lca[0][i] != lca[1][i])
				break;
		}
		return i < 1 ? nullptr : lca[0][i - 1];
	}

	// borrowed idea
	TreeNode* lowestCommonAncestor2(TreeNode* root, TreeNode* p, TreeNode* q) {  // beat 86%
		if (!root || root == p || root == q)
			return root;
		TreeNode *left = lowestCommonAncestor2(root->left, p, q);
		TreeNode *right = lowestCommonAncestor2(root->right, p, q);
		return !left ? right : !right ? left : root;
	}
};

TEST_CASE("Lowest Common Ancestor", "[NEW]")
{
	TreeNode *r = new TreeNode(3);
	r->left = new TreeNode(5);
	r->right = new TreeNode(1);
	r->left->left = new TreeNode(6);
	r->left->right = new TreeNode(2);
	r->right->left = new TreeNode(0);
	r->right->right = new TreeNode(8);
	r->left->right->left = new TreeNode(7);
	r->left->right->right = new TreeNode(4);

	LCA t;
	CHECK(t.lowestCommonAncestor(r, r->left, r->right)==r);
}
