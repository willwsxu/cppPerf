#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;
#include "TreeNode.h"
#include "helper.h"

class Tree {
public:
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
		size_t h = TreeNode::getHeight(root);  // rows
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

	// 222. Count Complete Tree Nodes
	int countNodes(TreeNode* root) { // beat 100%
		if (!root)
			return 0;
		int h = TreeNode::heightL(root);
		int hr = TreeNode::heightR(root);
		if (hr == h)  // complete binary tree
			return (1 << h) - 1;
		return 1 + countNodes(root->left) + countNodes(root->right);
	}
	// 894. All Possible Full Binary Trees
	vector<TreeNode*> allPossibleFBT(int N) { // beat 71%, bottom up
		if (N % 2 == 0)  // must be odd for full BT
			return{};
		if (N == 1)
			return{ new TreeNode{0} };
		vector<TreeNode*> heads;
		for (int r = 2; r < N; r++) {
			heads.reserve(heads.size() + (r - 1)*(N - r));
			auto left = allPossibleFBT(r - 1);
			auto right = allPossibleFBT(N - r);
			for (const auto L : left) {
				for (const auto R : right) {
					heads.push_back(new TreeNode{ 0 });
					heads.back()->left = L;
					heads.back()->right = R;
				}
			}
		}
		return heads;
	}
	pair<int, TreeNode*> subtreeWithAllDeepestHelper(TreeNode* root) {
		if (!root)
			return{ 0, nullptr };
		auto left = subtreeWithAllDeepestHelper(root->left);
		auto right = subtreeWithAllDeepestHelper(root->right);
		return{ max(left.first,right.first) + 1, left.first == right.first ? root : (left.first > right.first ? left.second : right.second) };
	}
	// 865. Smallest Subtree with all the Deepest Nodes
	TreeNode* subtreeWithAllDeepest(TreeNode* root) {
		return subtreeWithAllDeepestHelper(root).second;
		/*
		if (!root)
			return nullptr;
		int diff = TreeNode::getHeight(root->left) - TreeNode::getHeight(root->right);
		if (!diff)
			return root;
		return diff > 0 ? subtreeWithAllDeepest(root->left) : subtreeWithAllDeepest(root->right);
		*/
	}

	// 951. Flip Equivalent Binary Trees
	bool flipEquiv(TreeNode* root1, TreeNode* root2) {
		if (root1 == nullptr && root2 == nullptr)
			return true;
		else if (root1 == nullptr || root2 == nullptr)
			return false;
		else if (root1->val != root2->val)
			return false;
		return flipEquiv(root1->left, root2->left) && flipEquiv(root1->right, root2->right) || flipEquiv(root1->right, root2->left) && flipEquiv(root1->left, root2->right);
	}

	bool is_leaf_or_none(TreeNode *r) {
		if (r) {
			if (r->left || r->right)
				return false;
		}
		return true;
	}
	bool need_camera_cover(TreeNode *r) {
		if (r && r->val == 0)
			return true;
		return false;
	}
	bool camera_installed(TreeNode *r) {
		if (r && r->val == 1)
			return true;
		return false;
	}
public:
	// 968. Binary Tree Cameras. a camera on a node covers parent and immediate children
	// find minimal cameras needed to cover all
	int dfs(TreeNode* root) {  // greedy always start from leaf and install camera on leaf's parent
		if (!root || (root->left == nullptr && root->right == nullptr))
			return 0;
		if (is_leaf_or_none(root->left) && is_leaf_or_none(root->right)) {
			root->val = 1;
			return 1;
		}
		int cameras = dfs(root->left)+ dfs(root->right);
		if (need_camera_cover(root->left) || need_camera_cover(root->right)) {
			root->val = 1;
			cameras++;
		}
		else if (camera_installed(root->left) || camera_installed(root->right))
			root->val = 2;  // covered by its children
		return cameras;
	}
	int minCameraCover(TreeNode* root) {
		int ans = dfs(root);
		if (ans == 0)
			return root ? 1 : 0;
		if (root->val == 0 && !camera_installed(root->left) && !camera_installed(root->right))
			ans++;
		return ans;
	}
};

