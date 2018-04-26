#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
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
#include <iterator>
#include <iostream>

void print(vector<int>& v)
{
	std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
	cout << endl;
}

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
	print(ans);
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

auto printvv = [](vector<vector<int>>& ans) {
	for (auto& v : ans) {
		//for_each(begin(v), end(v), [](auto i) { cout << i << " ";});
		std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
		cout << endl;
	}
	cout << endl;
};
void testMatrix()
{
	Matrix m;

	vector<vector<int>>& ans = m.updateMatrix(vector<vector<int>>{ {0, 0, 0}, { 0,1,0 }, { 1,1,1 }});

	printvv(ans);

	vector<vector<int>>& ans2 = m.updateMatrix2(vector<vector<int>>{ {0, 0, 0}, { 0,1,0 }, { 1,1,1 }});
	printvv(ans2);

	vector<vector<int>> matrix{ {1, 0, 1, 1, 0, 0, 1, 0, 0, 1}, { 0,1,1,0,1,0,1,0,1,1 }, { 0,0,1,0,1,0,0,1,0,0 }, { 1,0,1,0,1,1,1,1,1,1 }, { 0,1,0,1,1,0,0,0,0,1 }, { 0,0,1,0,1,1,1,0,1,0 }, { 0,1,0,1,0,1,0,0,1,1 }, { 1,0,0,0,1,2,1,1,0,1 }, { 2,1,1,1,1,2,1,0,1,0 }, { 2,2,2,1,0,1,0,0,1,1 } };
	vector<vector<int>>& ans3 = m.updateMatrix2(matrix);
	printvv(ans3);
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
class SortRandom {  // randomized selection sort
	int partition(vector<int>& nums, int low, int hi)
	{
		int pivot = low;
		int& pV = nums[low];
		while (++low < hi) {
			if (nums[low] >= pV) { // find number larger or equal
				do {
					if (nums[hi] < pV)
						break;  // find a number smaller
				} while (--hi > low);
				if (low < hi)
					std::swap(nums[low], nums[hi]);
				else {
					low--;
					break;
				}
			}
		}
		if (nums[low] < pV)
			std::swap(nums[low], pV);
		else if (low > pivot + 1)
			std::swap(nums[--low], pV); //move low 1 back
		else if (low>pivot)
			low--;
		return low;
	}
public:
	int findKthLargest_old(vector<int>& nums, int k) {
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		//std::uniform_int_distribution<> dis(0, nums.size()-1);
		shuffle(begin(nums), end(nums), gen);   // add shuffling improve perforamnce from 23 ms to 12ms
		// pivot on value on low
		int low = 0; int high = nums.size()-1;
		int target = nums.size() - k;
		while (low < high) {
			int pivot = partition(nums, low, high);	// lower bound value at pivot
			if (pivot == target)
				return nums[pivot];
			else if (pivot < target) {  // answer lies to right
				low = pivot + 1;
			}
			else {
				high = pivot - 1;
			}
		}
		return nums[low];
	}

	int findKthLargest(vector<int>& nums, int k) {  // use stl partition, same speed as my own partition, shuffling helps
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		shuffle(begin(nums), end(nums), gen);   // add shuffling improve perforamnce from 23 ms to 12ms
		auto low = begin(nums);
		auto high = end(nums)-1;
		auto target = end(nums) - k;
		while (low < high) {
			int val = *low;
			auto pred = [val](auto& item) {return item < val; };
			std::partition(low, high+1, pred);
			auto pivot = partition_point(low, high + 1, pred);  // pivot value is not same as val
			if (pivot == target) {
				return val;
			}
			else if (pivot < target) {
				if (*pivot == val)  // trick cdoe, stop infinite loop
					low = pivot + 1;
				else
					low = pivot;
			}
			else
				high = pivot;  // don't use pivot -1 as the desired value can on on this point
		}
		return *low;
	}
	int findKthLargest_stl(vector<int>& nums, int k) {
		nth_element(begin(nums), begin(nums) + k-1, end(nums), greater<int>());  // stl method works great, beat 98%, don't add shuffle
		return *(begin(nums) + k-1);
	}
};


void testKthLargest()
{
	SortRandom s;
	cout << s.findKthLargest(vector<int>{7, 6, 5, 4, 3, 2, 1}, 5) << " exp " << 3 << endl;
	cout << s.findKthLargest(vector<int>{3, 2, 1, 5, 6, 4}, 2) << " exp " << 5 << endl;
	cout << s.findKthLargest(vector<int>{3, 2, 1, 5, 6, 4}, 1) << " exp " << 6 << endl;
	cout << s.findKthLargest(vector<int>{3, 2, 1, 5, 6, 4}, 6) << " exp " << 1 << endl;
	cout << s.findKthLargest(vector<int>{3}, 1) << " exp " << 3 << endl;
}

#include <sstream>
#include <iterator>
#include <iostream>
class NQueens {
	class DiagMask
	{
	protected:
		vector<bool> mask;
		DiagMask(int n):mask(2*n-1,false)
		{

		}
		bool get(int idx) {
			if (idx < mask.size())
				return mask[idx];
			return false;
		}
		void set(int idx, bool v)
		{
			if (idx < mask.size())
				mask[idx] = v;
		}
	};
	class ForwardDiagMask : public DiagMask
	{
	public:
		ForwardDiagMask(int n) :DiagMask(n)
		{
		}
		void set(int r, int c) {
			DiagMask::set(r + c, true);
		}
		void reset(int r, int c) {
			DiagMask::set(r + c, false);
		}
		bool get(int r, int c)	{
			return DiagMask::get(r + c);
		}
	};
	class BackwardDiagMask : public DiagMask
	{
		int offset;
	public:
		BackwardDiagMask(int n) :DiagMask(n), offset(n - 1)
		{
		}
		void set(int r, int c) {
			DiagMask::set(offset+r - c, true);
		}
		void reset(int r, int c) {
			DiagMask::set(offset + r - c, false);
		}
		bool get(int r, int c) {
			return DiagMask::get(offset + r - c);
		}
	};
	void backtracking(vector<vector<string>>& ans, int n, vector<int>& rows, vector<bool>& mCol, ForwardDiagMask& fwd, BackwardDiagMask& bwd)
	{
		if (rows.size() == n) {
			vector<string> board;
			for (int c : rows) {
				string r(n, '.');     // default row config
				r[c] = 'Q';					// set Q position
				board.push_back(r);  // speed up to 6ms from 10ms after switch to use string, instead of vector to ostream 
			}
			ans.push_back(board);
			return;
		}
		int r = rows.size();
		for (int c = 0; c < n; c++) {  // try each col
			if (mCol[c] || fwd.get(r, c) || bwd.get(r, c))  // invalid
				continue;
			mCol[c] = true;  // set masks for row, and 2 diagonals
			fwd.set(r, c);
			bwd.set(r, c);
			rows.push_back(c); // add col value for this row
			backtracking(ans, n, rows, mCol, fwd, bwd);
			mCol[c] = false;   // reset all states
			fwd.reset(r, c);
			bwd.reset(r, c);
			rows.pop_back();
		}
	}

	int backtrackingCount(int r, vector<bool>& mCol, ForwardDiagMask& fwd, BackwardDiagMask& bwd)
	{
		if (r<0) {
			return 1;
		}
		int ans = 0;
		for (int c = 0; c < mCol.size(); c++) {  // try each col
			if (mCol[c] || fwd.get(r, c) || bwd.get(r, c))  // invalid
				continue;
			mCol[c] = true;  // set masks for row, and 2 diagonals
			fwd.set(r, c);
			bwd.set(r, c);
			ans += backtrackingCount(r-1, mCol, fwd, bwd);
			mCol[c] = false;   // reset all states
			fwd.reset(r, c);
			bwd.reset(r, c);
		}
		return ans;
	}
public:
	vector<vector<string>> solveNQueens(int n) {  // beat 74%
		vector<int> rows;  // rows of the board, its value is the column at each row
		vector<bool> mCol(n, false);
		ForwardDiagMask fwd(n);
		BackwardDiagMask bwd(n);
		vector<vector<string>> ans;
		backtracking(ans, n, rows, mCol, fwd, bwd);
		return ans;
	}
	int totalNQueens(int n) {  // beat 88%
		vector<bool> mCol(n, false);
		ForwardDiagMask fwd(n);
		BackwardDiagMask bwd(n);
		return  backtrackingCount(n-1, mCol, fwd, bwd);
	}
};


void testNQueens()
{
	NQueens nq;
	vector<vector<string>> ans=nq.solveNQueens(4);
	for (auto& v : ans) {
		copy(begin(v), end(v), std::ostream_iterator<string>(cout, "\n"));
		cout << endl;
	}
	cout << nq.totalNQueens(8) << endl;  // 92
}

class WordDictionary {
	struct Node
	{
		char letter;	// lower case only
		bool complete;  // mark work is complete
		vector<Node *> next;
		Node(char c) : next(26, nullptr), letter(c), complete(false){}
		Node *& operator [](int idx) {
			return next[idx];
		}
		void put(const string& word, int pos) {
			int idx = word[pos] - 'a';
			if ( next[idx]==nullptr )
				next[idx] = new Node(word[pos]);
			if (pos == word.length() - 1)
				next[idx]->complete = true;
			else
				next[idx]->put(word, pos + 1);
		}
		bool find(const string& word, int pos)
		{
			if (word[pos] == '.') {  // wild char matching
				for (Node* c : next) {
					if (c == nullptr)
						continue;
					if (pos == word.length() - 1) {
						if (c->complete)
							return true;
					}
					else if (c->find(word, pos + 1))
						return true;
				}
				return false;
			}
			int idx = word[pos] - 'a';
			if (next[idx] == nullptr)
				return false;
			if (pos == word.length() - 1)
				return next[idx]->complete;
			return next[idx]->find(word, pos + 1);
		}
	};
	class DictTrie
	{
		Node root;
	public :
		DictTrie() :root(0) {}
		void put(const string& word)
		{
			if (!word.empty())
				root.put(word, 0);
		}
		bool find(const string& word)
		{
			if (word.empty())
				return true;
			return root.find(word,0);
		}
	};
	//unordered_set<string> dict;
	DictTrie	dict;
public:
	/** Initialize your data structure here. */
	WordDictionary(){

	}

	/** Adds a word into the data structure. */
	void addWord(string word) {
		//dict.insert(word);
		dict.put(word);
	}

	/** Returns if the word is in the data structure. A word could contain the dot character '.' to represent any one letter. */
	bool search(string word) {
		return dict.find(word);/*
		auto pred = [&word](auto item) {
			if (item.length() != word.length())
				return false;
			return equal(begin(item), end(item), begin(word), [](auto a, auto b) {return (b == '.' || a == b);});
		};
		return find_if(begin(dict), end(dict), pred) !=end(dict);*/
	}
};

/**
* Your WordDictionary object will be instantiated and called as such:
* WordDictionary obj = new WordDictionary();
* obj.addWord(word);
* bool param_2 = obj.search(word);
*/
#include <cctype>
void testWordSearch()
{
	WordDictionary d;
	d.addWord("bad");
	d.addWord("dad");
	d.addWord("mad");
	cout << (d.search("pad")==false) << endl;
	cout << d.search("bad") << endl;
	cout << d.search(".ad") << endl;
	cout << d.search("b..") << endl;
	cout << (d.search("ba")==false) << endl;
	cout << (d.search(".")==false) << endl;
}

#include <numeric>

// 526. Beautiful Arrangement
// Give array of integers 1 to N, arrange with either of following is true, for each position i, 1 <= i <= N
// 1. The number at the ith position is divisible by i
// 2. i is divisible by the number at the ith position
// N is a positive integer and will not exceed 15
class Arrangement {
	int backtracking(int N, int mask, int pos)
	{
		if (pos < 1)
			return 1;
		int count = 0;
		//for (int v = 1; v <= N; v++) {
		for (int v = N; v >= 1; v--) {
			int m = 1 << (v - 1);
			if ((m&mask) > 0)  // number is already used
				continue;
			if (v%pos > 0 &&    // break rule 1
				pos%v > 0)   // break rule 2
				continue;
			count += backtracking(N, mask|m, pos - 1);
		}
		return count;
	}
	int backtracking2(int N, vector<int>& arr, int pos) // use vector and swap to store remaining #s is faster, 7ms vs 16s, beat 92%
	{
		if (pos < 1)
			return 1;
		int count = 0;
		for (int v = pos-1; v >= 0; v--) {
			if (arr[v]%pos > 0 &&    // break rule 1
				pos%arr[v] > 0)   // break rule 2
				continue;
			if (v!=pos-1)
				std::swap(arr[v],arr[pos-1]);
			count += backtracking2(N, arr, pos - 1);
			if (v != pos - 1)
				std::swap(arr[v], arr[pos - 1]);
		}
		return count;
	}
public:
	int countArrangement(int N) {
		//return backtracking(N, 0, N);  // big performance boost after reverse position, from 93 to 16ms
		vector<int> arr(N, 0);
		std::iota(arr.begin(), arr.end(), 1);
		return backtracking2(N, arr, N);
	}
};

void testArrangement()
{
	Arrangement s;
	cout << s.countArrangement(2) << endl; // 2
	cout << s.countArrangement(10) << endl; // 700
}

// 131. Palindrome Partitioning
// Given a string s, partition s such that every substring of the partition is a palindrome.
// Return all possible palindrome partitioning of s.
class Palindrome {
	bool palindrome(const string& s, int start, int end)  // inclusive
	{
		while (start < end) {
			if (s[start] != s[end])
				return false;
			start++;
			end--;
		}
		return true;
	}
	void backtracking(vector<vector<string>> &ans, vector<string>& palin, const string& s, int idx)
	{
		if (idx == s.length()) {
			ans.push_back(palin);
			return;
		}
		for (int i = idx; i < s.length(); i++) {
			if (palindrome(s, idx, i)) {
				palin.push_back(s.substr(idx, i-idx+1));
				backtracking(ans, palin, s, i + 1);
				palin.pop_back();
			}
		}
	}
public:
	vector<vector<string>> partition(string s) {
		vector<vector<string>> ans{};
		vector<string> palin{};
		backtracking(ans, palin, s, 0);
		return ans;
	}
};


void testPalindromePart()
{
	Palindrome p;
	vector<vector<string>> ans=p.partition("aab");
	for (vector<string>& v : ans) {
		copy(begin(v), end(v), std::ostream_iterator<string>(cout, " "));
		cout << endl;
	}
}

//452. Minimum Number of Arrows to Burst Balloons, similar to scheduling problem
// similar to 435. Non-overlapping Intervals
class Balloons {
public:
	int findMinArrowShots_old(vector<pair<int, int>>& points) {
		if (points.size() < 2)
			return points.size();
		std::sort(begin(points), end(points), [](auto p1, auto p2) {return p1.first < p2.first;}); // sort by start interval
		int arrows = 1;
		int rigthEnd = points[0].second;
		for (auto it = begin(points) + 1; it < end(points); it++) {
			if (it->first > rigthEnd) { // no overlapping
				arrows++;
				rigthEnd = it->second;
			}
			else if (it->second < rigthEnd) {
				rigthEnd = it->second;  // adjust overlapping range (critical step)
			}
		}
		return arrows;
	}
	int findMinArrowShots(vector<pair<int, int>>& points) {// same as scheduling except does not allow edge case when end meets next start, beat 98%
		if (points.size() < 2)
			return points.size();
		std::sort(begin(points), end(points), [](auto p1, auto p2) {return p1.second < p2.second;}); // sort by end interval
		int arrows = 1;
		int rigthEnd = points[0].second;
		for (auto it = begin(points) + 1; it < end(points); it++) {
			if (it->first > rigthEnd) { // no overlapping
				arrows++;
				rigthEnd = it->second;
			}
		}
		return arrows;
	}
};

void testBaloons()
{
	Balloons t;
	cout << t.findMinArrowShots(vector<pair<int, int>>{ {1, 2}, { 2,3 }, { 3,4 }, { 4,5 }}) << endl;  // 2

	cout << t.findMinArrowShots(vector<pair<int, int>>{ {3, 9}, { 7,12 }, { 3,8 }, { 6,8 }, { 9,10 }, { 2,9 }, { 0,9 }, { 3,9 }, { 0,6 }, { 2,8 }}) << endl;  // 2
}

// check digit from left to right, find first place where next digit is smaller
// travel back to find digit of same value
// set it to one less, set rest of digits to 9
class Monotone {
public:
	int monotoneIncreasingDigits(int N) {
		if (N < 10)
			return N;
		vector<int> digits{};
		digits.reserve(10);
		while (N > 0) {
			digits.emplace_back(N % 10);  // insert digits backwards
			N /= 10;
		}
		for (auto d = rbegin(digits); d < rend(digits)-1; d++) {
			if (*d > *(d + 1)) {
				while (d > rbegin(digits)) {  // move back to find digit of same value
					if (*(d - 1) == *d)
						d--;
					else
						break;
				}
				(*d)--;  // borrow 1, and set the rest as 9
				while (++d < rend(digits))
					*d = 9;
				break;
			}
		}
		auto b = rbegin(digits);
		if (*b == 0)  // skip leading 0
			b++;
		return accumulate(b, rend(digits), 0, [](auto last, auto cur) { return last * 10 + cur;});
	}
};

void testDigits()
{
	Monotone t;
	cout << t.monotoneIncreasingDigits(120) << endl;
	cout << t.monotoneIncreasingDigits(10) << endl;
	cout << t.monotoneIncreasingDigits(1234) << endl;
	cout << t.monotoneIncreasingDigits(332) << endl;
	cout << t.monotoneIncreasingDigits(100) << endl;
	cout << t.monotoneIncreasingDigits(3311322) << endl;
}

// 659. Split Array into Consecutive Subsequences
// each subsequences consist of at least 3 consecutive integers
class Consecutive {
public:
	bool isPossible(vector<int>& nums) {

	}
};

class Greedy {
public:
	//Given a string S, check if the letters can be rearranged so that two characters that are adjacent to each other are not the same.
	string reorganizeString(string S) { // beat 94%
		vector<int> count(26);
		for_each(begin(S), end(S), [&count](char x) {count[x - 'a']++;});
		auto _max = max_element(begin(count), end(count));
		int total = accumulate(begin(count), end(count), 0);
		if (total < *_max * 2 - 1)
			return "";
		string ans(*_max, _max - begin(count) + 'a');  // construct the longest char
		int fill = 0;
		int dist = 2;  // of same char
		for (auto it = begin(count); it != end(count); it++) {
			if (*it == 0 || it == _max)
				continue;
			char ch = it - begin(count) + 'a';
			for (int j = 0; j < *it; j++) {
				ans.insert((++fill)* dist - 1, 1, ch);  // formular to get insert position
				if (fill == *_max) {  // next line
					fill = 0;
					dist++;
				}
			}
		}
		return ans;
	}

	// Given a non - negative integer num represented as a string, remove k digits from the number so that the new number is the smallest possible.
	string removeKdigits(string num, int k) {
		int pos = 0; // position to remove
		for (int i = 0; i < k; i++) {
			while (num[pos] == '0')  // find next none 0
				num.erase(pos, 1);
			int newK = k - i;
			if (pos + newK >= num.size())  // remove all
				return "0";
			while (pos < num.size()) {
				if (pos >= num.size() - newK)
					return num.substr(0, pos); // remove all char from pos
				auto min = min_element(begin(num) + pos + 1, begin(num) + pos + 1 + newK);
				if (*min < num[pos]) {  // find a small char, so we can remove current char
					num.erase(pos, 1);
					break;
				}
				else  // current char is small, move to next
					pos++;
			}
		}
		auto trim = find_if_not(begin(num), end(num), [](char c) {return c == '0';});
		if (trim != begin(num))
			num.erase(begin(num), trim);
		return num.empty() ? "0" : num;
	}

	// Given N gas station, gas[i] is amount of gas you can fill, cost[i] is amount needed to drive from i to i+1
	// Return the starting gas station's index if you can travel around the circuit once in the clockwise direction, otherwise return -1
	// idea: compute prefix sum from left to right, when it is negative, start from next stattion
	// if prefix sum ended negative, no answer. else add up last total with previous negative cumulative sum, it is good if it is not negative
	int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
		transform(begin(gas), end(gas), begin(cost), begin(gas), minus<int>());  // gas-cost, net gain on each station
		int n = gas.size();
		if (n == 1)
			return gas[0] >= 0 ? 0 : -1;
		int start = 0;
		int total = 0;
		int cumul = 0;
		for (int i = 0; i < n; i++) {
			total += gas[i];
			if (total < 0) {     // negative will not help, start from next station
				cumul += total;  // optimization, save values calculated so far, beat 98%
				total = 0;
				start = i+1;  // bug fix start++;
			}
		}
		if (start == n)
			return -1;
		return cumul +total<0?-1:start;
	}
};

