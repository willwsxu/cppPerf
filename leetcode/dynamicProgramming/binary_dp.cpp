#include <vector>
#include <algorithm>

using namespace std;

int mctFromLeafValues(vector<int>& arr, int i, int j) 
{
    if (i == j)
        return 0;
    int result=INT32_MAX;
	// try all subtree [i, k], [K+1, j]
    for (int k = i; k < j; k++) {
        int left_max = *max_element(begin(arr)+i, begin(arr)+k+1);
        int right_max = *max_element(begin(arr) + k+1, begin(arr) + j + 1);
        result = min(result, left_max * right_max + mctFromLeafValues(arr, i, k) + mctFromLeafValues(arr, k + 1, j));
    }
    return result;
}
int mctFromLeafValues(vector<int>& arr) {
    return mctFromLeafValues(arr, 0, (int)arr.size()-1);
}

#include <catch.hpp>
TEST_CASE("1130. Minimum Cost Tree From Leaf Values", "[DYN]")
{
    CHECK(mctFromLeafValues(vector<int>{6,2,4}) == 32);
}