TEST_CASE("968. Binary Tree Cameras", "[NEW]")
{
	SECTION("4 node edge case") {
		auto t = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"0", "null", "0", "null", "0", "null", "0"});
		CHECK(Tree().minCameraCover(t) == 2);
	}
	SECTION("longer Tree of more nodes") {
		auto t2 = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"0", "null", "0", "null", "0", "null", "0", "0", "0", "null", "0", "null", "0", "0"});
		CHECK(Tree().minCameraCover(t2) == 3);

		auto t = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"0", "0", "null", "null", "0", "0", "null", "null", "0", "0"});
		CHECK(Tree().minCameraCover(t) == 2);
	}
	SECTION("Tree of more nodes") {
		auto t2 = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"0", "null", "0", "0", "0", "null", "null", "null", "0"});
		CHECK(Tree().minCameraCover(t2) == 2);
		auto t = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"0", "0", "null", "0", "null", "0", "null", "null", "0"});
		CHECK(Tree().minCameraCover(t) == 2);
	}
	SECTION("Tree of 1 node") {
		auto t1 = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"0", "null", "null"});
		CHECK(Tree().minCameraCover(t1) == 1);
	}
	SECTION("Tree of 2 nodes") {
		auto t = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"0", "0", "null", "null"});
		CHECK(Tree().minCameraCover(t) == 1);
	}
	SECTION("Tree of 4 nodes") {
		auto t = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"0", "0", "null", "0", "0"});
		CHECK(Tree().minCameraCover(t) == 1);
	}
}
TEST_CASE("865. Smallest Subtree with all the Deepest Nodes", "[NEW]")
{
	auto *x = TreeNode::CreateBinaryTree({ 3,5,1,6,2,0,8,INT32_MIN,INT32_MIN,7,4 });
	CHECK(Tree().subtreeWithAllDeepest(x) == x->left->right);
}
class TreeTraversal
{
public:
	// 515. Find Largest Value in Each Tree Row of a binary tree.
	// BFS, similar to 
	//   102. Binary Tree Level Order Traversal, 
	//   199. Binary Tree Right Side View, 
	//   103. Binary Tree Zigzag Level Order Traversal,
	//   513. Find Bottom Left Tree Value
	vector<int> largestValues(TreeNode* root) {
		vector<int> ans;
		if (!root)
			return ans;
		deque<TreeNode*> q{ root };
		while (!q.empty()) {
			ans.push_back((*max_element(begin(q), end(q), [](TreeNode *r1, TreeNode* r2) {return r1->val < r2->val; }))->val);
			int oldSize = q.size();
			while (oldSize--) {
				TreeNode *tn = q.front();
				q.pop_front();
				if (tn->left)
					q.push_back(tn->left);
				if (tn->right)
					q.push_back(tn->right);
			}
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
	// 145. Binary Tree Postorder Traversal
    vector<int> postorderTraversal(TreeNode* root) { // reverse of pre-order, beat 100%
		if (!root)
			return{};
		deque<TreeNode*> post_order_stack{ root };
		vector<int> ans;  // in reverse order
		while (!post_order_stack.empty()) {
			root = post_order_stack.back();
			ans.push_back(root->val);  // pre-order visit, order will be reversed at end
			post_order_stack.pop_back();
			if (root->left)
				post_order_stack.push_back(root->left);  // visit child in reverse order
			if (root->right)
				post_order_stack.push_back(root->right);
		}
		reverse(begin(ans), end(ans));
		return ans;
    }

	// 987. Vertical Order Traversal of a Binary Tree
	void verticalTraversal(TreeNode* root, int col, int row, map<int, map<int, vector<int>>>& col_row_map) {
		if (!root)
			return;
		col_row_map[col][row].push_back(root->val);
		verticalTraversal(root->left, col - 1, row + 1, col_row_map);
		verticalTraversal(root->right, col + 1, row + 1, col_row_map);
	}
public:
	vector<vector<int>> verticalTraversal(TreeNode* root) {
		map<int, map<int, vector<int>>> col_row_map;
		verticalTraversal(root, 0, 0, col_row_map);
		vector<vector<int>> ans;
		for (auto& col : col_row_map) {
			//cout << col.first << "=col\n";
			ans.push_back(vector<int>());
			for (auto& row : col.second) {
				sort(begin(row.second), end(row.second));
				copy(begin(row.second), end(row.second), back_inserter(ans.back()));
			}
			//copy(begin(ans.back()), end(ans.back()), ostream_iterator<int>(cout, " "));
			//cout << "\n";
		}
		return ans;
	}
};
TEST_CASE("145. Binary Tree Postorder Traversal", "[NEW]")
{
	TreeNode *root = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"1", "2", "3", "4", "5", "6", "7", "null", "null", "8","null","null","9"});
	CHECK(TreeTraversal().postorderTraversal(root) == vector<int>{4,8,5,2,9,6,7,3,1});
}

