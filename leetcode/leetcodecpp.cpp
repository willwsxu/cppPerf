﻿#include "stdafx.h"

////////////////////////////////////////////////
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
using namespace std;

int findMaxForm(vector<string>& strs, int m, int n) {
	int **memo = new int*[m + 1];
	for (int i = 0; i <= m; i++) {
		memo[i] = new int[n + 1];
		for (int j = 0; j <= n; j++) {
			memo[i][j] = 0;
		}
	}
	for (size_t i = 0; i < strs.size(); i++) {
		int ones = 0;
		int zero = 0;
		for (size_t j = 0; j < strs[i].size(); j++) {
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
		triangle.emplace_back( 3, 4 );
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
	for (size_t i = numIdx; i < nums.size(); i++) {
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
	if (pos >= (int)prices.size()) {
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

class NQueens {
	class DiagMask
	{
	protected:
		vector<bool> mask;
		DiagMask(int n):mask(2*n-1,false)
		{

		}
		bool get(int idx) {
			if (idx < (int)mask.size())
				return mask[idx];
			return false;
		}
		void set(int idx, bool v)
		{
			if (idx < (int)mask.size())
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
		for (size_t c = 0; c < mCol.size(); c++) {  // try each col
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
		for (size_t i = idx; i < s.length(); i++) {
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
			if (pos + newK >= (int)num.size())  // remove all
				return "0";
			while (pos < (int)num.size()) {
				if (pos >= (int)num.size() - newK)
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

	// A scientist has index h if h of his / her N papers have at least h citations each, and the other N − h papers have no more than h citations each
	// idea 1: sort by citation in reverse order, find the first index where c[i]>=i+1, c[i+1]<=i+1
	int hIndex_sort(vector<int>& citations) {  // beat 97%, sorting
		if (citations.empty())
			return 0;
		int n = citations.size();
		sort(begin(citations), end(citations), greater<int>());
		for (int i = 0; i < n-1; i++) {
			if (citations[i] >= i + 1 && citations[i + 1] <= i + 1)
				return i + 1;
		}
		return citations[n - 1] >= n ?n:0;
	}
	int hIndex(vector<int>& citations) { // fastest, beat 99%
		if (citations.empty())
			return 0;
		int n = citations.size();
		vector<int> count(n+1, 0); // count papers of a certain citation,  bound all to last bucket if citation>=n
		for (int c : citations) {
			if (c >= n)
				count[n]++;
			else
				count[c]++;
		}
		int papers = 0;
		for (auto ri = rbegin(count); ri != rend(count); ri++) {
			papers += *ri;
			if (papers >= n)  // n papaers has at least n citation
				break;
			n--;
		}
		return n;
	}

	// sorted in ascending orders
	int bs(vector<int>& citations, int start, int end)
	{
		if (start>=end)
			return citations.size() - start;
		int mid = (start + end) / 2;
		int h = citations.size() - mid;
		if (citations[mid] >= h && (mid==0 || citations[mid - 1] <= h))
			return h;
		if (citations[mid] < h)
			return bs(citations, mid + 1, end);
		return bs(citations, start, mid-1);
	}
	int hIndex2(vector<int>& citations) {  // beat 24% without binary search
		if (citations.empty())
			return 0;
		int n = citations.size();
		if (citations[0] >= n)
			return n;
		return bs(citations, 0, n);  // binary search improve performance 9ms vs 31ms, beat 80%
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


// 659. Split Array into Consecutive Subsequences
// each subsequences consist of at least 3 consecutive integers
// borrowed idea: keep state 3 sequence at current and previous element, 1, 2, 3+
class Consecutive {  // Greedy
public:
	bool isPossible(vector<int>& nums) {  // beat86%
		int N = nums.size();
		if (N < 3)
			return false;
		int pre = INT_MIN;        // previous element
		int p1 = 0, p2 = 0, p3 = 0; // count of seq ends at pre with len 1, 2, or 3+
		int cur = nums[0];        // current element
		int cnt = 1;    // count of current element
						//int c1, c2, c3; // count of seq ends at cur with len 1, 2, or 3+
		for (int i = 1; i <= N; i++)  // include exra to take care of processing of last sequence
		{
			if (i != N && nums[i] == cur) { // count number of same value
				cnt++;
				continue;
			} // num[i] is now different from cur
			if (cur != pre + 1) {  // number is not consecitive, start new sequence
				if (p1 + p2 != 0)   // some sequence of len 1 or 2 is stopped
					return false;
				p1 = cnt;  // start new sequence
				p3 = 0;
				pre = cur;
				cur = nums[i];
				cnt = 1;
				continue;
			}
			if (cnt < p1 + p2)
				return false;  // not enough current to extend sequence with len 1 or 2
			int c2 = p1;  // len 1 seq will increment 1
			int c3 = p2 + std::min(cnt - p1 - p2, p3);  // all p2 will become len3, some or all p3 will extend to more
			p1 = std::max(cnt - p1 - p2 - p3,0);        // extra cnt will start from seq len=1
			p2 = c2;
			p3 = c3;
			pre = cur;
			if (i == N)
				break;
			cur = nums[i];
			cnt = 1;
		}
		return p1+p2==0;
	}
};

void testSplitSubSeq()
{
	Consecutive g;
	cout << g.isPossible(vector<int>{1, 2, 3, 3, 4, 5}) << endl;
	cout << g.isPossible(vector<int>{1, 2, 3, 3, 4, 4, 5, 5}) << endl;
	cout << g.isPossible(vector<int>{1, 2, 3, 4, 4, 5}) << endl;
}

struct Interval {
	int start;
	int end;
	Interval() : start(0), end(0) {}
	Interval(int s, int e) : start(s), end(e) {}
	
};
class BinarySearch {
	int bsPeak(vector<int>& nums, int start, int end)
	{
		if (start >= end)
			return start;
		int mid = (start + end) / 2;
		int mid2 = mid + 1;
		if (nums[mid] > nums[mid2])  //exist local maximum as left end id -∞
			return bsPeak(nums, start, mid);
		return bsPeak(nums, mid2, end); //exist local maximum as right end id -∞
	}

	int bsRotated(vector<int>& nums, int start,  int end)  // find minimum
	{
		if (start >= end)
			return nums[start];
		int mid = (start + end) / 2;
		if (nums[mid] > nums[end])  // pivot point is to the right
			return bsRotated(nums, mid + 1, end);
		return bsRotated(nums,start, mid);
	}
	int bsRotated(vector<int>& nums, int start, int end, int target)
	{
		if (start >= end) {
			if ( nums[start] == target)
				return start;
			return -1;
		}
		int mid = (start + end) / 2;
		if (nums[mid] == target)
			return mid;
		if (nums[mid] < nums[end])  // right side is sorted
		{
			if (target > nums[mid] && target <= nums[end])
				return bsRotated(nums, mid + 1, end, target);
			return bsRotated(nums, start, mid - 1, target);
		}
		if (target >= nums[start] && target < nums[mid])  // left side is sorted, and target is within
			return bsRotated(nums, start, mid - 1, target);
		return bsRotated(nums, mid + 1, end, target);
	}
	int lower_bound_closest(vector<int>& arr, int k, int x, int low, int hi)
	{
		if (low >= hi)
			return low;
		int mid = (low + hi) / 2;
		if (hi - low == 1)
			mid = low;
		int hiVal = mid + k<(int)arr.size()? arr[mid + k]: INT_MAX;  // right most elem+k will be out of bound
		if (x - arr[mid] > arr[mid + k] - x)
			return lower_bound_closest(arr, k, x, mid + 1, hi);
		return lower_bound_closest(arr, k, x, low, mid);
	}
public:
	// 162. Find Peak Element
	// A peak element is an element that is greater than its neighbors
	// The array may contain multiple peaks, in that case return the index to any one of the peaks is fine.
	// You may imagine that num[-1] = num[n] = -∞ (Important clue)
	int findPeakElement(vector<int>& nums) { // beat 98%
		return bsPeak(nums, 0, nums.size()-1);
	}

	// 153. Find Minimum in Rotated Sorted Array
	// array is sorted ascending, then torated
	int findMin(vector<int>& nums) {  // beat 99%
		return bsRotated(nums, 0, nums.size() - 1);
	}

	// 33. Search in Rotated Sorted Array, no duplicates
	int search(vector<int>& nums, int target) {
		if (nums.empty())
			return -1;
		return bsRotated(nums, 0, nums.size() - 1, target);
	}

	//34. Search for a Range
	vector<int> searchRange(vector<int>& nums, int target) {
		auto found = equal_range(begin(nums), end(nums), target);
		if (found.first == found.second)
			return vector<int>{-1, -1};
		return vector<int>{found.first-begin(nums), found.second- begin(nums)-1};
	}

	//436. Find Right Interval
	// for each of the interval i, check if there exists an interval j whose start point is bigger than or equal to 
	//   the end point of the interval i, which can be called that j is on the "right" of i
	// store minimal interval j's index, -1 if not found
	vector<int> findRightInterval(vector<Interval>& intervals) {  // use map is faster than sorted vector
		const int n = intervals.size();
		std::map<int,int> starter; // save starting point of each interval and its index
		for (int i = 0; i < n; i++) {
			starter[intervals[i].start]= i;
		}
		vector<int> ans(n, -1);
		for (int i = 0; i < n; i++) {
			auto found = starter.lower_bound(intervals[i].end);
			if (found != end(starter))
				ans[i] = found->second;
		}
		return ans;
	}

	// 658. Find K Closest Elements
	// 
	vector<int> findClosestElements(vector<int>& arr, int k, int x) {
		if ((int)arr.size() <= k)
			return arr;
		int i = lower_bound_closest(arr, k ,x, 0, arr.size()-k);
		vector<int> ans;
		copy_n(begin(arr) + i, k, back_inserter(ans));
		return ans;
	}

	// 209. Minimum Size Subarray Sum
	// minimal length of a contiguous subarray of which the sum ≥ s. If there isn't one, return 0 instead
	int minSubArrayLen(int s, vector<int>& nums) {
		if (nums.empty())
			return 0;
		int n = nums.size();
		for (int i = 1; i < n; i++)  // prefix sum
			nums[i] += nums[i - 1];
		if (nums[n - 1] < s)  // nums[n - 1] is max sum
			return 0;
		//print(nums);
		int ans = lower_bound(begin(nums), end(nums), s)-begin(nums)+1;  // from the very first element
		//cout << " initial ans " << ans << endl;
		for (int i = 1; i < n; i++) {
			auto found = lower_bound(begin(nums), end(nums), s + nums[i - 1]);
			if (found == end(nums))  // total is smaller <s, from i
				break;
			int dist = found - begin(nums) - i +1;
			//cout << " dist " << dist << " from " << i << endl;
			if (dist < ans)
				ans = dist;
		}
		return ans;
	}
	void prefixsum(vector<int>& nums)
	{
		for (unsigned int i = 1; i < nums.size(); i++)
			nums[i] += nums[i - 1];
	}
	//560. find the total number of continuous subarrays whose sum equals to k
	// The range of numbers in the array is [-1000, 1000] and the range of the integer k is [-1e7, 1e7].
	// strategy: compute prefix sum Sj, find if there is any Sj-k already computed, store prefix sum in hashmap
	int subarraySum(vector<int>& nums, int k) {
		unordered_map<int, int> PrefixSumCount{ {0,1} };  // add sum=0, count=1 as baseline
		int ans = 0;
		int sum = 0;
		for (unsigned int i = 0; i < nums.size(); i++) {
			sum += nums[i];
			auto found = PrefixSumCount.find(sum - k);  // search sum-k in map
			if (found != PrefixSumCount.end()) {
				ans += found->second;
			}
			found = PrefixSumCount.find(sum);  // update current sum to map
			if (found != PrefixSumCount.end()) {
				PrefixSumCount[sum] = PrefixSumCount[sum] + 1;
			}
			else
				PrefixSumCount[sum] = 1;
		}
		return ans;
	}
};

class BinarySearch2
{
	bool bsRotated(vector<int>& nums, int start, int end, int target)
	{
		if (start >= end)
			return nums[start] == target;
		int mid = (start + end) / 2;
		if (nums[mid] == target)
			return true;
		if (nums[mid] < nums[end])  // right side is sorted
		{
			if (target > nums[mid] && target <= nums[end])
				return bsRotated(nums, mid + 1, end, target);
			return bsRotated(nums, start, mid - 1, target);
		}
		else if (nums[mid] == nums[end]) {  // try both sides
			if (bsRotated(nums, start, mid - 1, target))
				return true;
			return bsRotated(nums, mid + 1, end, target);
		}
		if (target >= nums[start] && target < nums[mid])  // left side is sorted, and target is within
			return bsRotated(nums, start, mid - 1, target);
		return bsRotated(nums, mid + 1, end, target);
	}
public:
	// 81. Search in Rotated Sorted Array II
	// Search a target value. array was in ascending order
	bool search(vector<int>& nums, int target) { // lots of edge cases, beat 95%
		if (nums.empty())
			return false;
		return bsRotated(nums, 0, nums.size() - 1, target);
	}
};

TEST_CASE("H-Index2", "HINDEX2")
{
	Sort s;
	SECTION("middle case") {
		CHECK(s.hIndex2(vector<int>{0, 1, 3, 5, 6}) == 3);
		CHECK(s.hIndex2(vector<int>{0, 1, 4, 5, 6}) == 3);
		REQUIRE(s.hIndex2(vector<int>{0, 1, 2, 5, 6}) == 2);
	}
	SECTION("right edge case") {
		CHECK(s.hIndex2(vector<int>{5, 5, 5, 5, 6}) == 5);
		REQUIRE(s.hIndex2(vector<int>{11,15}) == 2);
	}
	SECTION("edge case") {
		REQUIRE(s.hIndex2(vector<int>{0}) == 0);
	}
}

TEST_CASE("H-Index", "HINDEX")
{
	Sort s;
	SECTION("middle case") {
		CHECK(s.hIndex(vector<int>{3, 0, 6, 1, 5}) == 3);
		CHECK(s.hIndex(vector<int>{4, 0, 6, 1, 5}) == 3);
		REQUIRE(s.hIndex(vector<int>{2, 0, 6, 1, 5}) == 2);
	}
	SECTION("right edge case") {
		CHECK(s.hIndex(vector<int>{5, 5, 6, 5, 5}) == 5);
		REQUIRE(s.hIndex(vector<int>{11, 15}) == 2);
	}
	SECTION("edge case") {
		CHECK(s.hIndex(vector<int>{0}) == 0);
		REQUIRE(s.hIndex(vector<int>{1,2}) == 1);
	}
}


TEST_CASE("Find Peak", "[BS]")
{
	BinarySearch t;
	SECTION("middle case") {
		REQUIRE(t.findPeakElement(vector<int>{2,6,6,8,5}) == 3);
	}
	SECTION("edge case") {
		CHECK(t.findPeakElement(vector<int>{3, 4, 5}) == 2);
		REQUIRE(t.findPeakElement(vector<int>{3, 2, 1}) == 0);
	}
	SECTION("edge case 2") {
		REQUIRE(t.findPeakElement(vector<int>{0}) == 0);
	}
}

TEST_CASE("Rotated Array", "[Rotate]")
{
	BinarySearch t;
	SECTION("no rotate") {
		REQUIRE(t.findMin(vector<int>{0, 1, 2, 4, 5, 6, 7}) == 0);
	}
	SECTION("rotate case") {
		CHECK(t.findMin(vector<int>{4, 5, 6, 7, 0, 1, 2}) ==0);
		REQUIRE(t.findMin(vector<int>{7, 0, 1, 2, 4, 5, 6}) == 0);
	}
	SECTION("edge case") {
		CHECK(t.findMin(vector<int>{0}) == 0);
		CHECK(t.findMin(vector<int>{1,0}) == 0);
		REQUIRE(t.findMin(vector<int>{0,1}) == 0);
	}
}

TEST_CASE("Right Interval", "[RINT]")
{
	BinarySearch t;
	SECTION("basic") {
		REQUIRE(t.findRightInterval(vector<Interval>{ {1, 2}}) == vector<int>{-1});
	}
	SECTION("normal") {
		CHECK(t.findRightInterval(vector<Interval>{ {3, 4}, { 2,3 }, {1, 2}}) == vector<int>{-1,0,1});
		REQUIRE(t.findRightInterval(vector<Interval>{ {1, 4}, { 2,3 }, { 3, 4 }}) == vector<int>{-1, 2, -1});
	}
}

TEST_CASE("Search Rotated Array II", "[SearchRotate]")
{
	BinarySearch2 t;
	SECTION("edge case") {
		CHECK(t.search(vector<int>{1, 1, 3, 1}, 3) == true);	// try both sides if mid is same as one end
		CHECK(t.search(vector<int>{3, 1, 1}, 3) == true);     
		CHECK(t.search(vector<int>{1, 1}, 0) == false);			// need to increment or decrement mid to avoid infinite loop
		CHECK(t.search(vector<int>{}, 3) == false);				// check empty
		CHECK(t.search(vector<int>{1}, 3) == false);
		CHECK(t.search(vector<int>{3}, 3) == true);
		REQUIRE(t.search(vector<int>{1,3}, 3) == true);
		CHECK(t.search(vector<int>{1,1}, 3) == false);
	}
	SECTION("left") {
		CHECK(t.search(vector<int>{2, 5, 6, 0, 0, 1, 2}, 5) == true);
		REQUIRE(t.search(vector<int>{2, 2, 5, 6, 0, 0, 1}, 5) == true);
	}
	SECTION("easy case, mid is target") {
		REQUIRE(t.search(vector<int>{2, 5, 6, 0, 0, 1, 2}, 0) == true);
	}
	SECTION("not found") {
		REQUIRE(t.search(vector<int>{2, 5, 6, 0, 0, 1, 2}, 3) == false);
	}
	SECTION("right") {
		CHECK(t.search(vector<int>{2, 5, 6, 0, 0, 1, 2}, 1) == true);
		REQUIRE(t.search(vector<int>{2, 2, 5, 6, 0, 0, 1}, 1) == true);
	}
}


TEST_CASE("Search Rotated Array", "[SearchRotate]")
{
	BinarySearch t;
	SECTION("edge case") {
		CHECK(t.search(vector<int>{2, 3, 1}, 3) == 1);	// try both sides if mid is same as one end
		CHECK(t.search(vector<int>{3, 1, 2}, 3) == 0);
		CHECK(t.search(vector<int>{}, 3) == -1);				// check empty
		CHECK(t.search(vector<int>{1}, 3) == -1);
		CHECK(t.search(vector<int>{3}, 3) == 0);
		REQUIRE(t.search(vector<int>{1, 3}, 3) == 1);
	}
	SECTION("left") {
		CHECK(t.search(vector<int>{4, 5, 6, 7, 0, 1, 2}, 5) == 1);
		REQUIRE(t.search(vector<int>{4, 5, 6, 7, 0, 1, 2}, 1) == 5);
	}
	SECTION("easy case, mid is target") {
		REQUIRE(t.search(vector<int>{4, 5, 6, 7, 0, 1, 2}, 7) == 3);
	}
	SECTION("not found") {
		REQUIRE(t.search(vector<int>{4, 5, 6, 7, 0, 1, 2}, 3) == -1);
		REQUIRE(t.search(vector<int>{6, 7, 0, 1, 2}, 3) == -1);
	}
	SECTION("right") {
		CHECK(t.search(vector<int>{6, 7, 0, 1, 2, 3, 4}, 0) == 2);
		REQUIRE(t.search(vector<int>{6, 7, 0, 1, 2, 3, 4}, 3) == 5);
	}
}

TEST_CASE("Search Range", "[RANGE]")
{
	BinarySearch t;
	SECTION("edge case") {
		CHECK(t.searchRange(vector<int>{}, 3) == vector<int>{-1, -1});				// check empty
		CHECK(t.searchRange(vector<int>{1}, 3) == vector<int>{-1, -1});
		CHECK(t.searchRange(vector<int>{3}, 3) == vector<int>{0, 0});
		CHECK(t.searchRange(vector<int>{1, 3}, 3) == vector<int>{1, 1});
		REQUIRE(t.searchRange(vector<int>{1, 3}, 1) == vector<int>{0, 0});
	}
	SECTION("normal case") {
		CHECK(t.searchRange(vector<int>{5, 7, 7, 8, 8, 10}, 8) == vector<int>{3, 4});
		REQUIRE(t.searchRange(vector<int>{5, 7, 7, 8, 8, 10}, 6) == vector<int>{-1, -1});
	}
}

TEST_CASE("Find K Closest", "[KClosest]")
{
	BinarySearch t;
	SECTION("normal case") {
		REQUIRE(t.findClosestElements(vector<int>{0, 1, 2, 3, 8, 9}, 4, 3) == vector<int>{0, 1, 2, 3});
	}
	SECTION("edge case") {
		REQUIRE(t.findClosestElements(vector<int>{1, 2, 3, 4, 5}, 4, 4) == vector<int>{2, 3, 4, 5});
	}
	SECTION("tie case") {
		REQUIRE(t.findClosestElements(vector<int>{1, 2, 3, 4, 5}, 4, 3) == vector<int>{1, 2, 3, 4});
	}
}


TEST_CASE("Minimum Size Subarray Sum", "[SubArray]")
{
	BinarySearch t;
	SECTION("normal case") {
		REQUIRE(t.minSubArrayLen(7, vector<int>{2, 3, 1, 2, 4, 3}) == 2);
	}
	SECTION("edge case") {
		CHECK(t.minSubArrayLen(15, vector<int>{1, 2, 3, 4, 5}) == 5);
		REQUIRE(t.minSubArrayLen(16, vector<int>{1, 2, 3, 4, 5}) == 0);
	}
}

class Array
{
	void visit(vector<int>& nums, int pos, vector<int>& ans)
	{
		int next = nums[pos] - 1;  // next position to visit
		nums[pos] = 0;
		while (next != pos && nums[next] > 0) {
			pos = next; // save last position
			next = nums[pos] - 1;
			nums[pos] = -1;  // mark last position as visited
		}
		if (nums[next] == 0)
			nums[next] = -1;
		else
			ans.push_back(next + 1); // find dup when loop ends
	}
public:
	// 442. Given an array of integers, 1 ≤ a[i] ≤ n (n = size of array), some elements appear twice and others appear once
	vector<int> findDuplicates(vector<int>& nums) {
		vector<int> ans{};
		for (auto i = 0u; i < nums.size(); i++) {
			if (nums[i] <= 0)  // visited
				continue;
			if (nums[i] == i + 1) {  // nums at right position
				nums[i] = -1;
				continue;
			}
			visit(nums, i, ans);
		}
		return ans; // move constructor is called automatically
	}

	// attack at each time point, cause duration of being poisoned, each point represent time elapses
	int findPoisonedDuration(vector<int>& timeSeries, int duration) {  // beat 44%
		int total = timeSeries.empty()?0:duration; // duration on last time series
		for (int i = 0; i < (int)timeSeries.size() - 1; i++) {
			total += min(duration, timeSeries[i + 1] - timeSeries[i]);  // poisoned duration can not exceed time span between 2 points
		}
		return total;
	}

	// 667. Beautiful Arrangement II
	// Give Array with value 1 to n, arrange it so difference of neighboring 2 numbers have exact k distinct values
	vector<int> constructArray(int n, int k) {  // beat 90%
		vector<int> ans(n);
		int delta = k;
		int val = 1;  // start from, add k, minus k-1, add k+2, etc.
		bool up = true;
		for (int i = 0; i <= k; i++) {  // fill first k+1 elements per algorithm above
			ans[i] = val;
			val = up ? val + delta : val - delta;
			up = !up;
			--delta;
		}
		iota(begin(ans)+ k+1, end(ans), k+2); // fill remaining slots in sequential order
		return ans;
	}
	vector<int> constructArray2(int n, int k) {  // borrow idea, beat 70%
		vector<int> ans(n);
		for (int i = 0,j=1; i <= k; i+=2,j++)  // even slot, 1,2,3,...
			ans[i] = j;
		for (int i = 1, j = k+1; i <= k; i += 2, j--)  // odd slot, k+1, k, k-1,...
			ans[i] = j;
		iota(begin(ans) + k + 1, end(ans), k + 2); // fill remaining slots in sequential order
		return ans;
	}

	// 238. Product of Array Except Self
	void productExceptSelf(vector<int>& nums, vector<int>& ans, int start, int end) {
		if (start >= end)
			return;
		int mid = (start + end) / 2;
		int leftprod = 1;
		int rightprod = 1;
		for (int i = start; i <= mid; i++)
			leftprod *= nums[i];
		for (int i = mid + 1; i <= end; i++) {
			rightprod *= nums[i];
			ans[i] *= leftprod;
		}
		for (int i = start; i <= mid; i++)
			ans[i] *= rightprod;
		productExceptSelf(nums, ans, start, mid);
		productExceptSelf(nums, ans, mid+1, end);
	}
	vector<int> productExceptSelf(vector<int>& nums) {  // beat 56%
		vector<int> ans(nums.size(), 1);
		productExceptSelf(nums, ans, 0, nums.size() - 1);
		return ans;
	}
	vector<int> productExceptSelf2(vector<int>& nums) {  // 99%
		int n = nums.size();
		vector<int> ans(n, 1);
		for (int i = 1; i < n; i++)
			ans[i] = ans[i - 1] * nums[i - 1];
		int right = 1;  // right to left
		for (int i = n - 1; i >= 0; i--) {
			ans[i] *= right;
			right *= nums[i];
		}
		return ans;
	}

	// A zero-indexed array A of length N contains all integers from 0 to N-1. 
	// Find and return the longest length of set S, where S[i] = {A[i], A[A[i]], A[A[A[i]]], ... }
	int arrayNesting(vector<int>& nums) {  // beat 64%
		int count = 0;
		for (int i = 0; i < (int)nums.size(); i++) {
			int size = 0;
			for (int j = i; nums[j] >= 0; size++) {
				int temp = nums[j];
				nums[j] = -1; // mark as visited
				j = temp;
			}
			count = max(count, size);
		}
		return count;
	}

	// 769. Max Chunks To Make Sorted, when each chunk is sorted
	int findChunk(vector<int>& arr, int start, int n) { //  chunk must include value start
		int i = start;
		int maxVal = arr[start];
		for (; i < n; i++) {
			maxVal = max(maxVal, arr[i]);
			if (arr[i] == start)
				break;
		}
		for (i = i + 1; i <n; i++) {
			if (i<= maxVal)
				maxVal = max(maxVal, arr[i]);
			if (arr[i] > maxVal)
				break;
		}
		return i - 1;
	}
	int maxChunksToSorted(vector<int>& arr) {  // beats 97%
		int chunks = 0;
		int start=0;
		int n = arr.size();
		while (start < n) {
			start = findChunk(arr, start, n) + 1;  // chunk must include the start value and all values less than max
			chunks++;
		}
		return chunks;
	}
	// 26. Remove Duplicates from Sorted Array, O(1) memory
	int removeDuplicates(vector<int>& nums) { // 2 pointers
		int n = nums.size();
		if (n < 2)
			return n;
		int uniq = 0; // first one is unique,
		for (int i = 1; i < n; i++) {
			if (nums[i] == nums[uniq]) // skip same value
				continue;
			if (++uniq != i) {  // uniq slot extended
				nums[uniq] = nums[i];
			}
		}
		return uniq + 1;
	}
	// 80. Remove Duplicates from Sorted Array II, allow duplicates appeared at most twice
	int removeDuplicates2(vector<int>& nums) { // 2 pointers, beat 93%
		int n = nums.size();
		if (n <= 2)
			return n;
		int result = 0; // first one is chosen
		int count = 1;
		for (int i = 1; i < n; i++) {
			if (nums[i] == nums[result]) { // skip dup more than twice
				if (++count > 2)
					continue;
			}
			else
				count = 1;  // a different number
			if (++result != i) {  // uniq slot extended
				nums[result] = nums[i];
			}
		}
		return result + 1;
	}

	// 795. Number of Subarrays with Bounded Maximum between L and R
	// 1 2 3 4 5  L=3, R=4
	// 1 2 3
	//   2 3
	//     3
	//       4
	int numSubarrayBoundedMax(vector<int>& A, int L, int R) {
		int n = A.size();
		if (n == 0)
			return 0;
		int count = 0;  // # of sub aray seq ends at i
		int total = 0;
		int smallElem = 0;
		for (int i = 0; i < n; i++) {
			if (A[i] > R) {
				count = 0;  // sequence stopped
				smallElem = 0;
				continue;
			}
			if (A[i] >= L) {  // start another new sequence
				count += (1+ smallElem);
				smallElem = 0;
			}
			else
				smallElem++; //leading small number cannot start a new sequence, but can if there is a bigger one later
			total += count; // add up ongoing sequences
		}
		return total;
	}
	//670. given none-negative int, swap two digits at most once to get the maximum valued number
	int maximumSwap(int num) {
		if (num < 10)
			return num;
		int ori = num;
		vector<int> digits;
		while (num) {
			digits.push_back(num % 10);
			num /= 10;
		}
		reverse(begin(digits), end(digits));  // digits in right order
		auto part = begin(digits);  // find reverse sorted sequence from begin
		while (++part != end(digits) && *part <=*(part-1)) {
		}
		if (part == end(digits))
			return ori;
		auto target = max_element_last(part, end(digits)); // e.g 1993
		auto ins = upper_bound(begin(digits), part, *target, greater<int>());
		iter_swap(ins, target);
		num = 0;
		for (int i : digits)
			num = num * 10 + i;
		return num;
	}

	// 392 Given a string s and a string t, check if s is subsequence of t.
	bool isSubsequence(const string& s, const string& t) {  // beat 98%
		auto start=begin(t);
		for (char c : s) {
			auto found = find(start, end(t), c);
			if (found == end(t))
				return false;
			start = found + 1;
		}
		return true;
	}
	// 792. Number of Matching Subsequences
	int numMatchingSubseq_too_slow(string S, vector<string>& words) {
		int count = 0;
		for (string& w : words) {
			if (isSubsequence(w, S))
				++count;
		}
		return count;
	}
	int numMatchingSubseq(string S, vector<string>& words) { // parallel processing, beat 79%
		int n = words.size();
		if (n == 0)
			return 0;
		if (n == 1)
			return isSubsequence(words[0], S) ? 1 : 0;
		unordered_map<char, vector<const char*>> lanes;  // put words in lane per first char
		for (string& w : words)
			lanes[w[0]].push_back(w.c_str());
		auto& res = lanes[0];  // complete matches are in lane 0
		for (char c : S) {  // go through S once
			auto& vec = lanes[c];
			if (!vec.empty()) { // c match some words 
				auto advance = vec;  // copy these words, and advance to next char
				vec.clear();  // clear existing bucket
				for (const char *p : advance)
					lanes[*(++p)].push_back(p); // re-assign lanes per next char
				if (res.size() == n)
					return n;
			}
		}
		return res.size();
	}
};


TEST_CASE("Array find duplciate", "[DUP]")
{
	Array arr;
	CHECK(arr.findDuplicates(vector<int>{4, 3, 2, 7, 8, 2, 3, 1}) == vector<int>{3, 2});
	CHECK(arr.findDuplicates(vector<int>{1}) == vector<int>{});
	CHECK(arr.findDuplicates(vector<int>{1,1}) == vector<int>{1});
	REQUIRE(arr.findDuplicates(vector<int>{2,2}) == vector<int>{2});
}

TEST_CASE("Array calculate inerval", "[POISON]")
{
	Array arr;
	CHECK(arr.findPoisonedDuration(vector<int>{}, 2) == 0);
	CHECK(arr.findPoisonedDuration(vector<int>{1}, 2) == 2);
	CHECK(arr.findPoisonedDuration(vector<int>{1,2}, 2) == 3);
	REQUIRE(arr.findPoisonedDuration(vector<int>{1, 3}, 2) == 4);
}

TEST_CASE("Array arrange k", "[Construct]")
{
	Array r;
	CHECK(r.constructArray(6, 5) == vector<int>{1,6,2,5,3,4});
	CHECK(r.constructArray(6, 4) == vector<int>{1, 5, 2, 4, 3, 6});
	CHECK(r.constructArray(6, 3) == vector<int>{1, 4, 2, 3, 5, 6});
	CHECK(r.constructArray(6, 2) == vector<int>{1,3, 2, 4, 5, 6});
	CHECK(r.constructArray(6, 1) == vector<int>{1,2,3,4,5,6});

	CHECK(r.constructArray2(6, 5) == vector<int>{1, 6, 2, 5, 3, 4});
	CHECK(r.constructArray2(6, 4) == vector<int>{1, 5, 2, 4, 3, 6});
	CHECK(r.constructArray2(6, 3) == vector<int>{1, 4, 2, 3, 5, 6});
	CHECK(r.constructArray2(6, 2) == vector<int>{1, 3, 2, 4, 5, 6});
	REQUIRE(r.constructArray2(6, 1) == vector<int>{1, 2, 3, 4, 5, 6});

	CHECK(r.productExceptSelf2(vector<int>{1, 2, 3, 4}) == vector<int>{24, 12, 8, 6});
}

TEST_CASE("chunk sort", "[CHUNK]")
{
	Array t;
	CHECK(t.maxChunksToSorted(vector<int>{2, 0, 1, 4, 3, 5, 6}) == 4);
	CHECK(t.maxChunksToSorted(vector<int>{}) == 0);
	CHECK(t.maxChunksToSorted(vector<int>{0}) == 1);
	CHECK(t.maxChunksToSorted(vector<int>{1, 4, 3, 6, 0, 7, 8, 2, 5}) == 1);
}

TEST_CASE("maximum Swap once", "[SWAP]")
{
	Array t;
	CHECK(t.maximumSwap(1993) == 9913);
	CHECK(t.maximumSwap(10909091) == 90909011);
}

using MyPair = tuple<int, int>;
namespace std
{
	template<> struct less<MyPair>
	{
		bool operator()(const MyPair& lhs, const MyPair& rhs) const  //constexpr not working 
		{
			return get<1>(lhs) <= get<0>(rhs) ? true : false;
		}
	};
}

class MyCalendar {
	using Calendar = set<MyPair>;  // sorted by start
	Calendar cal;

public:
	MyCalendar() {}

	bool book(int start, int end) {
		return cal.emplace(start, end).second;
	}
};


TEST_CASE("calendar booking", "[BOOK]")
{
	MyCalendar t;
	CHECK(t.book(10, 20) == true);
	CHECK(t.book(15, 25) == false);
	CHECK(t.book(20, 30) == true);
}

vector<MyPair> intersect(const MyPair& p1, const MyPair& p2, set<MyPair>& overlap)
{
	if (less<MyPair>()(p1, p2) || less<MyPair>()(p2, p1))   // less<> is not same as < operator
		return{}; // no overlap

	if (get<1>(p2) < get<1>(p1))
		return intersect(p2, p1, overlap);   //ensure p1 not after p2
	vector<MyPair> ans;
	if (get<0>(p1) >= get<0>(p2)) {  // p1 is subset of p2
		overlap.insert(p1);  // overlapping section
		if (get<0>(p1) > get<0>(p2))  // left of p1
			ans.emplace_back(get<0>(p2), get<0>(p1));
	}
	else {
		ans.emplace_back(get<0>(p1), get<0>(p2));
		overlap.emplace(get<0>(p2), get<1>(p1));  // overlapping section
	}
	if (get<1>(p1) < get<1>(p2))  // rigth of p1
		ans.emplace_back(get<1>(p1), get<1>(p2));
	return ans;
}

class MyCalendarTwo {
	using Calendar = set<MyPair>; 
	Calendar cal, overlap;

public:
	MyCalendarTwo() {}

	bool book(int start, int end) {
		if (overlap.count({ start,end }) > 0)  // triple booking
			return false;
		auto rng = cal.equal_range({ start, end });  // could have more than one overlap sections
		if (rng.first == rng.second)  // no overlap
			cal.emplace(start, end);
		else {  // double booking
			vector<MyPair> noneoverlap;
			for (auto found = rng.first; found != rng.second; ++found) 			{
				auto parts = intersect(*found, { start,end }, overlap);  // compute parts of none overlapping sections, and overlapped section
				noneoverlap.insert(noneoverlap.end(), parts.begin(), parts.end());  // append result, don't erase in the loop as iterator will be invalid
			}
			cal.erase(rng.first, rng.second);  // delete the old one
			cal.insert(begin(noneoverlap), noneoverlap.end());
		}
		return true;
	}
};


TEST_CASE("calendar booking 2", "[CAL]")
{
	set<MyPair> overlap;
	CHECK(intersect({ 3, 6 }, { 5,8 }, overlap).size() == 2);
	CHECK(intersect({ 5, 6 }, { 5,8 }, overlap).size() == 1);
	CHECK(overlap.size() == 1);  //[5,6)
	CHECK(intersect({ 6, 7 }, { 5,8 }, overlap).size() == 2);
	CHECK(overlap.size() == 2);  //[6,7) added
	CHECK(intersect({ 6, 8 }, { 5,8 }, overlap).size() == 1);
	CHECK(overlap.size() == 2);  // [6,8) not added
	CHECK(intersect({ 7, 8 }, { 5,8 }, overlap).size() == 1);
	CHECK(overlap.size() == 3);  // [7,8) not added
	CHECK(intersect({ 5,8 }, { 3, 6 }, overlap).size() == 2);
	CHECK(overlap.size() == 3);
	
	MyCalendarTwo t;
	CHECK(t.book(10, 20) == true);
	CHECK(t.book(50, 60) == true);
	CHECK(t.book(10, 40) == true);
	CHECK(t.book(5, 15) == false);
	CHECK(t.book(5, 10) == true);
	CHECK(t.book(25, 55) == true);

	MyCalendarTwo t2;
	CHECK(t2.book(5,12) == true);
	CHECK(t2.book(42, 50) == true);
	CHECK(t2.book(4, 9) == true);
	CHECK(t2.book(33, 41) == true);
	CHECK(t2.book(2, 7) == false);

	CHECK(t2.book(16, 25) == true);
	CHECK(t2.book(7, 16) == false);
	CHECK(t2.book(6, 11) == false);
	CHECK(t2.book(13, 18) == true);
	CHECK(t2.book(38, 43) == true);

	CHECK(t2.book(49, 50) == true);
	CHECK(t2.book(6, 15) == false);
	CHECK(t2.book(5, 13) == false);
	CHECK(t2.book(35, 42) == false);
	CHECK(t2.book(19, 24) == true);

	CHECK(t2.book(46, 50) == false);
	CHECK(t2.book(39, 44) == false);
	CHECK(t2.book(28, 36) == true);
	CHECK(t2.book(28, 37) == false);
	CHECK(t2.book(20, 29) == false);
	CHECK(t2.book(41, 49) == false);
}


TEST_CASE("subarray with bounded max", "[BOUND]")
{
	Array t;
	CHECK(t.numSubarrayBoundedMax(vector<int>{2,9,2,5,6}, 2, 8) == 7);
	CHECK(t.numSubarrayBoundedMax(vector<int>{73, 55, 36, 5, 55, 14, 9, 7, 72, 52}, 32, 69) == 22);
	CHECK(t.numSubarrayBoundedMax(vector<int>{1,2,3,4,5}, 3,4) == 7);
}

// average O(1) time
class RandomizedSet {  // beat 93% after making random engine as data member
	vector<int> indexMap;
	unordered_map<int,int> valueMap;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()

public:
	/** Initialize your data structure here. */
	RandomizedSet():gen(rd()) {	}

	/** Inserts a value to the set. Returns true if the set did not already contain the specified element. */
	bool insert(int val) {
		if (valueMap.find(val) != valueMap.end())
			return false;
		auto size = valueMap.size();  // solve leetcode compiler bug, valueMap[val]=valueMap.size() didn't work
		valueMap[val] = size; // map value to last index (0 based)
		indexMap.emplace_back(val);
		return true;
	}

	/** Removes a value from the set. Returns true if the set contained the specified element. */
	bool remove(int val) {
		auto found = valueMap.find(val);
		if (found == valueMap.end())
			return false;
		int remIndex = found->second; // index to remove
		valueMap.erase(found);
		int last = indexMap.size()-1;
		if (remIndex < last)  // remove is not the last one
		{
			indexMap[remIndex] = indexMap[last]; // swap value at last
			valueMap[indexMap[remIndex]] = remIndex;  // update index in value map
		}
		indexMap.pop_back();
		return true;
	}

	/** Get a random element from the set. */
	int getRandom() {
		std::uniform_int_distribution<int> dist(0, indexMap.size() - 1);
		return indexMap.at(dist(gen));
	}
};


TEST_CASE("random set O(1) op", "[RAND]")
{
	RandomizedSet t;
	CHECK(t.insert(1) == true);
	CHECK(t.remove(2) == false);
	CHECK(t.insert(2) == true);
	cout << "get random=" << t.getRandom() << endl;
	CHECK(t.remove(1) == true);
	CHECK(t.insert(2) == false);
	CHECK(t.getRandom() == 2);  // fix bug insert
}

class Triangle2
{
	// 611. Valid Triangle Number
	// The length of the given array won't exceed 1000.
	// The integers in the given array are in the range of[0, 1000].
	int triangleNumber(vector<int>& nums) {  // beat 95%
		sort(nums.begin(), nums.end(),greater<int>());  // sort in reverse order
		int n = nums.size();
		int ans = 0;
		for (int i = 0; i < n - 2; i++) { // pick c as the longest, a+b>c
			int L = i + 1;
			int R = n - 1;
			int c = nums[i];
			while (L < R) {  // check 2 end points
				if (nums[L] + nums[R] > c) {
					ans += (R - L);  // a=nums[L], b can be any from L+1
					++L;
				}
				else  // a+b<=c, R does not work with any, as it it the smallest
					--R;
			}
		}
		return ans;
	}
};


TEST_CASE("subarray sum", "[SUB]")
{
	BinarySearch t;
	CHECK(t.subarraySum(vector<int>{28, 54, 7, -70, 22, 65, -6}, 100) == 1);//prefix is not sorted
	CHECK(t.subarraySum(vector<int>{1, 1, 1}, 2) == 2);
}

class TwoPointers {
public:
	//567  return true if s2 contains the permutation of s1
	bool checkInclusion(string s1, string s2) {  // beat 99%
		int len1 = s1.length();
		int len2 = s2.length();
		if (len1 > len2)
			return false;
		int count[128] = { 0 };
		auto same = [&count]() {
			for (int i = 'a'; i <= 'z'; i++) {
				if (count[i])
					return false;
			}
			return true;
		};
		for (int i = 0; i < len1; i++) {
			count[s1[i]]++;   // add letter count from s1
			count[s2[i]]--;   // subtract letter count from s2
		}
		if (same())  // same when count is 0 for all letters
			return true;
		for (int i = len1; i < len2; i++) {  // sliding window of len1
			count[s2[i]]--;         // subtract next char
			count[s2[i - len1]]++;  // add back first
			if (same())
				return true;
		}
		return false;
	}

	// 713 number of (contiguous) subarrays where the product of all the elements in the subarray is less than k
	int numSubarrayProductLessThanK(vector<int>& nums, int k) {  // beat 62%
		int count = 0;
		int prod = 1;   // compute product between first and second pointer
		for (int second = 0, first = 0; second < (int)nums.size(); second++) {
			prod *= nums[second];     // multiply new val
			while (prod >= k && first <= second) { // divide from beginning of subarray, update pointer
				prod /= nums[first++];
			}
			if (first <= second)   // subarray is not empty
				count += (second - first + 1);  // add all subarrays from second pointer to first pointer
		}
		return count;
	}
	// 826. Most Profit Assigning Work, first 2 vectors are for job's difficulty and profit
	// 3rd vector is worker's ability, can only do job of same of less difficulty
	int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
		auto p = begin(profit);
		using pii = pair<int, int>;
		vector<pii> zip;
		zip.reserve(profit.size());
		for (auto d = begin(difficulty); d != end(difficulty) && p != end(profit); d++, p++) {
			zip.emplace_back(*d, *p);
		}
		// assume all vectors are not sorted
		sort(begin(zip), end(zip), [](auto&a, auto&b) { return a.first < b.first; });  // sort by difficult
		sort(begin(worker), end(worker));
		// work with higher ability should earn more, thus we only need to go through zip once;
		int maxP = 0;
		auto z = begin(zip);
		auto Max = [&z, &zip, &maxP](int acc, int a) {
			for (; z != end(zip); z++) {
				if (z->first > a)  // job too difficult for worker a
					break;
				maxP = max(maxP, z->second);
			}  // found max profit up to difficulty a
			return maxP+acc;
		};
		return accumulate(begin(worker), end(worker), 0, Max); // faster after using accumulate, 89ms vs 114ms
	}

	template<class RanIt>
	pair<RanIt, RanIt> mountain_range(RanIt first, RanIt last)
	{
		first= adjacent_find(first, last, less<int>());
		if (first != last) {
			auto top = first + 1;
			auto down = top;
			while (++down != last) {
				if (*down == *top)
					return mountain_range(down, last);
				else if (*down < *top)
					break;
				else
					++top;
			}
			if (down != last) {
				auto next = down;
				while (++next != last) {
					if (*next >= *down)
						break;
					else
						++down;
				}
				return{ first, down };
			}
		}
		return{ last,last };
	}
	// 845. Longest Mountain in Array, strictly up or down
	int longestMountain(vector<int>& A) {
		int longest = 0;
		auto res = mountain_range(begin(A), end(A));
		while (res.first != res.second) {
			longest = max(longest, (int)distance(res.first, res.second)+1);
			res= mountain_range(res.second, end(A));
		}
		return longest;
	}
};

class Anagram
{
public:
	// 242. Valid Anagram
	// You may assume the string contains only lowercase alphabets.
	bool isAnagram(string s, string t) {
		sort(begin(s), end(s));
		sort(t.begin(), t.end());
		return s == t;
	}
	bool isAnagram2(string s, string t) {  // beat 98%
		int count[26] = { 0 };
		if (s.size() != t.size())
			return false;
		for (int i = 0; i < s.size(); i++) {
			count[s[i] - 'a']++;  // count chars from one string
			count[t[i] - 'a']--;  // subtract chars from other
		}
		return all_of(count, count + 26, [](int a) {return a == 0; });  // char count is 0 for all letter
	}
};


// 481. Magical String
// 1221121221221121122 count of 1 or 2 becomes the sequence
class Magic {
public:
	int magicalString(int n) {  // borrowed idea, beat 98%
		string seq = "122";
		seq.reserve(n);
		int i = 2; // build new seq from "122", from position 2, must be 2 of some digit
		while (seq.size() < n) {
			seq.append(seq[i++] - '0', seq.back() ^ 3);//^3 will alternate between 1 and 2
		}
		return count(seq.begin(), seq.begin() + n, '1');
	}
};
