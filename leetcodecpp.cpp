#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int findMaxForm(vector<string>& strs, int m, int n) {
	int **memo = new int*[m + 1];
	for (int i = 0; i <= m; i++) {
		memo[i] = new int[n + 1];
		for (int j = 0; j <= n; j++) {
			memo[i][j] = 0;
		}
	}
	for (auto i = 0; i < strs.size(); i++) {
		int ones = 0;
		int zero = 0;
		for (auto j = 0; j < strs[i].size(); j++) {
			if (strs[i][j] == '1')
				ones++;
			else
				zero++;
		}
		for (int j = m; j >= zero; j--) {
			for (int k = n; k >= ones; k--)
				memo[j][k] = max(memo[j][k], 1 + memo[j - zero][k - ones]);
		}
	}
	int ans = memo[m][n];
	delete[] memo;
	return ans;
}

#include <memory>

std::unique_ptr<int[]> dp;
int combinationSum4Dp(vector<int>& nums, int target) {
	if (dp[target] >= 0)
		return dp[target];
	int total = 0;
	for (auto val : nums) {
		if (target >= val)
			total += combinationSum4Dp(nums, target - val);
	}
	dp[target] = total;
	return total;
}

int combinationSum4(vector<int>& nums, int target) {
	dp = make_unique<int[]>(target + 1);
	std::fill(dp.get(), dp.get() + target + 1, -1);
	//for (int i=0; i<=target; i++)
	//    dp[i]=-1;
	dp[0] = 1;
	return combinationSum4Dp(nums, target);
}

class TrianglePath //120. Triangle
{
	int minimumTotal(vector<vector<int>>& triangle, int level, int pos, vector<vector<int>>& memo) {
		if (level == triangle.size() - 1)
			return triangle[level][pos];
		if (memo[level][pos] != INT_MAX)
			return memo[level][pos];
		int left = minimumTotal(triangle, level + 1, pos, memo);
		int right = minimumTotal(triangle, level + 1, pos + 1, memo);
		memo[level][pos] = std::min(left, right) + triangle[level][pos];
		return memo[level][pos];
	}
public:
	int minimumTotal(vector<vector<int>>& triangle) {
		vector<vector<int>> memo(triangle.size(), vector<int>(triangle.size(), INT_MAX));
		return minimumTotal(triangle, 0, 0, memo);
	}
	void test()
	{
		vector<vector<int>> triangle;
		triangle.push_back(vector<int>{ 2 });
		triangle.push_back(vector<int>{ 3, 4 });
		triangle.push_back(vector<int>{ 6, 5, 7 });
		triangle.push_back(vector<int>{ 4, 1, 8, 3 });
		printf("%d", minimumTotal(triangle));
	}
};

// 416. Partition Equal Subset Sum
bool canPartition(vector<int>& nums) {
	int sum = 0;
	for (int n : nums)
		sum += n;
	if ((sum & 1) > 0)  // odd sum
		return false;
	sum /= 2;
	vector<bool> dp(sum + 1, false);
	dp[0] = true;
	for (int n : nums) {
		if (sum >= n) {
			dp[sum] = dp[sum] || dp[sum - n];
			if (dp[sum])
				return true;
		}
		for (int j = sum - 1; j > 0; j--) {
			if (j >= n)
				dp[j] = dp[j] || dp[j - n];
		}
	}
	return dp[sum];
}


int findPaths(int m, int n, int N, int i, int j) {
	vector < vector<vector<int>>> dp3(2, vector<vector<int>>(m, vector<int>(n, 0)));
	const int MOD = 1000000007;
	int prev = 0;
	int current = 0;
	for (int x = 1; x <= N; x++)
	{
		current = 1 - prev;
		for (int r = 0; r < m; r++)
		{
			for (int c = 0; c < n; c++)
			{
				dp3[current][r][c] = (r == 0 ? 1 : dp3[prev][r - 1][c]) % MOD;  // up !ERROR don't use i-- etc
				dp3[current][r][c] += r == m - 1 ? 1 : dp3[prev][r + 1][c];  // down
				dp3[current][r][c] %= MOD;
				dp3[current][r][c] += c == 0 ? 1 : dp3[prev][r][c - 1];  // left
				dp3[current][r][c] %= MOD;
				dp3[current][r][c] += c == n - 1 ? 1 : dp3[prev][r][c + 1];  // left
				dp3[current][r][c] %= MOD;
			}
		}
		prev = current; // swap
	}
	return dp3[current][i][j];
}