TEST_CASE("largestValues", "[NEW]")
{
	TreeNode *root = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"1","3","2","5","3","null","9"});
	CHECK(TreeTraversal().largestValues(root) == vector<int>{1, 3, 9});
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
TEST_CASE("tree width", "[WIDTH]")
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

TEST_CASE("BST delete", "[DEL]")
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

class LCA_BST
{
public:
	// 235. Lowest Common Ancestor of a Binary Search Tree
	// 236 is more general
	// find a node that is between p and 1, per definition of BST
	TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
		if (root->val > q->val && root->val > p->val)
			return lowestCommonAncestor(root->left, q, p);
		if (root->val < q->val && root->val < p->val)
			return lowestCommonAncestor(root->right, q, p);
		return root;
	}
};
TEST_CASE("235. Lowest Common Ancestor of BST", "[NEW]")
{
	TreeNode *r = TreeNode::CreateBinaryTree(vector<int>{2,1});
	CHECK(LCA_BST().lowestCommonAncestor(r, r, r->left)==r);
}
TEST_CASE("Lowest Common Ancestor", "[LCA]")
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


class TreeEasy
{
public:
	// 100. Same Tree
	bool isSameTree(TreeNode* p, TreeNode* q) { // in-order traversal, beat 100%
		if (p == nullptr && q == nullptr)
			return true;
		if (p == nullptr || q == nullptr || p->val != q->val)
			return false;
		return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
	}

	void dfs(TreeNode* r, vector<int>& leaf) {
		if (r == nullptr)
			return;
		if (!r->left && !r->right)
			leaf.push_back(r->val);
		else if (r->left && r->right) {
			dfs(r->left, leaf);
			dfs(r->right, leaf);
		}
		else if (r->left)
			return dfs(r->left, leaf);
		else
			dfs(r->right, leaf);
	}
	bool leafSimilar(TreeNode* root1, TreeNode* root2) {// beat 19%
		vector<int> leaf1;	dfs(root1, leaf1);
		vector<int> leaf2;	dfs(root2, leaf2);
		return leaf1 == leaf2;
	}
	//104. Maximum Depth of Binary Tree
	int maxDepth(TreeNode* root) {  // beat 98%
		if (!root)
			return 0;
		return 1 + max(maxDepth(root->left), maxDepth(root->right));
	}
	//111. Minimum Depth of Binary Tree
	int minDepth(TreeNode* root) {  // beat 20%
		if (!root)
			return 0;
		if (!root->left && !root->right)
			return 1;
		if (!root->left)
			return 1 + minDepth(root->right);
		if (!root->right)
			return 1 + minDepth(root->left);
		if (!root->left->left && !root->left->right) // left is leaf
			return 2;
		if (!root->right->left && !root->right->right) // right is leaf
			return 2; // optimization did not show visible speed up
		return 1 + min(minDepth(root->right), minDepth(root->left));
	}
	// 897. Increasing Order Search Tree
	TreeNode* increasingBST(TreeNode* root) {
		if (!root)
			return nullptr;
		TreeNode *left = root->left;
		TreeNode *head = increasingBST(root->left);
		if (!head)
			head = root;
		else {
			while (left->right)
				left = left->right;  // move to right most leaf node
			left->right = root;
		}
		root->left = nullptr;  // clean up left tree
		TreeNode *tail = increasingBST(root->right);
		root->right = tail;
		return head;
	}

