#include <vector>
#include <algorithm>
#include <map>

using namespace std;

// given N vertex convex polygon, find
using vi=vector<int>;
pair<vi, vi> split_vec(vi src, int start, int last)  // all points in a vector form a circle, split in 2
{
	if (start > 0)  // make starting place as first element
		(void)std::rotate(begin(src), begin(src)+start, end(src));
	last -= start;
	pair<vi, vi> ret;
	ret.first.assign(begin(src), begin(src) + last + 1); // [start, last]
	src.erase(begin(src)+1, begin(src) + last);  // delete [start+1, end-1]
	rotate(begin(src), begin(src) + 1, end(src));  // [end, start]
	ret.second = move(src);
	return ret;
}
int minScoreTriangulation(vector<int>& points, map<vi, int>& memo)
{
	if (points.size() == 3)
		return points[0] * points[1] * points[2];
	int result = INT32_MAX;
	// split a polygon into 2 valid polygons
	for (int i = 0; i < points.size() - 2; i++) {  // # of valid starting place
		// carefully find all possible second point given first point to split a polygon
		for (int j = 0; j < points.size() - 3; j++) { // # of valid ending place to split a polygon
			auto split = split_vec(points, i, j+i+2);  // end=i+2 + j
			result = min(result, minScoreTriangulation(split.first, memo)+ minScoreTriangulation(split.second, memo));
		}
	}
	return result;
}

int minScoreTriangulation(vector<int>& points, vector<vector<int>>& memo, int i, int j)
{
	if (j-i<2)
		return 0;
	if (memo[i][j] > 0)
		return memo[i][j];
	int result = INT32_MAX;
	// split a polygon into 2 valid polygons
	for (int k = i+1; k < j; k++) {  // # of valid starting place
		int middle = points[i] * points[j] * points[k];
		result = min(result, minScoreTriangulation(points, memo,i,k) +middle+ minScoreTriangulation(points, memo, k,j));
	}
	memo[i][j] = result;
	return result;
}

int minScoreTriangulation(vector<int>& points)
{
	//map<vi, int> memo;
	//return minScoreTriangulation(points, memo);
	int N = static_cast<int>(points.size());
	vector<vector<int>> memo(N, vector<int>(N,0));
	return minScoreTriangulation(points, memo, 0, N - 1);
}

#include "catch.hpp"
TEST_CASE("jam2019 1B #3", "[1B3]")
{
	CHECK(minScoreTriangulation(vector<int>{2, 7, 1, 9, 3, 1, 2, 2, 9, 2, 7, 1, 9, 3, 1, 2, 2, 9, 2, 7, 1, 9, 3, 1, 2, 2, 9, 2, 7, 1, 9, 3, 1, 2, 2, 9, 2, 7, 1, 9, 3, 1, 2, 2, 9, 2, 7, 1, 9, 3}) == 488);
	CHECK(minScoreTriangulation(vector<int>{2, 7, 1, 9, 3, 1, 2, 2, 9}) == 86);
	CHECK(minScoreTriangulation(vector<int>{2, 1, 4, 4}) == 24);  // fix loop count issue
	CHECK(minScoreTriangulation(vector<int>{3,7,4,5}) == 144);
	CHECK(minScoreTriangulation(vector<int>{1, 3, 1, 4, 1, 5}) == 13);
}