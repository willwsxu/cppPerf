#include "..\catch.hpp"

#include <vector>
#include <iostream>
#include <random>

using namespace std;
class Board
{
	enum Cell_State { Opened, Unopened, Marked };
	struct Cell
	{
		int		mine=0; // -1 mine or count of mines surround
		Cell_State		state= Unopened;
	};
	vector<vector<Cell>>  board;
	int rows, cols;
	void shuffleBoard()
	{
		std::random_device rd;
		thread_local std::mt19937 engine(rd());
		int cells = rows*cols;
		for (int i = 0; i < cells-1; i++) {
			uniform_int_distribution<> dis(i, cells-1);
			int to = dis(engine);
			if (i != to) {
				std::swap(board[i / rows][i%rows], board[to / rows][to%rows]);
			}
		}
	}
	void computeMinesAroundCell()
	{
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (board[i][j].mine >= 0)
					continue;
				vector<vector<int>> dir{ {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{ 1,-1 },{ 1,0 },{ 1,1 } };
				for (const auto& d : dir) {  // add 1 to all cells surrounding a mine
					int r = i + d[0];
					int c = j + d[1];
					if (r >= 0 && c >= 0 && r < rows && c < cols)
						board[r][c].mine++;
				}
			}
		}
	}
public:
	Board(int r, int c, int mines):board(r, vector<Cell>(c)), rows(r), cols(c)
	{
		for (int i = 0; i < mines; i++) {
			board[i / r][i%c].mine = -1;
		}
		shuffleBoard();
		computeMinesAroundCell();
	}

	bool play(int r, int c) {  // open this cell
		return true;  // return false if bomb
	}
	void mark(int r, int c) {  // mark a cell as bomb

	}
	void print(bool debug=false) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (!debug && board[i][j].state == Unopened)
					cout << "-";
				else if (board[i][j].mine < 0)
					cout << '!';
				else if (board[i][j].mine == 0)
					cout << " ";
				else
					cout << board[i][j].mine;
				cout << "|";
			}
			cout << "\n";
		}
	}
	bool done() {
		return true;
	}
};

class MineSweeperGame
{
	bool playBame(Board& board)
	{
		while (true) {
			board.print();
			cout << "make your move: m r c\n";
			cout << "or mark a cell as bomb: b r c\n";
			cout << "To quit at any time, enter: Q\n";
			char move;
			cin >> move;
			if (move == 'Q')
				return false;
			int r, c;
			cin >> r >> c;
			if (move == 'm') {
				if (!board.play(r, c))
					return false;
				if (board.done())
					return true;
			}
			else
				board.mark(r, c);
		}
		return true;
	}
public:
	MineSweeperGame() {

	}
	void GameLoop() {
		while (true) {
			cout << "To start a new Game, enter: N rows cols mines\n";
			cout << "To quit at any time, enter: Q\n";
			char play_game;
			cin >> play_game;
			if (play_game == 'Q' || play_game == 'q')
				break;
			if (play_game == 'N') {
				int rows, cols, mines;
				cin >> rows >> cols >> mines;
				Board board(rows, cols, mines);
				if (playBame(board))
					cout << "You won! ";
				cout << "Play again?\n";
			}
		}
	}
};


TEST_CASE("minesweeper test", "[NEW]")
{
	Board board(10, 10, 20);
	board.print(true);
	board.print();
}