	void dfsPaths(TreeNode* root, string target, vector<string>& result)
	{
		if (!root)
			return;
		if (!target.empty())
			target.append("->");
		target.append(to_string(root->val));
		if (!root->left && !root->right) {
			result.push_back(target);
			return;
		}
		if (root->left)
			dfsPaths(root->left, target, result);
		if (root->right)
			dfsPaths(root->right, target, result);
	}
	// 257. Binary Tree Paths
	vector<string> binaryTreePaths(TreeNode* root) {
		vector<string> res;
		dfsPaths(root, "", res);
		return res;
	}

	int height(TreeNode* root) {
		if (!root)
			return 0;
		int left = height(root->left);
		int right = height(root->right);
		if (left== INT32_MAX || right== INT32_MAX || abs(left - right) > 1) // INT32_MAX indicates not balanced
			return INT32_MAX;
		return max(left, right)+1;
	}
	// 110. Balanced Binary Tree
	bool isBalanced(TreeNode* root) { // beat 100%
		return height(root) != INT32_MAX;
	}
	
	bool isSymmetric(TreeNode* left, TreeNode *right) {
		if (left == nullptr || right == nullptr)
			return left==right;
		return left->val==right->val && isSymmetric(left->left, right->right) && isSymmetric(left->right, right->left); // subtree is symmetric
	}

	// 101. Symmetric Tree, node value symmetric around center
	bool isSymmetric(TreeNode* root) {
		//return root?isSymmetric(root->left, root->right):true; beat 13% recursive solution
		if (root == nullptr)  // iterative solution, beat 98%
			return true;
		deque<pair<TreeNode*, TreeNode*>> nodes;
		nodes.push_back({ root->left, root->right });
		while (!nodes.empty()) {
			auto p = nodes.front(); 
			nodes.pop_front();
			if (p.first == nullptr && p.second == nullptr)
				continue;
			if (p.first == nullptr || p.second == nullptr)
				return false;
			if (p.first->val != p.second->val)
				return false;
			nodes.push_back({ p.first->left,p.second->right });
			nodes.push_back({ p.first->right,p.second->left });
		}
		return true;
	}

	// 107. Binary Tree Level Order Traversal II
	vector<vector<int>> levelOrderBottom(TreeNode* root) { // BFS, beat 98%
		vector<vector<int>> ans = TreeNode::levelOrder(root);
		reverse(begin(ans), end(ans));
		return ans;
	}
	// 637. Average of Levels in Binary Tree, compute average at each level
	vector<double> averageOfLevels(TreeNode* root) { // similar to #107, watch for overflow
		vector<double> ans;
		if (root == nullptr)
			return ans;
		deque<TreeNode*> dq{ root };
		while (!dq.empty()) {
			size_t oldSize = dq.size();
			int64_t  total=accumulate(begin(dq), end(dq), (int64_t)0, [](int64_t init, const TreeNode* n) {
				return init+n->val;
			});
			ans.push_back((double)total/ oldSize);
			TreeNode::bfs(dq, oldSize);
		}
		return ans;
	}

