#include "..\catch.hpp"

#include <vector>
#include <deque>
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
	vector<vector<int>> dir{ { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 },{ 0,1 },{ 1,-1 },{ 1,0 },{ 1,1 } };
	vector<vector<Cell>>  board;
	int		opened = 0;
	int		mines;
	int rows, cols;

	bool valid(int r, int c) {
		return r >= 0 && c >= 0 && r < rows && c < cols;
	}
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
				for (const auto& d : dir) {  // add 1 to all cells surrounding a mine
					int r = i + d[0];
					int c = j + d[1];
					if ( valid (r,c) && board[r][c].mine>=0 )
						board[r][c].mine++;
				}
			}
		}
	}
	void openBlankCells(int r, int c)
	{
		deque<pair<int, int>> q{ {r,c} };
		while (!q.empty()) {
			auto blank = q.front();
			q.pop_front();
			for (const auto& d : dir) {  // add 1 to all cells surrounding a mine
				int r = blank.first + d[0];
				int c = blank.second + d[1];
				if (valid(r, c) && board[r][c].state != Opened && board[r][c].mine >= 0) {
					opened++;
					board[r][c].state=Opened;
					if (board[r][c].mine == 0)
						q.emplace_back(r, c);
				}
			}
		}
	}
public:
	Board(int r, int c, int mines):board(r, vector<Cell>(c)), rows(r), cols(c),mines(mines)
	{
		for (int i = 0; i < mines; i++) {
			board[i / r][i%c].mine = -1;
		}
		shuffleBoard();
		computeMinesAroundCell();
	}

	bool play(int r, int c) {  // open this cell
		if (!valid(r, c) || board[r][c].state==Opened) {
			cout << "invalid move or already opened, try again\n";
			return true;
		}
		if (board[r][c].mine < 0)  // bomb, lost
			return false;
		opened++;
		board[r][c].state = Opened;
		if (board[r][c].mine == 0)
			openBlankCells(r, c);
		return true;  // return false if bomb
	}
	void mark(int r, int c) {  // mark a cell as bomb
		if (!valid(r, c) || board[r][c].state == Opened) {
			cout << "invalid move or already opened, try again\n";
			return;
		}
		if (board[r][c].state == Unopened)  // toggle buess if cell is a bomb
			board[r][c].state = Marked;
		else
			board[r][c].state = Unopened;
	}
	void print(bool debug=false) {
		cout << " |0|1|2|3|4|5|6|7|8|9|\n";
		for (int i = 0; i < rows; i++) {
			cout << i << "|";
			for (int j = 0; j < cols; j++) {
				if (debug || board[i][j].state == Opened) {
					if (board[i][j].mine < 0)
						cout << '!';
					else if (board[i][j].mine == 0)
						cout << " ";
					else
						cout << board[i][j].mine;
				}
				else {
					cout <<( board[i][j].state == Marked? "?": "-");
				}
				cout << "|";
			}
			cout << "\n";
		}
	}
	bool done() {
		return opened + mines == rows*cols;
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
				board.print(true);
				if (playBame(board))
					cout << "You won! ";
				cout << "Play again?\n";
			}
		}
	}
};


TEST_CASE("minesweeper test", "[NO]")
{
	//Board board(10, 10, 20);
	//board.print(true);
	//board.print();

	//MineSweeperGame().GameLoop();
}