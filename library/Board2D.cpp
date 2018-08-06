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
};