	// 617. Merge Two Binary Trees
	TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {
		if (t1 == nullptr || t2 == nullptr) {
			return t1 == nullptr ? t2 : t1;
		}
		TreeNode * r = new TreeNode(t1->val + t2->val);
		r->left = mergeTrees(t1->left, t2->left);
		r->right = mergeTrees(t1->right, t2->right);
		return r;
	}
	// 226. Invert Binary Tree
	void InvertHelp(TreeNode* root)
	{
		if (!root)
			return;
		swap(root->left, root->right);
		InvertHelp(root->left);
		InvertHelp(root->right);
	}
	TreeNode* invertTree(TreeNode* root) {
		InvertHelp(root);
		return root;
	}
	// 404. Sum of Left Leaves
	bool isLeaf(TreeNode* r)
	{
		return r && r->left == nullptr && r->right == nullptr;
	}
	int sumOfLeftLeaves(TreeNode* root) { // beat 69%
		if (!root)
			return 0;
		if (isLeaf(root->left))
			return root->left->val + sumOfLeftLeaves(root->right);
		return sumOfLeftLeaves(root->left) + sumOfLeftLeaves(root->right);
	}
	int sumOfLeftLeaves_iterative(TreeNode* root) { // beat 69%
		deque<TreeNode*> dq{ root };
		int sum = 0;
		while (!dq.empty()) {
			TreeNode *n = dq.front();
			dq.pop_front();
			if (!n)
				continue;
			if (isLeaf(n->left))
				sum += n->left->val;
			else
				dq.push_back(n->left);
			dq.push_back(n->right);
		}
		return sum;
	}
	// 543. Diameter of Binary Tree
	// The diameter of a binary tree is the length of the longest path between any two nodes in a tree. 
	// This path may or may not pass through the root
	int maxDim = 0;
	int height_543(TreeNode* root)
	{
		if (!root)
			return 0;
		int left = height_543(root->left);
		int right = height_543(root->right);
		maxDim = max(left + right, maxDim);
		return max(left, right) + 1;
	}
	// idea: diameter at each root=sum of left and right height
	int diameterOfBinaryTree(TreeNode* root) { // beat 100%
		maxDim = 0;
		height_543(root);
		return maxDim;
	}

	tuple<int, int> findTiltHelp(TreeNode* root) {  // return {sum of at root, total_tilt}
		if (!root)
			return{ 0,0 };
		auto left = findTiltHelp(root->left);
		auto right = findTiltHelp(root->right);
		return{ root->val + get<0>(left) + get<0>(right), abs(get<0>(left) - get<0>(right))+ get<1>(left) + get<1>(right) };
	}
	// 563. Binary Tree Tilt, sum of all difference of left and right subtree
	int findTilt(TreeNode* root) {
		auto x= findTiltHelp(root);
		return get<1>(x);
	}

	// 671. Second Minimum Node In a Binary Tree, all values >=0
	// a node with has no or 2 children. if 2, root=min(left,right child)
	// means values of nodes at lower level is no less than the parent node
	int findSecondMinimumValue(TreeNode* root) {
		if (!root || root->left == nullptr || root->right == nullptr)
			return -1;
		int minSecond = INT32_MAX;
		if (root->right->val == root->val) {
			int x = findSecondMinimumValue(root->right);
			if (x > 0)
				minSecond = min(minSecond, x);
		}
		if (root->left->val == root->val) {
			int x = findSecondMinimumValue(root->left);
			if (x > 0)
				minSecond = min(minSecond, x);
		}
		if (root->left->val != root->right->val) {
			minSecond = min(minSecond, max(root->left->val, root->right->val));
		}
		return minSecond == INT32_MAX ? -1 : minSecond;
	}

	// 572. Subtree of Another Tree, t is same as a subtree of s
	bool isSubtree(TreeNode* s, TreeNode* t) {
		if (t == nullptr)
			return true;
		if (s == nullptr)
			return false;
		return isSameTree(s, t) || isSubtree(s->left, t) || isSubtree(s->right, t);
	}