void testReorg()
{
	Greedy g;
	cout << g.reorganizeString("aabbcd") << endl;
	cout << g.reorganizeString("aabbccdde") << endl;
	cout << g.reorganizeString("aaab") << endl;
	cout << g.reorganizeString("aab") << endl;
	cout << g.reorganizeString("a") << endl;
	cout << g.reorganizeString("") << endl;
}
void testGreedy()
{
	Greedy g;
	cout << g.removeKdigits("10200", 1) << endl;  //200
	cout << g.removeKdigits("1432219", 3) << endl;  // 1219
	cout << g.removeKdigits("10", 2) << endl;  //0
	cout << g.removeKdigits("10200", 2) << endl;
	cout << g.removeKdigits("102", 2) << endl;
	cout << g.removeKdigits("1012", 2) << endl;

	cout << g.canCompleteCircuit(vector<int>{1,2,3,4,5,5,70}, vector<int>{2,3,4,3,9,6,2}) << endl;  // 6
	cout << g.canCompleteCircuit(vector<int>{3,3,4}, vector<int>{3,4,4}) << endl;  // -1
	cout << g.canCompleteCircuit(vector<int>{1,2, 3, 4,5}, vector<int>{3, 4, 5,1,2}) << endl;  // 3
	cout << g.canCompleteCircuit(vector<int>{2,3,4}, vector<int>{3,4,3}) << endl;  // -1
	cout << g.canCompleteCircuit(vector<int>{2}, vector<int>{2}) << endl;  // 0
}

