#include "TreeNode.h"
#include <algorithm>
using namespace std;

long long sufficientSubset_dfs(TreeNode* root, int limit, long long sum) {
    if (!root)
        return sum;
    auto left = sufficientSubset_dfs(root->left, limit, sum+root->val);
    auto right = sufficientSubset_dfs(root->right, limit, sum + root->val);
    if (root->left && (left < limit)) {
        root->left = nullptr;
        left = sum + root->val;
    }
    if (root->right && (right < limit )) {
        root->right = nullptr;
        right = sum + root->val;
    }
    return max(left, right);
}
TreeNode* sufficientSubset(TreeNode* root, int limit) {
    long long val = sufficientSubset_dfs(root, limit, 0);
    return val < limit ? nullptr : root;
}

#include "catch.hpp"  // don't put this file in stdafx.h
TEST_CASE("1080	Insufficient Nodes in Root to Leaf Paths", "[TREE]")
{
    auto t = TreeNode::ConstructBinaryTreePerLevel(vector<string>{ "5", "4", "8", "11", "null", "17", "4", "7", "1", "null", "null", "5", "3"});
    auto result = sufficientSubset(t, 22);
    CHECK(TreeNode::levelOrder(result) == vector<vector<int>>{ {5}, { 4,8 }, { 11,17,4 }, { 7,5 }});
    // edge case 1: sum can overflow long, use type long long
    // edge case 2: use LONG_MIN as invalid sum, 0 is not good as sum can be negative
}