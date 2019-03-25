#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>

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

bool pancake_cuts(const vector<string>& grid, const int H, const int V) {
	int total_chips = 0;
	for (const auto& row : grid)
		total_chips += std::count(begin(row), end(row), '@');
	int chips = total_chips / (H + 1) / (V + 1);
	if (chips*(H + 1)*(V + 1) != total_chips)
		return false;
	vector<int> h_cuts{0};  // [0,1,2] example for 1 cut 2 rows
	return cut_horizonal(grid, h_cuts, H, V, chips);
}

// implement editorial idea
bool pancake_cuts_edit(const vector<string>& grid, const int H, const int V) {
	vector<int> row_count;
	row_count.reserve(grid.size());
	for (const auto& row : grid) {
		row_count.push_back(std::count(begin(row), end(row), '@'));
	}
	partial_sum(begin(row_count), end(row_count), begin(row_count));
	int chips = row_count.back() / (H + 1) / (V + 1);
	if (chips*(H + 1)*(V + 1) != row_count.back())
		return false;
	int row_cut_count = row_count.back() / (H + 1); // chip count per row cut
	int col_cut_count = row_count.back() / (V + 1);	// chip count per col cut
	int h_cut = row_cut_count;
	vector<int> h_cuts{ 0 };  // [0,1,2] example for 1 cut 2 rows
	for (size_t r = 0; r < row_count.size(); r++) {  // verify each row cut
		if (row_count[r] == h_cut) {
			h_cuts.push_back(r + 1);
			h_cut += row_cut_count;
		}
		else if (row_count[r] > h_cut)
			return false;
	}
	if (h_cut - row_cut_count != row_count.back())
		return false;
	int col_start = 0;
	int col_count = 0;
	for (size_t c = 0; c < grid[0].size(); c++) {
		for (size_t r = 0; r < grid.size(); r++) {
			if (grid[r][c] == '@')
				col_count++;
		}
		if (col_count > col_cut_count)
			return false;
		if (col_count < col_cut_count)
			continue;
		for (size_t r = 1; r < h_cuts.size(); r++) {
			if (count_chips(grid, h_cuts[r - 1], h_cuts[r], col_start, c + 1) != chips)
				return false;
		}
		col_start = c + 1;
		col_count = 0;
	}
	return true;
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
		if (pancake_cuts_edit(grid, H, V))
			cout << ": POSSIBLE\n";
		else
			cout << ": IMPOSSIBLE\n";
	}
}
void test1A()
{
	vector<string> test5{ "@.@@","@@.@","@.@@" };
	cout << pancake_cuts_edit(test5, 2, 2) << "\n";
	vector<string> test1{ ".@@..@", ".....@", "@.@.@@" };
	//cout << first_cut(test1, 2, 4, 1, 1, 2) << "\n";
	cout << pancake_cuts_edit(test1, 1, 1) << "\n";
	test_online();
}

// Given C cashiers, each can checkout at most Mi bits, takes Si for each bit, plus cost Pi for packaging
// There are R robot shopper for B bits in total, find optimal distribution so check out time is shortest
// R<=C, each robot can check out at most once
int item_from_cost(long long c, int unit_cost, int extra_cost, int capacity)
{
	if (c < extra_cost)
		return 0;
	return static_cast<int>(min<long long>((c - extra_cost) / unit_cost, capacity));
}
long long bitParty(int R, int B, const vector<int>& M, const vector<int>& S, const vector<int>& P, long long low, long long hi)
{
	if (low >= hi)
		return low;
	int C = M.size();
	long long mid = (low + hi) / 2;
	vector<int> items;
	items.reserve(C);
	for (int i = 0; i < C; i++) {
		items.push_back(item_from_cost(mid, S[i], P[i], M[i]));
	}
	sort(begin(items), end(items), greater<int>());
	long long total = accumulate(begin(items), begin(items) + R, 0LL); // only robots can shop
	if (total < B)
		return bitParty(R, B, M, S, P, mid + 1, hi);
	return bitParty(R, B, M, S, P, low, mid);  // don't use mid-1 as total>B is valid answer!
}
long long bitParty(int R, int B, const vector<int>& M, const vector<int>& S, const vector<int>& P)
{
	long long max_time = B;
	max_time = *max_element(begin(S), end(S)) * max_time + *max_element(begin(P), end(P));
	return bitParty(R, B, M, S, P, 1, max_time);
}
void test_online2()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int R, B, C;
		cin >> R >> B >> C;
		vector<int> M, S, P;
		for (int i = 0; i < C; i++) {
			int m, s, p;
			cin >> m >> s >> p;
			M.push_back(m);
			S.push_back(s);
			P.push_back(p);
		}
		cout << "Case #" << t;
		cout << ": " << bitParty(R, B, M, S, P) << "\n";
	}
}
void test1A2()
{
	vector<int> M{1000000000,1000000000};
	vector<int> S{900000000,1000000000};
	vector<int> P{400000000,200000000};
	cout << bitParty(2,2,M,S,P) << "\n";  // 1300000000
}
void test1A2t2()
{
	vector<int> M{ 1000000000,1000000000,1000000000,1000000000 };
	vector<int> S{ 900000000,1000000000,800000000,700000000 };
	vector<int> P{ 400000000,200000000, 600000000,800000000  };
	cout << bitParty(4, 10, M, S, P) << "\n";   //  3000000000 (bits 2, 2, 3, 3)
	cout << bitParty(3, 100, M, S, P) << "\n";  // 27400000000 (bits 30, 27, 33, 38)
}
int main()
{
	test1A2t2();
	return 0;
}
