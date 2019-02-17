#include <vector>
#include <algorithm>
using namespace std;

class TicTacToeBoard
{
	enum Piece { PLAYER = 'X', COMPUTER = 'O' };
	static const int DIMENSION = 3;
	vector<char> board;
public:
	TicTacToeBoard() :board(DIMENSION*DIMENSION, ' ') {}
	void set_move(char piece, int row, int col) {
		board[row*DIMENSION + col] = piece;
	}
	bool row_won(int r) {
		r *= DIMENSION;
		return board[r + 1] == board[r] && board[r + 2] == board[r] && board[r]!=' ';
	}
	bool col_won(int c) {
		return board[c + 3] == board[c] && board[c + 6] == board[c] && board[c] != ' ';
	}
	bool forward_diag_won()  // 2 4 6
	{
		return board[2] == board[4] && board[2] == board[6] && board[2] != ' ';
	}
	bool backward_diag_won()  // 0 4 8
	{
		return board[0] == board[4] && board[0] == board[8] && board[4] != ' ';
	}
	bool game_over() const {
		return find(begin(board), end(board), ' ') == end(board);
	}
	bool is_blank(int r, int c) const {
		return board[r*DIMENSION + c] == ' ';
	}
	int evaluate() {  // PLAYER won 10, or -10 if computer won
		auto score = [](char side) { return side == PLAYER ? 10 : -10; };
		for (int i = 0; i<DIMENSION; i++) {
			if (row_won(i))
				return score(board[i * 3]);
			if (col_won(i))
				return score(board[i]);
		}
		if (forward_diag_won() || backward_diag_won())
			return score(board[4]);
		return 0;
	}
	int minimax(int depth, bool isMax, int alpha, int beta)
	{
		int score = evaluate();
		if (score == 10)
			return score - depth;
		else if (score == -10)
			return score + depth;
		if (game_over())
			return 0;
		// compute score of next move
		score = isMax ? -1000 : 1000;
		for (int r = 0; r<DIMENSION; r++) {
			for (int c = 0; c<DIMENSION; c++) {
				if (!is_blank(r, c))
					continue;
				set_move(isMax ? PLAYER : COMPUTER, r, c);
				int next_score = minimax(depth + 1, !isMax, alpha, beta);
				score = isMax ? max(score, next_score) : min(score, next_score);
				set_move(' ', r, c); // roll back
				if (isMax)
					alpha = max(alpha, score);
				else
					beta = min(score, beta);
				if (beta <= alpha) {
					break;
				}
			}
		}
		return score;
	}
	pair<int, int> find_best_move(bool isMax)
	{
		int score = isMax ? -10000 : 10000;
		pair<int, int> best;

		for (int r = 0; r<DIMENSION; r++) {
			for (int c = 0; c<DIMENSION; c++) {
				if (!is_blank(r, c))
					continue;
				set_move(isMax ? PLAYER : COMPUTER, r, c);
				int next_score = minimax(0, !isMax, -10000, 10000);
				set_move(' ', r, c); // roll back
				if (isMax && next_score > score || !isMax && next_score < score) {
					score = next_score;
					best.first = r;
					best.second = c;
				}
			}
		}
		return best;
	}
};


#include "..\catch.hpp"
TEST_CASE("Tic Tac Toe test", "[TEST]")
{
	SECTION("test 1") {
		TicTacToeBoard board;
		board.set_move('X', 2, 1);
		board.set_move('X', 2, 2);
		board.set_move('O', 2, 0);
		board.set_move('O', 1, 1);
		CHECK(board.find_best_move(true) == pair<int, int>{0, 2});
	}
	SECTION("test 2") {
		TicTacToeBoard board;
		board.set_move('X', 2, 0);
		board.set_move('X', 0, 2);
		board.set_move('O', 0, 0);
		CHECK(board.find_best_move(false) == pair<int, int>{1, 1});
	}
}