#include <iostream>
void testEqualSumPartition()
{
	std::cout << canPartition(vector<int>{}) << endl;
	std::cout << canPartition(vector<int>{1, 5, 11, 5}) << endl;  // true
	std::cout << canPartition(vector<int>{1, 2, 3, 5}) << endl;   // false
}

// support negative num[i], even though current requirement is 
// 1 <= k <= len(nums) <= 16.
// 0 < nums[i] < 10000
bool dfs(vector<int>& nums, int k, vector<bool>& visited, int subsetSum, int target, int setIdx, int numIdx) {
	if (k == 1 && target != 0 || k == 0)
		return true;
	if (subsetSum == target && setIdx > 0)  // setIdx>0 is used to support target=0
		return dfs(nums, k - 1, visited, 0, target, 0, 0);
	for (int i = numIdx; i < nums.size(); i++) {
		if (visited[i] || subsetSum + nums[i] > target)
			continue;
		visited[i] = true;
		if (dfs(nums, k, visited, subsetSum + nums[i], target, setIdx + 1, i + 1))
			return true;
		visited[i] = false;
	}
	return false;
}
// 698. Partition to K Equal Sum Subsets
bool canPartitionKSubsets(vector<int>& nums, int k) {
	vector<bool> visited(nums.size(), false);
	int sum = 0;
	for (int n : nums)
		sum += n;
	if ((sum %k) > 0)  // not divisible
		return false;
	if (k < 1)
		return false;
	return dfs(nums, k, visited, 0, sum / k, 0, 0);
}

void testKpartition()
{
	std::cout << canPartitionKSubsets(vector<int>{4, 3, 2, 3, 5, 2, 1}, 4) << endl;  // true
	std::cout << canPartitionKSubsets(vector<int>{-1, 1, -1, 1, -1, 1, -1, 1, -1, 1}, 5) << endl;  // true
	std::cout << canPartitionKSubsets(vector<int>{-1, 1, -1, 1, -1, 1, -1, 1, -1, 1}, 6) << endl;  // false
	std::cout << canPartitionKSubsets(vector<int>{-1, 1, -1, 1, -1, 1, -1, 1, -1, 1}, 7) << endl;  // false
}

/* 650. 2 Keys Keyboard
Initially on a notepad only one character 'A' is present.You can perform two operations on this notepad for each step:
	Copy All : You can copy all the characters present on the notepad(partial copy is not allowed).
	Paste : You can paste the characters which are copied last time.
	The n will be in the range [1, 1000]
*/
// greedy methods, find the largest the factor, steps = s[factor]+n/factor
int minSteps(int n) {
	int s = 0;
	for (int factor = n / 2; factor >= 1; factor--) {
		if (n%factor > 0)
			continue;
		s += n / factor;
		n = factor;
		factor = factor / 2 + 1;
	}
	return s;
}


void testKey2()
{
	std::cout << ((minSteps(1) == 0) ? "true" : "false") << endl;
	std::cout << (minSteps(2) == 2) << endl;
	std::cout << (minSteps(3) == 3) << endl;
	std::cout << (minSteps(4) == 4) << endl;
	std::cout << (minSteps(8) == 6) << endl;
	std::cout << (minSteps(9) == 6) << endl;
	std::cout << (minSteps(12) == 7) << endl;
}

// prevBuy, last postion of buy, -1 means no
int maxProfit(vector<int>& prices, int pos, int prevBuy, vector<vector<int>>& dp) {
	if (pos >= prices.size()) {
		return 0;
	}
	if (dp[pos][prevBuy + 1] >= 0)
		return dp[pos][prevBuy + 1];
	int ans = maxProfit(prices, pos + 1, prevBuy, dp);  // no action
	if (prevBuy >= 0) {
		if (prices[pos] > prices[prevBuy])   // sell if there is profit
			ans = std::max(ans, (prices[pos] - prices[prevBuy]) + maxProfit(prices, pos + 2, -1, dp));
	}
	else
		ans = std::max(ans, maxProfit(prices, pos + 1, pos, dp));  // buy
	dp[pos][prevBuy + 1] = ans;
	return ans;
}
int maxProfit(vector<int>& prices) {
	int n = prices.size();
	vector<vector<int>> dp(n, vector<int>(n + 1, -1));
	return maxProfit(prices, 0, -1, dp);
}

void testStockBuySell()
{
	std::cout << (maxProfit(vector<int> { 1, 2, 3, 0, 2 }) == 3) << endl;
	std::cout << (maxProfit(vector<int> { 2, 1}) == 0) << endl;
}

