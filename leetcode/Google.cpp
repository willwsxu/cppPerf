#include "..\catch.hpp"  // don't put this file in stdafx.h
#include <vector>
#include <string>
#include <iterator>
#include <iostream>

using namespace std;

/*Question: We are interested in getting 7 digit phone number by watching a chess piece(rook) traverse a board with numbers on each board position.

	Given a 3 by 3 board that looks like this.

	1 2 3
	4 5 6
	7 8 9

	And assuming that a rook starts on position 4, write a function which provides all of the combinations of 7 digit numbers which start with 4.

	For example :

		  456 - 3214 would be generated when the rook moves
		  { right one, right one, up one, left one, left one, down one }

			  464 - 6464 would be generated when the rook moves
		  { right two, left two, right two, left two, right two, left two }
		  */

vector<int> find_rook_moves(vector<vector<int>>& digits, int from) {
	vector<int> moves{};
	int i = (from - 1) / digits[0].size();
	int j = (from - 1) % digits[0].size();
	for (size_t k = 0; k < digits[0].size(); k++) {
		if (digits[i][k] != from)
			moves.push_back(digits[i][k]);
		if (digits[k][j] != from)
			moves.push_back(digits[k][j]);
	}
	return moves;
}
void phone_numbers(vector<vector<int>>& digits, vector<string>& result, int count)
{
	if (count == 7)
		return;
	vector<string> dup = result;
	result.clear();
	for (auto& prefix : dup) {
		auto moves = find_rook_moves(digits, prefix.back() - '0');
		prefix.append(1, '0'); // add a place holder
		for (int next : moves) {
			prefix.back() = next + '0';
			result.push_back(prefix);
		}
	}
	phone_numbers(digits, result, count + 1);
}

vector<string> phone_numbers(vector<vector<int>> digits, int num)
{
	vector<string> result{ string(1, num + '0') };
	phone_numbers(digits, result, 1);
	return result;
}

TEST_CASE("google phone number test", "[NEW]")
{
	auto numbers = phone_numbers(vector<vector<int>>{ {1, 2, 3}, { 4,5,6 }, { 7,8,9 }}, 4);
	//copy(begin(numbers), end(numbers), ostream_iterator<string>(cout, " "));
	CHECK(numbers.size() == (1 << 12));
}