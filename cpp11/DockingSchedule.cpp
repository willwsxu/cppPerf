
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

// giving a schedule of warehouse truck loading, per day
// find out how many docks are required
// e.g. 12:00-13:00, 13:00-14:00 1 dock, clarify that it is ok to start or stop at same time
//      12:00-13:01, 13:00-14:00 2 docks
string get_next(const string& t, size_t& start, char sep) {
	auto last = find(begin(t) + start, end(t), sep);
	size_t len = distance(begin(t) + start, last);
	string ans = t.substr(start, len);
	start += len;
	while (t[start] == sep)  // move start to next token
		start++;
	return ans;
}
// find maximum interval overlaps
int compute_docks(vector<string> schedule) {
	map<int, int> count;
	for (const auto& interval : schedule) {
		size_t start = 0;
		int h1 = stoi(get_next(interval, start, ':'));
		int m1 = stoi(get_next(interval, start, '-'));
		int h2 = stoi(get_next(interval, start, ':'));
		int m2 = stoi(get_next(interval, start, ':'));
		count[h1 * 60 + m1] += 1;
		count[h2 * 60 + m2] += -1;
	}
	int max_val = 0;
	int sum = 0;
	for(const auto& c : count) {
		sum += c.second;
		max_val = max(max_val, sum);
	}
	return max_val;
}

// find all cells from following selection
// A1:C3, A1&B2, or combinations
void parse_cell_range(string& sel, vector<string>& cells)
{
	auto colon = find(begin(sel), end(sel), ':');
	if (colon == end(sel))
		cells.push_back(sel);
	else {
		char col1 = sel[0]; // assume column name from A to Z
		char col2 = *(colon + 1);
		int len = distance(begin(sel)+1, colon);
		int row1 = stoi(sel.substr(1, len));
		int row2 = stoi(sel.substr(len + 3));
		for (char c = col1; c <= col2; c++) {
			for (int r = row1; r <= row2; r++) {
				string s(1, c);
				s.append(to_string(r));
				cells.push_back(s);
			}
		}
	}
}
vector<string> excel_cells(string selection)
{
	vector<string> cell_ranges; // all ranges
	istringstream strm(selection);
	for (string r; getline(strm, r, '&'); ) {
		parse_cell_range(r, cell_ranges);
	}
	return cell_ranges;
}
#include "..\catch.hpp"
TEST_CASE("GoHealth compute docks test", "[TEST]")
{
	CHECK(compute_docks({ "12:00-13:00" }) == 1);
	CHECK(compute_docks({ "12:00-13:00", "13:00-14:00" }) == 1);
	CHECK(compute_docks({ "12:00-13:01", "13:00-14:00" }) == 2);
	CHECK(compute_docks({ "12:00-13:00", "13:00-14:00","12:00-15:00" }) == 2);
	CHECK(compute_docks({ "12:00-13:01", "13:00-14:00","12:00-15:00" }) == 3);
}

TEST_CASE("GoHealth excel spreadsheet selection test", "[TEST]")
{
	CHECK(excel_cells("A1") == vector<string>{"A1"});
	CHECK(excel_cells("A1&B2") == vector<string>{"A1","B2"});
	CHECK(excel_cells("A1:B1") == vector<string>{"A1", "B1"});
	CHECK(excel_cells("A1:A2") == vector<string>{"A1", "A2"});
	CHECK(excel_cells("A1:B2") == vector<string>{"A1", "A2", "B1", "B2"});
	CHECK(excel_cells("A123:B125&C4567&C3000:D3001") == vector<string>{"A123", "A124", "A125", "B123", "B124", "B125", "C4567","C3000","C3001","D3000","D3001"});
}
