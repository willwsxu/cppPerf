#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;


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
						ans[i][j] = std::min(ans[i][j], ans[i - 1][j] + 1);  // check top cell
					if (j>0)
						ans[i][j] = std::min(ans[i][j], ans[i][j - 1] + 1);  // check left cell
				}
			}
		}
		for (int i = m - 1; i >= 0; i--) {
			for (int j = n - 1; j >= 0; j--) {
				if (i<m - 1)
					ans[i][j] = std::min(ans[i][j], ans[i + 1][j] + 1);  // check bottom cell
				if (j<n - 1)
					ans[i][j] = std::min(ans[i][j], ans[i][j + 1] + 1);  // check right cell
			}
		}
		return ans;
	}

	// 74. Search a 2D Matrix, easier than #240
	//Integers in each row are sorted from left to right.
	//The first integer of each row is greater than the last integer of the previous row.
	int lowerBoundRow(vector<vector<int>>& matrix, int target, int col, int low, int high)  // find the row contains target
	{
		if (low == high)
			return low;
		int mid = (low + high) / 2;
		if (mid == high)
			mid = low;  // set up proper terminating condition low>=high
		if (matrix[mid][col] == target)
			return mid;
		if (matrix[mid][col] > target)  // could be in this row, or left half
			return lowerBoundRow(matrix, target, col, low, mid);
		return lowerBoundRow(matrix, target, col, mid + 1, high); // right half, from mid+1
	}
	bool searchMatrix(vector<vector<int>>& matrix, int target) { // beat 71%
		if (matrix.empty() || matrix[0].empty())
			return false;
		if (target < matrix[0][0])
			return false;
		int m = matrix.size();
		int n = matrix[0].size();
		if (target > matrix[m - 1][n - 1])
			return false;
		int row = lowerBoundRow(matrix, target, n - 1, 0, m - 1);
		if (matrix[row][n - 1] == target)
			return true;
		auto found = lower_bound(begin(matrix[row]), end(matrix[row]), target);
		return *found == target;
	}

	// binary search by value, not index
	int kthSmallest(vector<vector<int>>& matrix, int k, int low, int high)
	{
		if (low >= high)
			return low;
		int mid = (low + high) / 2;
		if (mid == high)
			mid = low;// prevent infinite loop
		int count = 0;
		int realMid = low;
		for (auto& v : matrix) {  // seach by row
			if (v[0] > mid)
				break;
			auto found = upper_bound(begin(v), end(v), mid);
			count += (found - begin(v));  // count of values <= mid
			int temp = *(found - 1);
			if (temp > realMid)
				realMid = temp;
		}
		if (count == k)
			return realMid;
		if (count > k)
			return kthSmallest(matrix, k, low, realMid);
		return kthSmallest(matrix, k, mid + 1, high);
	}
	// 378. Kth Smallest Element in a Sorted Matrix
	// Given a n x n matrix where each of the rows and columns are sorted in ascending order, find the kth smallest element.
	int kthSmallest(vector<vector<int>>& matrix, int k) {// binary search beat 90%
		int n = matrix.size() - 1;
		return kthSmallest(matrix, k, matrix[0][0], matrix[n][n]);
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

	vector<vector<int>> matrix{ { 1, 0, 1, 1, 0, 0, 1, 0, 0, 1 },{ 0,1,1,0,1,0,1,0,1,1 },{ 0,0,1,0,1,0,0,1,0,0 },{ 1,0,1,0,1,1,1,1,1,1 },{ 0,1,0,1,1,0,0,0,0,1 },{ 0,0,1,0,1,1,1,0,1,0 },{ 0,1,0,1,0,1,0,0,1,1 },{ 1,0,0,0,1,2,1,1,0,1 },{ 2,1,1,1,1,2,1,0,1,0 },{ 2,2,2,1,0,1,0,0,1,1 } };
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
			if (matrix[y][x] >= matrix[r][c])
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
			dfs(matrix, m - 1, i, m, n, atlantic); // last row
		}
		for (int i = 0; i < m; i++) {
			dfs(matrix, i, 0, m, n, pacific);   // first column
			dfs(matrix, i, n - 1, m, n, atlantic);   // last column
		}
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (pacific[i][j] && atlantic[i][j])
					ans.emplace_back(i, j);
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
	vector<pair<int, int>> ans = w.pacificAtlantic(vector<vector<int>>{ {1, 2, 2, 3, 5}, { 3,2,3,4,4 }, { 2,4,5,3,1 }, { 6,7,1,4,5 }, { 5,1,1,2,4 }});
	for_each(begin(ans), end(ans), [](auto p) {cout << "[" << p.first << "," << p.second << "] "; });
	cout << endl;  // [[0, 4], [1, 3], [1, 4], [2, 2], [3, 0], [3, 1], [4, 0]]
	ans = w.pacificAtlantic(vector<vector<int>>{ {1}});
	for_each(begin(ans), end(ans), [](auto p) {cout << "[" << p.first << "," << p.second << "] "; });
	cout << endl;  // [0,0]
	ans = w.pacificAtlantic(vector<vector<int>>{ {1, 2, 3}, { 8,9,4 }, { 7,6,5 }});  // can move right then up, 2 sweep method fail this case
	for_each(begin(ans), end(ans), [](auto p) {cout << "[" << p.first << "," << p.second << "] "; }); //[[0,2],[1,0],[1,1],[1,2],[2,0],[2,1],[2,2]]
}