class Sort {
public:
	void sortColors(vector<int>& nums) {  // 3 pointers, beat 100%
		int p0 = 0, p1 = nums.size() - 1, p2 = nums.size();
		while (p0 <= p1) {  // edge case , = is needed here
			while (nums[p0] == 0)  // scan 0 forward
				p0++;
			while (nums[p1] == 1)  // scan for 1 backward
				p1--;
			if (p0 > p1)
				break;
			if (nums[p1] == 0)      // find 0 at p1, swap with p0
				std::swap(nums[p1], nums[p0++]);
			if (nums[p1] == 2) {    // find 2 at p1 (could be from old p0)
				std::swap(nums[--p2], nums[p1]);
			}
			p1--;
		}
		//cout << p0 << " " << p1 << " " << p2 << endl;
	}

	// Given a string and a string dictionary, find the longest in dictionary that can be formed by deleting some characters of the given string
	// If there are more than one possible results, return the longest word with the smallest lexicographical order
	template<class ForwardIterator1, class ForwardIterator2>
	bool subsequence(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2)
	{
		while (first1 != last1 && first2 != last2) {
			if (*first1 == *first2) {
				first1++;
				first2++;
			}
			else
				first1++;
		}
		return first2 == last2;
	}

	string findLongestWord(string s, vector<string>& d) {  // beat 96%
		string *ans = nullptr;
		for (string& word : d) {
			if (ans) {
				int size = ans->size() - word.size();
				if (size>0)
					continue;
				else if (size == 0 && lexicographical_compare(begin(*ans), end(*ans), begin(word), end(word))) {
					continue;
				}
			}
			auto found = subsequence(begin(s), end(s), begin(word), end(word));
			if (found ) {
				ans = &word;
			}
		}
		return ans == nullptr ? "" : *ans;
	}
};

void testColor()
{
	Sort s;
	auto test = [&s](vector<int> nums) {
		s.sortColors(nums);
		print(nums);
	};
	test(vector<int>{ 1, 2, 0 });
	test(vector<int>{ 2, 0, 2, 1, 1, 0 });
}

void testLongestWord()
{
	Sort s;
	cout << s.findLongestWord("abpcplea", vector<string>{"ale", "apple", "monkey", "plea"}) << endl;

}

void test()
{
	testLongestWord();
}