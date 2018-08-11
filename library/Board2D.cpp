#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"

using namespace std;


class Solution {
public:
	// 419. Battleships in a Board, X ship, . space
	// Battleships can only be placed horizontally or vertically, width is 1
	// there are no adjacent battleships
	int countBattleships(vector<vector<char>>& board) { // beat 98%
		if (board.empty() || board[0].empty())
			return 0;
		int count = 0;
		int row = board.size();
		int col = board[0].size();
		for (int r = 0; r < row; r++) {  // scan each row
			if (board[r][0] == 'X' && (r==0 ||board[r - 1][0] == '.'))
				++count; // check first column
			for (int c = 1; c < col; c++) {
				if (board[r][c] == '.')
					continue;
				if (board[r][c - 1] == 'X' || (r>0 &&board[r - 1][c] == 'X'))
					continue;
				count++;
			}
		}
		return count;
	}

public:
	// 807. Max Increase to Keep City Skyline
	// grid dim [2,50]
	int maxIncreaseKeepingSkyline(vector<vector<int>>& grid) {// same code submit 3 times, 12ms, 8ms, 4ms
		int R = grid.size();
		int C = grid[0].size();
		vector<int> t_d(C, 0); // top down view of skyline
		vector<int> l_r(R, 0); // left right view of skyline
		for (int r = 0; r < R; r++) {
			for (int c = 0; c < C; c++) {
				t_d[c] = max(t_d[c], grid[r][c]);
				l_r[r] = max(l_r[r], grid[r][c]);
			}
		}
		int inc = 0;
		for (int r = 0; r < R; r++) {
			for (int c = 0; c < C; c++) {
				inc += min(t_d[c], l_r[r]) - grid[r][c];  // increase must not affect either skyline
			}
		}
		return inc;
	}

	// 861. Score After Flipping Matrix, dim [1,20], each value id bit 0 or 1
	// flip all bits in a row or column, find max
	// idea: for each row, flip if left most is 0
	//       for each col,  flip if there are more 0s than 1
	int matrixScore(vector<vector<int>>& A) {
		int R = A.size();
		int C = A[0].size();
		for (int r = 0; r < R; r++) {  // check each row first
			if (A[r][0])
				continue;
			for (int c = 0; c < C; c++) {
				A[r][c] = 1 - A[r][c];  // flip bit so high bit becomes 1
			}
		}
		int ans = 0;
		for (int c = 0; c < C; c++) {
			int count[2] = { 0 };
			for (int r = 0; r < R; r++)
				count[A[r][c]]++;
			//if (count[0] > count[1]) {  //more zero than 1, flip
			//	for (int r = 0; r < R; r++)
			//		A[r][c] = 1- A[r][c];
			//}
			// directly compute, using higher count as 1 at this position
			ans = (ans << 1) + max(count[0], count[1]);
		}
		return ans;
	}
};