TEST_CASE("Search Matrix", "[MATRIX]")
{
	Matrix m;
	vector<vector<int>> v1{ { 1, 3, 5, 7 },{ 10, 11, 16, 20 },{ 23, 30, 34, 50 } };
	SECTION("normal case") {
		CHECK(m.searchMatrix(v1, 3));
		CHECK(m.searchMatrix(v1, 4) == false);
		CHECK(m.searchMatrix(v1, 13) == false);
		CHECK(m.searchMatrix(v1, 10));
		CHECK(m.searchMatrix(v1, 31) == false);
		REQUIRE(m.searchMatrix(v1, 34));
	}
	SECTION("edge case") {
		CHECK(m.searchMatrix(vector<vector<int>>{}, 3) == false);
		CHECK(m.searchMatrix(vector<vector<int>>{ {}}, 3) == false);
		CHECK(m.searchMatrix(vector<vector<int>>{ {3}}, 3));
		CHECK(m.searchMatrix(vector<vector<int>>{ {3}}, 1) == false);
		CHECK(m.searchMatrix(vector<vector<int>>{ {1, 3, 5, 7}}, 8) == false);
		CHECK(m.searchMatrix(vector<vector<int>>{ {1, 3, 5, 7}}, 0) == false);
		CHECK(m.searchMatrix(vector<vector<int>>{ {1, 3, 5, 7}}, 4) == false);
		REQUIRE(m.searchMatrix(vector<vector<int>>{ {1, 3, 5, 7}}, 1) == true);
	}
}


TEST_CASE("Kth smallest Matrix", "[Matrix]")
{
	Matrix m;
	vector<vector<int>> v1{ { 1,  5,  9 },{ 10, 11, 13 },{ 12, 13, 15 } };
	SECTION("normal case") {
		CHECK(m.kthSmallest(v1, 1) == 1);
		CHECK(m.kthSmallest(v1, 2) == 5);
		CHECK(m.kthSmallest(v1, 3) == 9);
		CHECK(m.kthSmallest(v1, 7) == 13);
		CHECK(m.kthSmallest(v1, 8) == 13);
		REQUIRE(m.kthSmallest(v1, 9) == 15);
	}
	SECTION("edge case") {
		CHECK(m.kthSmallest(vector<vector<int>>{ {3}}, 1) == 3);
	}
}


