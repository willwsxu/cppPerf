#include "TreeNode.h"
#include <algorithm>
using namespace std;

int sufficientSubset_dfs(TreeNode* root, int limit) {
    if (!root)
        return 0;
    int left = sufficientSubset_dfs(root->left, limit);
    int right = sufficientSubset_dfs(root->right, limit);
    if (root->left && (left < limit ||left+root->val<limit)) {
        root->left = nullptr;
        left = 0;
    }
    if (root->right && (right < limit || right+root->val<limit)) {
        root->right = nullptr;
        right = 0;
    }
    int val = 0;
    if (root->left && root->right)
        val = min(left, right);
    else if (root->left)
        val = left;
    else if (root->right)
        val = right;
    return root->val + val;
}
TreeNode* sufficientSubset(TreeNode* root, int limit) {
    int val = sufficientSubset_dfs(root, limit);
    return val < limit ? nullptr : root;
}

#include "catch.hpp"  // don't put this file in stdafx.h
TEST_CASE("1080	Insufficient Nodes in Root to Leaf Paths", "[TREE]")
{
    auto t = TreeNode::ConstructBinaryTreePerLevel(vector<string>{ "5", "4", "8", "11", "null", "17", "4", "7", "1", "null", "null", "5", "3"});
    auto result = sufficientSubset(t, 22);
}