#include <queue>
#include <unordered_set>
class DirectGraph {
	vector<vector<vector<int>>>	adjList;
	vector<int>			distTo;  // dist from dest back to src
	int dfsMin(int s, int dest, int level, int K) {
		if (level > K)
			return INT_MAX / 2;
		if (s == dest)
			return 0;
		if (level == K) {
			return INT_MAX / 2;
		}
		//if (distTo[s] < INT_MAX / 2)  // dfs works but too slow because of these two lines are not working
		//	return distTo[s];
		int ans = INT_MAX / 2;
		for (auto item : adjList[s]) {
			ans = std::min(ans, item[2] + dfsMin(item[1], dest, level + 1, K));
		}
		distTo[s] = ans;
		return ans;
	}

	void print(vector<int>& v)
	{
		for (int d : v)
			cout << d << " ";
		cout << endl;
	}

public:
	int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int K) {
		adjList.resize(n);
		for (auto item : flights) {
			adjList[item[0]].push_back(item);
		}
		distTo.resize(n, INT_MAX / 2);
		//distTo[dst] = 0;
		//int ans= dfsMin(src, dst, 0, K+1);  // fails some cases
		//return ans >= INT_MAX / 2 ? -1 : ans;
		
		// modified bfs, or bellman-ford, loop K+1 times
		distTo[src] = 0;  // distance from src to current node
		unordered_set<int> q{ src };
		for (int i = 0; i <= K && !q.empty(); i++) {
			vector<int> oldAns(distTo);   // make a copy of last values, or it won't work
			vector<int> last_q(begin(q), end(q));
			q.clear();
			for (int s : last_q) {
				for (auto f : adjList[s]) {
					distTo[f[1]] = std::min(distTo[f[1]], oldAns[s]+f[2]);
					if (f[1] != dst)
						q.insert(f[1]);
				}
			}
			//print(distTo);
		}
		return distTo[dst] >= INT_MAX / 2 ? -1 : distTo[dst];
	}
};

void testCheapestTicket()
{
	DirectGraph t[7];
	int ans = 0;

	ans = t[6].findCheapestPrice(5, vector < vector<int>>{ {0, 1, 5}, { 1,2,5 }, { 0,3,2 }, { 3,1,2 }, { 1,4,1 }, { 4,2,1 }}, 0, 2, 2); // 7
	cout << ans << endl;

	ans = t[5].findCheapestPrice(4, vector < vector<int>>{ {0, 1, 1}, { 0,2,5 }, { 1,2,1 }, { 2,3,1 }}, 0, 3, 1); // 6
	cout << ans << endl;

	ans = t[0].findCheapestPrice(3, vector < vector<int>>{ {0, 1, 100}, { 1,2,100 }, { 0,2,500 }}, 0, 2, 0);  // 500
	cout << ans << endl;

	ans = t[1].findCheapestPrice(3, vector < vector<int>>{ {0, 1, 100}, { 1,2,100 }, { 0,2,500 }}, 0, 2, 1); // 200
	cout << ans << endl;

	ans = t[2].findCheapestPrice(3, vector < vector<int>>{ {0, 1, 100}, { 1,2,100 }, { 0,2,500 }}, 1, 0, 1); // -1
	cout << ans << endl;

	ans = t[3].findCheapestPrice(4, vector < vector<int>>{ {0, 1, 100}, { 0,2,500 }}, 0, 3, 0); // -1
	cout << ans << endl;

	vector < vector<int>> flights{ { 3,4,4 },{ 2,5,6 },{ 4,7,10 },{ 9,6,5 },{ 7,4,4 },{ 6,2,10 },{ 6,8,6 },{ 7,9,4 },{ 1,5,4 },{ 1,0,4 },{ 9,7,3 },{ 7,0,5 },{ 6,5,8 },{ 1,7,6 },{ 4,0,9 },{ 5,9,1 },{ 8,7,3 },{ 1,2,6 },{ 4,1,5 },{ 5,2,4 },{ 1,9,1 },{ 7,8,10 },{ 0,4,2 },{ 7,2,8 } };
	ans = t[4].findCheapestPrice(10, flights, 6, 0, 7); // 14
	cout << ans << endl;
}


struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int x) : val(x), left(NULL), right(NULL) {}
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
	vector<int> ans= t.largestValues(tn);
	for_each(begin(ans), end(ans), [](auto i) { cout << i << " ";});
}