// 48. Rotate Image. nxn matrix 90 degree clockwise
/*
1 2 3
4 5 6
7 8 9

7 8 9  reverse
4 5 6
1 2 3

7 4 1  swap A(i,j) with A(j,i)
8 5 2
9 6 3*/
class Array2D {
public:
	void rotate(vector<vector<int>>& matrix) {
		int n = matrix.size();
		if (n < 2)
			return;
		reverse(matrix.begin(), matrix.end()); // reverse row order
		for (int i = 0; i < n - 1; i++) {  // swap diagonally
			for (int j = i + 1; j < n; j++)
				swap(matrix[i][j], matrix[j][i]);
		}
	}
	vector<int> spiralOrder(vector<vector<int>>& matrix) {
		vector<int> ans;
		int left = 0;
		int top = 0;
		int bottom = matrix.size() - 1;
		if (bottom < 0)
			return ans;
		int right = matrix[0].size() - 1;
		if (right < 0)
			return ans;
		ans.reserve((bottom + 1)*(right + 1));
		int dir = 0;
		while (left <= right && top <= bottom) {
			switch (dir) {
			case 0:  // left to right, top row
				for (int i = left; i <= right; i++)
					ans.push_back(matrix[top][i]);
				++top;
				break;
			case 1: // top to bottom, right column
				for (int i = top; i <= bottom; i++)
					ans.push_back(matrix[i][right]);
				--right;
				break;
			case 2: // right to left, bottom row
				for (int i = right; i >= left; i--)
					ans.push_back(matrix[bottom][i]);
				--bottom;
				break;
			case 3: // bottom up, left column
				for (int i = bottom; i >= top; i--)
					ans.push_back(matrix[i][left]);
				++left;
				break;
			}
			dir = (dir + 1) % 4;
		}
		return ans;
	}

	// 59. Spiral Matrix II, Given a positive integer n, generate a square matrix filled with elements from 1 to n2 in spiral order.
	vector<vector<int>> generateMatrix(int n) {  // beat 74%, seems slower than just walk the spiral path
		vector<vector<int>> ans(n, vector<int>(n, 0));
		int i = 0, j = 0;
		int di = 0, dj = 1; // increment delta for i and j
		int k = 0;
		int n2 = n*n;
		while (k < n2) {
			ans[i][j] = ++k;
			if (i + di == n || j + dj == n || j + dj<0 || ans[i + di][j + dj]) {  // time to turn 
				int temp = di;
				di = dj;			// di=dj, dj=-di
				dj = -temp;
			}
			i += di;
			j += dj;
		}
		return ans;
	}
	// 289. Game of Life
	void gameOfLife(vector<vector<int>>& board) {
		if (board.empty())
			return;
		int m = board.size();
		if (board[0].empty())
			return;
		int n = board[0].size();
		vector<vector<int>> dir{ { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 },{ 0,1 },{ 1,-1 },{ 1,0 },{ 1,1 } };  // neighbors
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if ((board[i][j] & 1) == 0)  // first bit for live or die
					continue;
				for (auto& d : dir) {
					int ni = i + d[0];
					int nj = j + d[1];
					if (ni < 0 || ni >= m || nj < 0 || nj >= n)
						continue;  // invalid
					int count = board[ni][nj] >> 1;
					board[ni][nj] = ((++count) << 1) | (board[ni][nj] & 1);
				}
			}
		}
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				int count = board[i][j] >> 1;
				switch (count) {
				case 0:
				case 1:		board[i][j] = 0;	break;		// Any live cell with fewer than two live neighbors dies
				case 2:		board[i][j] = (board[i][j] & 1);	break;		// Any live cell with two or three live neighbors lives on to the next generation
				case 3:		board[i][j] = 1;	break;		// Any dead cell with exactly three live neighbors becomes a live cell
				default:	board[i][j] = 0;	break;		// Any live cell with more than three live neighbors dies
				}
			}
		}
	}
	// 661. Image Smoother
	// The value in the given matrix is in the range of [0, 255].  only 8 bits needed to store value
	vector<vector<int>> imageSmoother(vector<vector<int>>& M) {
		if (M.empty() || M[0].empty())
			return{};
		const static vector<vector<int>> dir{ { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 },{ 0,1 },{ 1,-1 },{ 1,0 },{ 1,1 } };
		int m = M.size();
		int n = M[0].size();
		//vector<vector<int>> ans(m, vector<int>(n, 0)); // use high 8 bits instead of extra memory, beat 66%
		for (int r = 0; r < m; r++) {
			for (int c = 0; c < n; c++) {
				int sum = M[r][c];
				int count = 1;
				for (const auto& d : dir) {
					int i = r + d[0];
					int j = c + d[1];
					if (i < 0 || j < 0 || i >= m || j >= n)
						continue;
					sum +=( M[i][j]&0xFF);
					count++;
				}
				M[r][c] |= ((sum / count)<<8);
			}
		}
		for (int r = 0; r < m; r++) {
			for (int c = 0; c < n; c++) {
				M[r][c] >>= 8;
			}
		}
		return M;
	}
	// 598. Range Addition II
	int maxCount(int m, int n, vector<vector<int>>& ops) { // beat 38%
		for (const auto& v : ops) {
			m = min(m, v[0]);  // overlapping rows and cols
			n = min(n, v[1]);
		}
		return m*n;
	}
};

