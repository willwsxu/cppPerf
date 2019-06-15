#include "TreeNode.h"
#include <algorithm>
using namespace std;

// remove any node if sum of each root to leaf path is less than limit
// key insight: subtract root val from limit as it traverse deep, instead of sum
// root (none leaf) is invalid if none of its child is valid
TreeNode* sufficientSubset(TreeNode* root, long long limit) {
    if (!root)
        return nullptr;
    if (!root->left && !root->right)  // leaf node, test valid 
        return root->val < limit ? nullptr : root;

    // dfs, subtract current root val from limit
    if (root->left)
        root->left = sufficientSubset(root->left, limit - root->val);
    if (root->right)
        root->right = sufficientSubset(root->right, limit - root->val);
    return !root->left && !root->right ? nullptr : root; // if none of it children is valid, root is invalid
}

int max_sum = INT_MIN;
int maxPathSum_helper(TreeNode* root) {
    if (!root)
        return 0;
    if (!root->left && !root->right) {
        max_sum = max(max_sum, root->val);
        return root->val;
    }
    int left = maxPathSum_helper(root->left);
    int right = maxPathSum_helper(root->right);
    // two cases: this root is the best subtree
    // or this root is part of other bigger subtree. in this case, only use left ot right part
    int ret = max(left + root->val, right + root->val);
    ret = max(ret, root->val);
    max_sum = max(max_sum, max(ret, left + root->val + right));
    return  ret;
}

int maxPathSum(TreeNode* root) {
    max_sum = INT_MIN;
    maxPathSum_helper(root);
    return max_sum;
}

#include "catch.hpp"  // don't put this file in stdafx.h
TEST_CASE("124. Binary Tree Maximum Path Sum", "[TREE]")
{
    SECTION("edge case: best path is not through root") {
        auto t = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"-10", "9", "20", "null", "null", "15", "7"});
        CHECK(maxPathSum(t) == 42);
    }

    SECTION("edge case: single node") {
        auto t = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"0"});
        CHECK(maxPathSum(t) == 0);
    }
}
TEST_CASE("1080	Insufficient Nodes in Root to Leaf Paths", "[TREE]")
{
    SECTION("edge case: sum==limit is OK") {
        auto t = TreeNode::ConstructBinaryTreePerLevel(vector<string>{ "5", "4", "8", "11", "null", "17", "4", "7", "1", "null", "null", "5", "3"});
        auto result = sufficientSubset(t, 22);
        CHECK(TreeNode::levelOrder(result) == vector<vector<int>>{ {5}, { 4,8 }, { 11,17,4 }, { 7,5 }});
    }
    SECTION("edge case: root to leaf 1 2 -5, node 2 and -5 are invalid") {
        auto t = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"1", "2", "-3", "-5", "null", "4", "null"});
        auto result = sufficientSubset(t, -1);
        CHECK(TreeNode::levelOrder(result) == vector<vector<int>>{ {1}, { -3 }, { 4 }});
    }
    SECTION("all nodes are valid") {
        auto t = TreeNode::ConstructBinaryTreePerLevel(vector<string>{"10", "7", "2", "null", "5", "5", "10", "null", "6", "1", "null", "2", "null", "null", "null", "null", "null", "0"});
        auto result = sufficientSubset(t, 9);
        CHECK(TreeNode::levelOrder(result) == vector<vector<int>>{ {10}, { 7,2 }, { 5,5,10 }, { 6,1,2 }, { 0 }});
    }
}
