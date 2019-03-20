#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;
// problem description: given a panccake of size RxC, each cell may or may not contain a chocolate chip
//   Is it possible to cut it so each piece contain same chips, H cuts horizontally, V cuts vertically

// count chips on a rentangle piece after cut
int count_chips(const vector<string>& grid, int rs, int re, int cs, int ce)  // rectangle (rs, re), (cs, ce)
{
	int chips = 0;
	for (; rs < re; rs++)
		chips += std::count(begin(grid[rs])+cs, begin(grid[rs])+ce, '@');
	return chips;
}
// ffor valid row cuts, search valid col cut one at a time
bool cut_vertical(const vector<string>& grid, const vector<int>& h_cuts, int v_cut, const int V, const int chips)
{
	if (V<0)  // decrement V each call until -1
		return true;
	int c = 0 == V ? grid[0].size() : v_cut + 1;  // special case for last cut, use all remaining columns
	for (; c <= (int)grid[0].size(); c++) {
		int good = 0;  // check row cut by row cut for this column
		for (int i = 0; i < (int)h_cuts.size() - 1; i++) {
			int count = count_chips(grid, h_cuts[i], h_cuts[i + 1], v_cut, c);
			if (count == chips)
				good++;
			else if (count > chips)
				return false;
		}
		if (good == h_cuts.size() - 1) {
			if (cut_vertical(grid, h_cuts, c, V-1, chips))
				return true;
		}
	}
	return false;
}
bool cut_horizonal(const vector<string>& grid, vector<int>& h_cuts, const int H, const int V, const int chips)
{
	if (h_cuts.size() == H + 1) { // last cut, just add last row value
		h_cuts.push_back(grid.size());
		bool ans = cut_vertical(grid, h_cuts, 0, V, chips);
		h_cuts.pop_back();
		return ans;
	}
	int row_start = h_cuts.back();
	int r = h_cuts.back() + 1;
	h_cuts.push_back(r);
	for (; r <= int(grid.size() - H + h_cuts.size() - 1); r++) {
		int count = count_chips(grid, row_start, r, 0, grid[0].size()); // count chips per horizontal cut to prune invalid
		if (count > chips*(V + 1)) // too many chips
			break;
		else if (count < chips*(V + 1))  // not enough chips
			continue;
		h_cuts.back() = r;
		if (cut_horizonal(grid, h_cuts, H, V, chips))
			return true;
	}
	h_cuts.pop_back();
	return false;
}

bool pancake_cuts(const vector<string>& grid, int H, int V) {
	int total_chips = 0;
	for (const auto& row : grid)
		total_chips += count(begin(row), end(row), '@');
	int chips = total_chips / (H + 1) / (V + 1);
	if (chips*(H + 1)*(V + 1) != total_chips)
		return false;
	vector<int> h_cuts{0};  // [0,1,2] example for 1 cut 2 rows
	return cut_horizonal(grid, h_cuts, H, V, chips);
}
void test_online()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int rows, cols, H, V;
		cin >> rows >> cols >> H >> V;
		string line;
		getline(cin, line);
		vector<string> grid;
		for (int i = 0; i < rows; i++) {
			getline(cin, line);
			grid.push_back(move(line));
		}
		cout << "Case #" << t;
		if (pancake_cuts(grid, H, V))
			cout << ": POSSIBLE\n";
		else
			cout << ": IMPOSSIBLE\n";
	}
}
int main()
{
	vector<string> test5{ "@.@@","@@.@","@.@@" };
	cout << pancake_cuts(test5, 2, 2) << "\n";
	vector<string> test1{ ".@@..@", ".....@", "@.@.@@" };
	//cout << first_cut(test1, 2, 4, 1, 1, 2) << "\n";
	cout << pancake_cuts(test1, 1, 1) << "\n";
	test_online();
	return 0;
}