	// at each node, calculate left and right, and compute max of left+right
	int DfsLength(TreeNode* root, int& maxPath) { // adapt calculating tree length
		if (!root)
			return 0;
		int left = 0, right = 0;
		if (root->left) {
			if (root->left->val == root->val)
				left = 1 + DfsLength(root->left, maxPath);  // same value
			else
				DfsLength(root->left, maxPath);  // different value, 
		}
		if (root->right) {
			if (root->right->val == root->val)
				right = 1 + DfsLength(root->right, maxPath);
			else
				DfsLength(root->right, maxPath);
		}
		maxPath = max(maxPath, left+right); // max path at root node
		return max(left, right); // max length of this subtree
	}
	// 687. Longest Univalue Path, similar to 543. Diameter of Binary Tree
	int longestUnivaluePath(TreeNode* root) {// beat 42%
		int longest = 0;
		DfsLength(root, longest);
		return longest;
	}
};

TEST_CASE("687. Longest Univalue Path", "[TREE]")
{
	auto *x = TreeNode::CreateBinaryTree({ 1,4,5,4,4,5 });
	CHECK(TreeEasy().longestUnivaluePath(x) == 2);
}

TEST_CASE("671. Second Minimum Node In a Binary Tree", "[TREE]")
{
	auto *x = TreeNode::CreateBinaryTree({ 1, 1, 3, 1, 1, 3, 4, 3, 1, 1, 1, 3, 8, 4, 8, 3, 3, 1, 6, 2, 1 });
	CHECK(TreeEasy().findSecondMinimumValue(x) == 2);
}
TEST_CASE("563. Binary Tree Tilt", "[TREE]")
{
	auto *x = TreeNode::CreateBinaryTree({ 1,2,3 });
	CHECK(TreeEasy().findTilt(x)==1);
	auto *x2 = TreeNode::CreateBinaryTree({ 5,5,8,3,5,8,10 });
	CHECK(TreeEasy().findTilt(x2) == 17);
}
TEST_CASE("637. Average of Levels in Binary Tree", "[TREE]")
{
	TreeNode * r1 = TreeNode::CreateBinaryTree(vector<int>{2147483647, 2147483647, 2147483647});
	CHECK(TreeEasy().averageOfLevels(r1) == vector<double>{2147483647.0, 2147483647.0});
}

TEST_CASE("107. Binary Tree Level Order Traversal II", "[TREE]")
{
	TreeNode * r1 = TreeNode::CreateBinaryTree(vector<int>{3, 9, 20, INT32_MIN, INT32_MIN, 15, 7});
	CHECK(TreeEasy().levelOrderBottom(r1) == vector<vector<int>>{ {15, 7}, { 9,20 }, { 3 }});
}

TEST_CASE("101. Symmetric Tree", "[TREE]")
{

	TreeNode * r2 = TreeNode::CreateBinaryTree(vector<int>{1,2,2, INT32_MIN,3,3});
	CHECK(TreeEasy().isSymmetric(r2) == true);
	TreeNode * r1 = TreeNode::CreateBinaryTree(vector<int>{1, 2, 3});
	CHECK(TreeEasy().isSymmetric(r1) == false);
}
TEST_CASE("110. Balanced Binary Tree", "[TREE]")
{
	TreeNode * r1 = TreeNode::CreateBinaryTree(vector<int>{3, 9, 20, INT32_MIN, INT32_MIN, 15, 7});
	CHECK(TreeEasy().isBalanced(r1) == true);
	TreeNode * r = TreeNode::CreateBinaryTree(vector<int>{1, 2, 2, 3, INT32_MIN, INT32_MIN, 3, 4, INT32_MIN, INT32_MIN, 4});
	CHECK(TreeEasy().isBalanced(r)==false);
}

