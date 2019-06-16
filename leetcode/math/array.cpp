#include <vector>
#include <map>
#include <algorithm>
using namespace std;

// for each 0, duplicate and shift all numbers to right
// numbers drop off if exceeded array size
void duplicateZeros(vector<int>& arr) {
    int size = arr.size();
    int shift_right = 0;
    int last_dup = -1;
    for (int i = 0; i < size - shift_right - 1; i++) {
        if (arr[i] == 0) {
            shift_right++;
            last_dup = i;
        }
    }
    for (int i = size - 1; i > 0; i--) {
        if (shift_right == 0)
            break;
        arr[i] = arr[i - shift_right];
        if (arr[i] == 0 && i - shift_right <= last_dup) {
            arr[--i] = 0;
            shift_right--;
        }
    }
}


#include "catch.hpp"
TEST_CASE("1089. Duplicate Zeros", "[ARR]")
{
    vector<int> arr3{ 1,5,2,0,6,8,0,6,0 };
    duplicateZeros(arr3);
    CHECK(arr3 == vector<int>{ 1, 5, 2, 0, 0, 6, 8, 0, 0});

    vector<int> arr2{ 9, 9, 9, 4, 8, 0, 0, 3, 7, 2, 0, 0, 0, 0, 9, 1, 0, 0, 1, 1, 0, 5, 6, 3, 1, 6, 0, 0, 2, 3, 4, 7, 0, 3, 9, 3, 6, 5, 8, 9, 1, 1, 3, 2, 0, 0, 7, 3, 3, 0, 5, 7, 0, 8, 1, 9, 6, 3, 0, 8, 8, 8, 8, 0, 0, 5, 0, 0, 0, 3, 7, 7, 7, 7, 5, 1, 0, 0, 8, 0, 0 };
    duplicateZeros(arr2);
    vector<int> ans2{ 9, 9, 9, 4, 8, 0, 0, 0, 0, 3, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0, 9, 1, 0, 0, 0, 0, 1, 1, 0, 0, 5, 6, 3, 1, 6, 0, 0, 0, 0, 2, 3, 4, 7, 0, 0, 3, 9, 3, 6, 5, 8, 9, 1, 1, 3, 2, 0, 0, 0, 0, 7, 3, 3, 0, 0, 5, 7, 0, 0, 8, 1, 9, 6, 3, 0, 0, 8, 8, 8, 8, 0 };
    CHECK(arr2 == ans2);

    vector<int> arr{ 1,0,2,3,0,4,5,0 };
    duplicateZeros(arr);
    CHECK(arr == vector<int>{1, 0, 0, 2, 3, 0, 0, 4});
}