//542. 01 Matrix
// Given a matrix consists of 0 and 1, find the distance of the nearest 0 for each cell
// The number of elements of the given matrix will not exceed 10,000.
// There are at least one 0 in the given matrix.
// The cells are adjacent in only four directions : up, down, left and right.
class Matrix {
public:
	vector<vector<int>> updateMatrix(vector<vector<int>>& matrix) { // bfs is slow, beat 42%
		int m = matrix.size();
		if (m == 0)
			return matrix;
		int n = matrix[0].size();
		queue<vector<int>> q;
		for (int i = 0; i < m; i++) {			
			for (int j = 0; j < n; j++) {
				if (matrix[i][j] == 0)
					q.push(vector<int>{i, j});
				else
					matrix[i][j] = INT_MAX;
			}
		}
		vector<vector<int>> direction{ { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 } };
		int dist = 0;
		while (!q.empty()) {
			dist++;
			int oldSize = q.size();
			for (int i = 0; i < oldSize; i++) {
				vector<int>& cell = q.front();
				for (auto& d : direction) {
					int r = cell[0] + d[0];
					int c = cell[1] + d[1];
					if (r < 0 || c < 0 || r >= m || c >= n)
						continue;
					if (matrix[r][c] != INT_MAX)
						continue;
					matrix[r][c] = dist;
					q.push(vector<int>{r, c});
				}
				q.pop();
			}
		}
		return matrix;
	}

	vector<vector<int>> updateMatrix2(vector<vector<int>>& matrix) { // 2 sweep, beat93%
		int m = matrix.size();
		if (m == 0)
			return matrix;
		int n = matrix[0].size();
		int MY_MAX = INT_MAX / 2;
		vector<vector<int>> ans(m, vector<int>(n, MY_MAX));
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (matrix[i][j] == 0)
					ans[i][j] = 0;
				else {
					ans[i][j] = MY_MAX;
					if (i>0)
						ans[i][j] = std::min(ans[i][j], ans[i-1][j]+1);  // check top cell
					if (j>0)
						ans[i][j] = std::min(ans[i][j], ans[i][j-1]+1);  // check left cell
				}
			}
		}
		for (int i = m-1; i >=0; i--) {
			for (int j = n-1; j >=0; j--) {
				if (i<m-1)
					ans[i][j] = std::min(ans[i][j], ans[i + 1][j]+1);  // check bottom cell
				if (j<n-1)
					ans[i][j] = std::min(ans[i][j], ans[i][j + 1]+1);  // check right cell
			}
		}
		return ans;
	}
};

auto print = [](vector<vector<int>>& ans) {
	for (auto& v : ans) {
		for_each(begin(v), end(v), [](auto i) { cout << i << " ";});
		cout << endl;
	}
	cout << endl;
};
void testMatrix()
{
	Matrix m;

	vector<vector<int>>& ans = m.updateMatrix(vector<vector<int>>{ {0, 0, 0}, { 0,1,0 }, { 1,1,1 }});

	print(ans);

	vector<vector<int>>& ans2 = m.updateMatrix2(vector<vector<int>>{ {0, 0, 0}, { 0,1,0 }, { 1,1,1 }});
	print(ans2);

	vector<vector<int>> matrix{ {1, 0, 1, 1, 0, 0, 1, 0, 0, 1}, { 0,1,1,0,1,0,1,0,1,1 }, { 0,0,1,0,1,0,0,1,0,0 }, { 1,0,1,0,1,1,1,1,1,1 }, { 0,1,0,1,1,0,0,0,0,1 }, { 0,0,1,0,1,1,1,0,1,0 }, { 0,1,0,1,0,1,0,0,1,1 }, { 1,0,0,0,1,2,1,1,0,1 }, { 2,1,1,1,1,2,1,0,1,0 }, { 2,2,2,1,0,1,0,0,1,1 } };
	vector<vector<int>>& ans3 = m.updateMatrix2(matrix);
	print(ans3);
}