TEST_CASE("872. Leaf-Similar Trees", "[TREE]")
{
	TreeNode * root1 = TreeNode::CreateBinaryTree(vector<int>{41, 62, INT32_MIN, 66, INT32_MIN, INT32_MIN, 21, 96, INT32_MIN, 70, 74});
	TreeNode * root2 = TreeNode::CreateBinaryTree(vector<int>{55, INT32_MIN, 84, INT32_MIN, 29, 116, INT32_MIN, 7, 74, INT32_MIN, 70});
	CHECK(TreeEasy().leafSimilar(root1, root2));
}
TEST_CASE("897. Increasing Order Search Tree", "[TREE]")
{
	TreeNode * root = TreeNode::CreateBinaryTree(vector<int>{5, 3, 6, 2, 4, INT32_MIN, 8, 1, INT32_MIN, INT32_MIN, INT32_MIN, 7, 9});
	TreeNode *t = TreeEasy().increasingBST(root);
	CHECK(TreeNode::heightR(t) == 9);
}

class CBTInserter {
	TreeNode *myRoot;
	deque<TreeNode*>	parents;
	deque<TreeNode*>	leaf_nodes;

public:
	CBTInserter(TreeNode* root):myRoot(root) {
		if (!root)
			return;
		parents.push_back(root);
		while (true) {
			for (const auto& p : parents) {  // traverse by level
				if (p->left)
					leaf_nodes.push_back(p->left);
				else
					return;
				if (p->right)
					leaf_nodes.push_back(p->right);
				else
					return;
			}
			parents.assign(begin(leaf_nodes), end(leaf_nodes));
			leaf_nodes.clear();
		}
	}

	int insert(int v) { // returns the value of the parent of the inserted
		if (myRoot == nullptr) {
			myRoot = new TreeNode(v);
			parents.push_back(myRoot);
			return -1;
		}
		if (leaf_nodes.size() == 2 * parents.size()) {
			parents.assign(begin(leaf_nodes), end(leaf_nodes));
			leaf_nodes.clear();
		}
		TreeNode * parent = parents.at(leaf_nodes.size() / 2);
		leaf_nodes.push_back(new TreeNode(v));
		if (leaf_nodes.size() % 2 == 0)
			parent->right = leaf_nodes.back();
		else
			parent->left = leaf_nodes.back();
		return parent->val;
	}

	TreeNode* get_root() {
		return myRoot;
	}
};

class ConstructBinaryTree
{

public:
	template<typename InIt>  // from root, find size of left and right subtree
	tuple<int, int> treePartition(int rootVal, InIt InFirst, InIt InStart, InIt InEnd, const unordered_map<int, int>& pos_map)
	{
		auto pos = pos_map.find(rootVal)->second;
		int left_size = distance(InStart, InFirst + pos);
		int right_size = distance(InFirst + pos + 1, InEnd);
		return{ left_size , right_size };
	}
	template<typename PreIt, typename InIt>
	TreeNode* buildTree(PreIt preStart, PreIt preEnd, InIt InFirst, InIt InStart, InIt InEnd, const unordered_map<int, int>& pos_map)
	{
		TreeNode * r = new TreeNode(*preStart);  // first value in preorder vec is the root
		int left_size, right_size;
		tie(left_size, right_size) = treePartition(*preStart, InFirst, InStart, InEnd, pos_map);
		r->left = left_size ? buildTree(preStart + 1, preStart + 1 + left_size, InFirst, InStart, InStart + left_size, pos_map) : nullptr;
		r->right = right_size ? buildTree(preStart + 1 + left_size, preEnd, InFirst, InStart + left_size + 1, InEnd, pos_map) : nullptr;
		return r;
	}

	// 105. Construct Binary Tree from Preorder and Inorder Traversal, no dup
	TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {  // beat 92%
		if (preorder.empty())
			return nullptr;
		unordered_map<int, int> inorder_pos = vector2map(inorder);
		return buildTree(preorder.begin(), preorder.end(), inorder.begin(), inorder.begin(), inorder.end(), inorder_pos);
	}