TEST_CASE("generate matrix", "[GEN]")
{
	Array2D t;
	CHECK(t.generateMatrix(3) == vector<vector<int>>{ {1, 2, 3}, { 8, 9, 4 }, { 7, 6, 5 }});
}
TEST_CASE("Game of life", "[LIFE]")
{
	Array2D t;
	SECTION("normal case") {
		vector<vector<int>> b{ { 0,1,0 },{ 0,0,1 },{ 1,1,1 },{ 0,0,0 } };
		vector<vector<int>> ans{ { 0,0,0 },{ 1,0,1 },{ 0,1,1 },{ 0,1,0 } };
		t.gameOfLife(b);
		CHECK(b == ans);
	}
}
TEST_CASE("661. Image Smoother", "[NEW]")
{
	CHECK(Array2D().imageSmoother(vector<vector<int>>{ { 2, 3, 4 }, { 5, 6, 7 }, { 8, 9, 10 }, { 11, 12, 13 }, { 14, 15, 16 } }) == vector<vector<int>>{ {4, 4, 5}, { 5,6,6 }, { 8,9,9 }, { 11,12,12 }, { 13,13,14 }});
}

namespace std
{
	template<> struct hash<pair<int, int>>
	{
		typedef pair<int, int> argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& s) const noexcept
		{
			return s.first * 33 + s.second;
		}
	};
}

