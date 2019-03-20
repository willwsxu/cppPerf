#include<iostream>
#include <utility>
#include <functional>
#include<vector>
#include <map>
#include <deque>
using namespace std;

void next_move(int posr, int posc, vector <string> board) {
	//add logic here
	if (board[posr][posc] == 'd') {
		cout << "CLEAN\n";
		return;
	}
	using pii = pair<int, int>;
	map<pii, pii> parent;  // parent position of next move
	auto back_trace = [&parent](const pii& root, pii last)
	{
		for (;;) {
			pii& p = parent[last];
			if (p == root)
				break;
			last = p;
		}
		if (root.first == last.first) { // same row
			if (root.second>last.second)
				cout << "LEFT\n";
			else
				cout << "RIGHT\n";
		}
		else {
			if (root.first>last.first)
				cout << "UP\n";
			else
				cout << "DOWN\n";
		}
	};
	deque<pii>  q{ { posr,posc } };
	while (!q.empty()) {
		pii p = q.front();
		q.pop_front();
		if (board[p.first][p.second] == 'd') {
			back_trace({ posr, posc }, p);
			break;
		}
		static const vector<vector<int>> dir{ { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 } };  // up down left right
		for (const auto& d : dir) {
			int r = p.first + d[0];
			int c = p.second + d[1];
			if (r<0 || c<0 || r >= (int)board.size() || c >= (int)board[0].size())
				continue;
			auto found = parent.find({ r,c });
			if (found != end(parent))
				continue;
			parent[make_pair(r, c)] = p;
			q.emplace_back(r, c);
		}
	}
}