	template<typename PostRIT, typename InIt>
	TreeNode* buildTreePost(PostRIT pStart, PostRIT pEnd, InIt InFirst, InIt InStart, InIt InEnd, const unordered_map<int, int>& pos_map)
	{
		TreeNode * r = new TreeNode(*pStart);  // first value from back in postorder vec is the root
		int left_size, right_size;
		tie(left_size, right_size) = treePartition(*pStart, InFirst, InStart, InEnd, pos_map);

		// code is same as inorder buildTree, except how to calculate left and right side for postorder
		r->left = left_size ? buildTreePost(pStart + 1 + right_size, pEnd, InFirst, InStart, InStart + left_size, pos_map) : nullptr;
		r->right = right_size ? buildTreePost(pStart + 1, pStart + 1 + right_size, InFirst, InStart + left_size + 1, InEnd, pos_map) : nullptr;
		return r;
	}

	// 106. Construct Binary Tree from Inorder and Postorder Traversal, no dup
	// inorder = [9, 3, 15, 20, 7]
	// postorder = [9, 15, 7, 20, 3]
	TreeNode* buildTreePost(vector<int>& inorder, vector<int>& postorder) {  // beat 88%
		if (postorder.empty())
			return nullptr;
		unordered_map<int, int> inorder_pos = vector2map(inorder);
		return buildTreePost(postorder.rbegin(), postorder.rend(), inorder.begin(), inorder.begin(), inorder.end(), inorder_pos);
	}

	// 889. Construct Binary Tree from Preorder and Postorder Traversal
	// from preorder, first is root, second is root of subtree, then root of other subtree
	// from postorder, from right, first is root, second is root of right subtree, then root of left subtree
	template<typename PreIter, typename PostIter>
	TreeNode* constructFromPrePost(PreIter pre_first, PreIter pre_last, PostIter post_first, PostIter post_last, 
		unordered_map<int, vector<int>::const_iterator>& pre_pos_map) {
		if (pre_first == pre_last)
			return nullptr;
		TreeNode * root = new TreeNode(*pre_first);
		auto nodes = distance(++pre_first, pre_last);
		++post_first;  // done with root node
		if (nodes < 2 || *pre_first == *post_first) {  // first node in pre=first node in post
			root->left= constructFromPrePost(pre_first, pre_last, post_first, post_last, pre_pos_map); // one subtree, pick left
		}
		else {
			//auto left_end = find(pre_first, pre_last, *post_first);  // start of right subtree, O(n), slow
			auto left_end = pre_pos_map[*post_first];
			auto right_size = distance(left_end, pre_last);  // partition 2 sub tree
			root->left = constructFromPrePost(pre_first, left_end, post_first+ right_size, post_last, pre_pos_map);
			root->right = constructFromPrePost(left_end, pre_last, post_first, post_first + right_size, pre_pos_map);
		}
		return root;
	}
	TreeNode* constructFromPrePost(vector<int>& pre, vector<int>& post) {  // beat 53% without map
		//unordered_map<int, int> pre_pos_map = vector2map(pre);  // fast find of subtree size, beat 95%
		unordered_map<int, vector<int>::const_iterator> x = value_iter_map(pre);  // this line fail to compile in leetcode
		return constructFromPrePost(cbegin(pre), cend(pre), crbegin(post), crend(post), x);  // must use const iterator everywhere
	}
};


TEST_CASE("889. Construct Binary Tree from Preorder and Postorder Traversal", "[NEW]")
{
	TreeNode *r = ConstructBinaryTree().constructFromPrePost(vector<int>{1, 2, 4, 5, 3, 6, 7}, vector<int>{4, 5, 2, 6, 7, 3, 1});
	CHECK(TreeNode::levelOrder(r) == vector < vector<int>>{ {1}, { 2,3 }, { 4,5,6,7 }});
}
