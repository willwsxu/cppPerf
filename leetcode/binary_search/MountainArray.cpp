#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

int peakIndexInMountainArray(vector<int>& A, int lo, int hi) {
    int mid = (lo + hi) / 2;
    if (mid == lo)
        return hi;
    if (A[mid] > A[mid - 1] && A[mid] > A[mid + 1])
        return mid;
    if (A[mid] > A[mid - 1] && A[mid] < A[mid + 1])
        return peakIndexInMountainArray(A, mid, hi);
    return peakIndexInMountainArray(A, lo, mid);
}
// 852. Peak Index in a Mountain Array
int peakIndexInMountainArray(vector<int>& A) {  // beat 98%
    return peakIndexInMountainArray(A, 0, static_cast<int>(A.size() - 1));
}
class MountainArray {
    vector<int> _mountain;
public:
    MountainArray(vector<int> arr) :_mountain(move(arr)) {}
    int get(int k) {
        if (k >= _mountain.size()) {
            cout << "out of bounds\n";
            return -1;
        }
        return _mountain[k];
    }
    int length() const {
        return (int)_mountain.size();
    }
};
class MountainArrayQuery {
    void query(MountainArray& mountainArr, int k)
    {
        if (mountain[k] < 0)
            mountain[k] = mountainArr.get(k);
    }
    int find_top(MountainArray& mountainArr, int low, int high) {
        if (low == high)
            return low;
        if (high - low == 1) {
            if (mountain[high] > mountain[low])
                return high;
            return low;
        }
        int mid = (low + high) / 2;
        query(mountainArr, mid);
        query(mountainArr, mid - 1);
        query(mountainArr, mid + 1);
        if (mountain[mid - 1] < mountain[mid] && mountain[mid] < mountain[mid + 1])
            return find_top(mountainArr, mid + 1, high);
        if (mountain[mid - 1] > mountain[mid] && mountain[mid] > mountain[mid + 1])
            return find_top(mountainArr, low, mid - 1);
        return mid;
    }
    // iterative binary search
    int find_top_loop(MountainArray& mountainArr, int low, int high) {
        while (low < high) {
            int mid = (low + high) / 2;
            query(mountainArr, mid);
            query(mountainArr, mid + 1);
            if (mountain[mid] < mountain[mid + 1])  // low to mid+1 increasing
                low = mid + 1;
            if (mountain[mid] > mountain[mid + 1])  // mid to high decreasing
                high = mid;
        }
        return low;
    }
    int findInMountainArrayInc(int target, MountainArray& mountainArr, int low, int high) {  // inclusive
        if (low >= high)
            return low;
        int mid = (low + high) / 2;
        query(mountainArr, mid);
        if (target == mountain[mid])
            return mid;
        if (target > mountain[mid])
            return findInMountainArrayInc(target, mountainArr, mid + 1, high);
        return findInMountainArrayInc(target, mountainArr, low, mid - 1);
    }
    int findInMountainArrayInc_loop(int target, MountainArray& mountainArr, int low, int high) {  // inclusive
        while (low<=high) {
            int mid = (low + high) / 2;
            query(mountainArr, mid);
            if (target == mountain[mid])
                return mid;
            if (target > mountain[mid])
                low = mid + 1;
            else
                high = mid - 1;
        }
        return -1;
    }
    int findInMountainArrayDec(int target, MountainArray& mountainArr, int low, int high) {  // inclusive
        if (low >= high)
            return low;
        int mid = (low + high) / 2;
        query(mountainArr, mid);
        if (target == mountain[mid])
            return mid;
        if (target < mountain[mid])
            return findInMountainArrayDec(target, mountainArr, mid + 1, high);
        return findInMountainArrayDec(target, mountainArr, low, mid - 1);
    }
    int findInMountainArrayDec_loop(int target, MountainArray& mountainArr, int low, int high) {  // inclusive
        while (low <= high) {
            int mid = (low + high) / 2;
            query(mountainArr, mid);
            if (target == mountain[mid])
                return mid;
            if (target < mountain[mid])
                low = mid + 1;
            else
                high = mid - 1;
        }
        return -1;
    }
    vector<int> mountain;
public:
    int findInMountainArray(int target, MountainArray& mountainArr) {
        int size = mountainArr.length();
        mountain.resize(size);
        fill(begin(mountain), end(mountain), -1);
        int top = find_top_loop(mountainArr, 0, size - 1);
        int found = findInMountainArrayInc_loop(target, mountainArr, 0, top);
        if (found<0)
            found = findInMountainArrayDec_loop(target, mountainArr, top + 1, size - 1);
        return found;
    }
};
#include <catch.hpp>
TEST_CASE("1095. Find in Mountain Array", "[INTERACTIVE]")
{
    MountainArray ma({ 1,2,3,4,5,3,1 });
    CHECK(MountainArrayQuery().findInMountainArray(3, ma) == 2);
}

TEST_CASE("852. Peak Index in a Mountain Array", "[BS]")
{
    CHECK(peakIndexInMountainArray(vector<int>{24, 69, 100, 99, 79, 78, 67, 36, 26, 19}) == 2);
}