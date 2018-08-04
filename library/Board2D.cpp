#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"

using namespace std;


class Solution {
public:
	// 419. Battleships in a Board, X ship, . space
	// Battleships can only be placed horizontally or vertically, width is 1
	// there are no adjacent battleships
	int countBattleships(vector<vector<char>>& board) { // beat 8%
		if (board.empty() || board[0].empty())
			return 0;
		int count = 0;
		int row = board.size();
		int col = board[0].size();
		if (board[0][0] == 'X')
			++count;
		for (int c = 1; c < col; c++) {  // scan first row
			if (board[0][c] == 'X' && board[0][c - 1] == '.')
				count++;
		}
		for (int r = 1; r < row; r++) {  // scan from second row
			if (board[r][0] == 'X' && board[r - 1][0] == '.')
				++count;
			for (int c = 1; c < col; c++) {
				if (board[r][c] == '.')
					continue;
				if (board[r][c - 1] == 'X' || board[r - 1][c] == 'X')
					continue;
				count++;
			}
		}
		return count;
	}
};