/* Given an m x n matrix of non - negative integers representing the height of each unit cell in a continent, 
 * the "Pacific ocean" touches the left and top edges of the matrix and the "Atlantic ocean" touches the right and bottom edges.
 * Water can only flow in four directions(up, down, left, or right) from a cell to another one with height equal or lower.
 * Find the list of grid coordinates where water can flow to both the Pacific and Atlantic ocean.
*/
// 417. Pacific Atlantic Water Flow
// Both m and n are less than 150
class WaterFlow { // borrow 2 sweep ideas from matrix above.
public:
	void dfs(vector<vector<int>>& matrix, int r, int c, int m, int n, vector<vector<bool>>& vis) {
		static vector<vector<int>> direction{ { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 } };
		vis[r][c] = true;
		for (vector<int> d : direction) {
			int y = r + d[0];
			int x = c + d[1];
			if (y < 0 || x < 0 || y >= m || x >= n)
				continue;
			if (vis[y][x])
				continue;
			if(matrix[y][x]>=matrix[r][c])
				dfs(matrix, y, x, m, n, vis);
		}
	}
	vector<pair<int, int>> pacificAtlantic(vector<vector<int>>& matrix) {
		vector<pair<int, int>> ans;
		int m = matrix.size();
		if (m == 0)
			return ans;
		int n = matrix[0].size();
		vector<vector<bool>> pacific(m, vector<bool>(n, false));  // find all cells flow to pacific
		vector<vector<bool>> atlantic(m, vector<bool>(n, false)); // find all cells flow to atlantic
		for (int i = 0; i < n; i++) {
			dfs(matrix, 0, i, m, n, pacific);    // first row
			dfs(matrix, m-1, i, m, n, atlantic); // last row
		}
		for (int i = 0; i < m; i++) {
			dfs(matrix, i, 0, m, n, pacific);   // first column
			dfs(matrix, i, n-1, m, n, atlantic);   // last column
		}
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (pacific[i][j] && atlantic[i][j])
					ans.push_back(pair<int, int>{i, j});
			}
		}
		/*
		auto print2 = [](vector<vector<bool>>& ans) {
			for (auto& v : ans) {
				for_each(begin(v), end(v), [](auto i) { cout << (i?"Y":"N") << " ";});
				cout << endl;
			}
			cout << endl;
		};
		print2(pacific);
		print2(atlantic);*/
		return ans;
	}
};

void testWaterFlow()
{
	WaterFlow w;
	vector<pair<int, int>> ans=w.pacificAtlantic(vector<vector<int>>{ {1,2,2,3,5}, {3,2,3,4,4}, {2,4,5,3,1}, {6,7,1,4,5}, {5,1,1,2,4}});
	for_each(begin(ans), end(ans), [](auto p) {cout << "[" << p.first << "," << p.second << "] ";});
	cout << endl;  // [[0, 4], [1, 3], [1, 4], [2, 2], [3, 0], [3, 1], [4, 0]]
	ans = w.pacificAtlantic(vector<vector<int>>{ {1}});
	for_each(begin(ans), end(ans), [](auto p) {cout << "[" << p.first << "," << p.second << "] ";});
	cout << endl;  // [0,0]
	ans = w.pacificAtlantic(vector<vector<int>>{ {1, 2, 3}, {8,9,4}, {7,6,5}});  // can move right then up, 2 sweep method fail this case
	for_each(begin(ans), end(ans), [](auto p) {cout << "[" << p.first << "," << p.second << "] ";}); //[[0,2],[1,0],[1,1],[1,2],[2,0],[2,1],[2,2]]
 }

#include <random>
#include <cmath>
// 215. Kth Largest Element in an Array, unsorted
// 1 ≤ k ≤ array's length
class SortRandom {
	int partition(vector<int>& nums, int i, int j)
	{
		int pivot = i;
		while (++i < j) {
			if (nums[i] >= nums[pivot]) { // find number larger or equal
				while (j > i) {
					if (nums[j] >= nums[pivot])
						j--;
					else
						break;  // find a number smaller
				}
				if (i < j)
					std::swap(nums[i], nums[j]);
				else {
					i--;
					break;
				}
			}
		}
		if (nums[i] < nums[pivot])
			std::swap(nums[i], nums[pivot]);
		else if (i > pivot + 1)
			std::swap(nums[--i], nums[pivot]); //move i 1 back
		else if (i>pivot)
			i--;
		return i;
	}
public:
	int findKthLargest(vector<int>& nums, int k) {
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		//std::uniform_int_distribution<> dis(0, nums.size()-1);
		shuffle(begin(nums), end(nums), gen);
		// pivot on value on low
		int low = 0; int high = nums.size()-1;
		while (low < high) {
			int i = partition(nums, low, high);	// i is the low bound of value nums[i]
			if (nums.size() - i == k)
				return nums[i];
			else if (nums.size() - i > k) {  // answer lies to right
				low = i + 1;
			}
			else {
				high = i - 1;
			}
		}
		return nums[low];
	}
};


void test()
{
	SortRandom s;
	cout << s.findKthLargest(vector<int>{3, 2, 1, 5, 6, 4}, 2) << endl;
	cout << s.findKthLargest(vector<int>{3, 2, 1, 5, 6, 4}, 1) << endl;
	cout << s.findKthLargest(vector<int>{3, 2, 1, 5, 6, 4}, 6) << endl;
	cout << s.findKthLargest(vector<int>{3}, 1) << endl;
}