class Solution {
public:
	// 73. Set Matrix Zeroes
	void setZeroes(vector<vector<int>>& matrix) {
		if (matrix.empty())
			return;
		int m = matrix.size();
		if (matrix[0].empty())
			return;
		int n = matrix[0].size();
		// borrowed idea: store state of each row at left most col, store state of column at top row, faster, beat 82%
		int col0 = -1;  // state of first column
		for (int i = 0; i < m; i++) {
			if (matrix[i][0] == 0)
				col0 = 0;
			for (int j = 1; j < n; j++) {
				if (matrix[i][j] == 0) {
					matrix[i][0] = 0;
					matrix[0][j] = 0;
				}
			}
		}
		// bottom up to set each cell to 0 if row or col is 0
		for (int i = m - 1; i >= 0; i--) {
			for (int j = n - 1; j >= 1; j--) {
				if (matrix[i][0] == 0 || matrix[0][j] == 0)
					matrix[i][j] = 0;
			}
			if (col0 == 0)
				matrix[i][0] = 0;
		}
	}
	vector<int> flat(vector<vector<int>>& A)
	{
		vector<int> ans;
		int n = A.size();
		ans.reserve(n*n);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				if (A[i][j] > 0)
					ans.push_back(i * 100 + j);  // i*n did not work
		}
		return ans;
	}
	int largestOverlap(vector<vector<int>>& A, vector<vector<int>>& B) {
		vector<int> flatA = flat(A);
		vector<int> flatB = flat(B);
		unordered_map<int, int> count;
		for (auto i : flatA)
			for (auto j : flatB)
				count[i - j]++;   // count cell of same offset
		auto m = max_element(count.begin(), count.end(), [](auto&a, auto&b) {return a.second < b.second; });
		return m == count.end() ? 0 : m->second;
	}

	vector<pair<int, int>> flat2(vector<vector<int>>& A)
	{
		vector<pair<int, int>> ans;
		int n = A.size();
		ans.reserve(n*n);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				if (A[i][j] > 0)
					ans.emplace_back(i, j);
		}
		return ans;
	}
	int largestOverlap2(vector<vector<int>>& A, vector<vector<int>>& B) {
		vector<pair<int, int>> flatA = flat2(A);
		vector<pair<int, int>> flatB = flat2(B);
		if (flatA.empty() || flatB.empty())  // a few special cases, speed up 10% from 65 ms to 60 ms
			return 0;
		if (flatA.size() == 1 || flatB.size() == 1)
			return 1;
		int n = A.size();
		int n2 = n*n;
		if (flatA.size() == n2)
			return flatB.size();
		if (flatB.size() == n2)
			return flatA.size();
		unordered_map<pair<int, int>, int> count;
		for (auto i : flatA)
			for (auto j : flatB)
				count[{i.first - j.first, i.second - j.second}]++;   // count cell of same offset
		auto m = max_element(count.begin(), count.end(), [](auto&a, auto&b) {return a.second < b.second; });
		return m == count.end() ? 0 : m->second;
	}
	// 867. Transpose Matrix
	vector<vector<int>> transpose(vector<vector<int>>& A) {
		if (A.empty() || A[0].empty())
			return{};
		int R = A.size();
		int C = A[0].size();
		vector<vector<int>> ans(C, vector<int>());
		for (int c = 0; c < C; c++) {
			ans[c].reserve(R);
			for (int r = 0; r < R; r++)
				ans[c].push_back(A[r][c]);
		}
		return ans;
	}
	// 766. Toeplitz Matrix
	// A matrix is Toeplitz if every diagonal from top-left to bottom-right has the same element.
	bool isToeplitzMatrix(vector<vector<int>>& matrix) { // beat 98%
		if (matrix.empty() || matrix[0].empty())
			return true;
		int row = matrix.size();
		int col = matrix[0].size();
		for (int r = 1; r < row; r++)
		{
			for (int c = 1; c < col; c++)
				if (matrix[r][c] != matrix[r-1][c-1])
					return false;
		}
		return true;
	}
	// 566. Reshape the Matrix
	// Reshaped matrix need to be filled with all elements of original matrix in same row-traversing order as they were
	vector<vector<int>> matrixReshape(vector<vector<int>>& nums, int r, int c) {  // beat 100%, faster than vopy row by row
		if (nums.empty() || nums[0].empty())
			return{};
		int total = nums.size()*nums[0].size();
		if (total != r*c)
			return nums;
		vector<vector<int>> ans(r, vector<int>(c,0));
		int col = nums[0].size();
		for (int i = 0; i < total; i++)
			ans[i / c][i%c] = nums[i / col][i%col];
		return ans;
	}
};

TEST_CASE("set zero", "[ZERO]")
{
	Solution t;
	SECTION("normal case") {
		vector<vector<int>> b{ { 0,1,2,0 },{ 3,4,5,2 },{ 1,3,1,5 } };
		vector<vector<int>> ans{ { 0,0,0,0 },{ 0,4,5,0 },{ 0,3,1,0 } };
		t.setZeroes(b);
		CHECK(b == ans);
	}
	SECTION("edge case") {
		vector<vector<int>> b{ { 2, 7, 5, 7, 9, 9, 9, 4, 7 },{ 8, 9, 9, 2, 1, 3, 2, 8, 9 },{ 8, 9, 9, 2, 1, 3, 2, 8, 9 } };
		vector<vector<int>> ans = b;
		t.setZeroes(b);
		CHECK(b == ans);
	}
}

TEST_CASE("image overlap", "[LAP]")
{
	Solution t;
	SECTION("normal case") {
		CHECK(t.largestOverlap2(vector<vector<int>>{ {0, 1}, { 1,1 }}, vector<vector<int>>{ {1, 1}, { 1,0 }}) == 2);
	}
	SECTION("edge case") {
		CHECK(t.largestOverlap2(vector<vector<int>>{ {0}}, vector<vector<int>>{ {0}}) == 0);
	}
}