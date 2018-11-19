#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
#include "helper.h"
using namespace std;

class Math
{

public:
	// 453. Minimum Moves to Equal Array Elements
	// each move increment all n-1 elements by 1, find min to make all equal
	// sum+(n-1)*m = x * n, minNum should be incremented in all m moves, so final value x=minNum+m
	// sum - m = n* minNUm, m=sum - n*minNum
	int minMoves(vector<int>& nums) {  // borrow ideas, beat 97%
		auto minNum = min_element(nums.begin(), nums.end());
		auto sum = accumulate(nums.begin(), nums.end(), 0);
		return sum - nums.size()*(*minNum);
	}

	// 462. Minimum Moves to Equal Array Elements II
	// find the minimum number of moves required to make all array elements equal, where a move is incrementing or decrementing a selected element by 1
	// borrowed idea target number is median
	// 2 2 2 3 6
	// 2 2 3 6
	int minMoves2(vector<int>& nums) { // beat 98%
		if (nums.size() < 2)
			return 0;
		nth_element(nums.begin(), nums.begin() + nums.size() / 2, nums.end());
		int median = *(nums.begin() + nums.size() / 2);
		return accumulate(nums.begin(), nums.end(), 0, [median](size_t a, int n) {
			return a + abs(n - median);
		});
	}

	// 789. Escape The Ghosts, you start from (0,0) and try to reach target. There are some ghosts on board
	// You and ghosts can move 1 step a time, in 4 directions (N S E W)
	// You lose if the ghost reaches you.
	// Borrowed Math idea: You should find the shortest path to target
	// ghost is better to do the same instead of trying to intercept you
	bool escapeGhosts(vector<vector<int>>& ghosts, vector<int>& target) { // beat 97%
		int steps = abs(target[0]) + abs(target[1]);
		for (const auto& g : ghosts) {
			if (abs(g[0] - target[0]) + abs(g[1] - target[1]) <= steps)
				return false;
		}
		return true;
	}

	// 319. Bulb Switcher. n bulbs that are initially off. You first turn on all the bulbs
	// Then, you turn off every second bulb
	// On the third round, you toggle every third bulb (turning on if it's off or turning off if it's on).
	// For the i-th round, you toggle every i bulb
	// For the n-th round, you only toggle the last bulb. Find how many bulbs are on after n rounds
	// Idea: it translates to toggle every ith bulb on ith round. that means ith bulb will toggled m times, m is the total factors of i
	// it is on if it is toggled odd # of times. perfect square number has odd number of factors
	int bulbSwitch(int n) {  // beat 100%
		int count = 1;
		while (count*count <= n)
			count++;
		return count - 1;
	}

	// 672. Bulb Switcher II. Given n room and 4 buttons, after m operations, how many status of n lights
	// operations: Flip all the lights, Flip even lights, Flip odd lights, Flip lights with (3k + 1) numbers
	int flipLights(int n, int m) {  // borrowed fomula, no idea!
		n = min(n, 3);
		return min(1 << n, 1 + m*n);
	}

protected:
	bool isPerfectSquare(int num, int low, int hi) {// watch out for tricky overflow issues
		if (low > hi)
			return false;
		int64_t mid = ((int64_t)low + hi) / 2;
		int64_t sq = mid*mid;
		if (sq == num)
			return true;
		if (sq > num)
			return isPerfectSquare(num, low, (int)mid - 1);
		return isPerfectSquare(num, (int)mid + 1, hi);
	}
public:
	bool isPerfectSquare(int num) {  // binary search, beat 100%
		return isPerfectSquare(num, 1, min((num + 1) / 2, 50000));
	}
	bool judgeSquareSum_old(int c) { // a few gotcha cases, number can be 0, same, watch for overflow, beat 18%
		int sq = 0;
		int ub = min((c + 1) / 2, 50000);
		for (int i = 0; (sq = i*i) <= c && sq >= 0; i++) {
			if (c == sq || isPerfectSquare(c - sq, i, ub))
				return true;
		}
		return false;
	}

	bool judgeSquareSum(int c) { // use hash set to store all perfect square, why it is slower?
		unordered_set<int> squares;
		int sq = 0;
		for (int i = 0; (sq = i*i) <= c && sq >= 0; i++) {
			squares.insert(sq);
			if (squares.count(c - sq))
				return true;
		}
		return false;
	}

	// 67. Add Binary
	string addBinary(string a, string b) {  // beat 100%
		string *pA = &a, *pB = &b;
		if (a.size() < b.size()) {
			pA = &b;  // pA longer than pB
			pB = &a;
		}
		int carry = 0;
		transform(pB->rbegin(), pB->rend(), pA->rbegin(), pA->rbegin(), [&carry](char a, char b) {  // compute common part, from back
			int add = a - '0' + b - '0' + carry;
			carry = add / 2;
			return static_cast<char>(add % 2 + '0');
		});
		if (!carry)
			return *pA;
		if (pA->size() > pB->size()) {  // add carry to longer string
			auto begin = pA->rbegin() + pB->size();
			transform(begin, pA->rend(), begin, [&carry](char a) {
				if (carry == 0)  // just copy
					return a;
				int add = a - '0' + carry;
				carry = add / 2;
				return static_cast<char>(add % 2 + '0');
			});
		}
		if (carry)
			pA->insert(0, 1, '1');
		return *pA;
	}

	// 66. Plus One, Given a non-empty array of digits representing a non-negative integer, plus one to the integer
	vector<int> plusOne(vector<int>& digits) { // similar to #67 above, easier, beat 100%
		int carry = 1;
		vector<int> ans(digits);
		transform(ans.rbegin(), ans.rend(), ans.rbegin(), [&carry](int a) {
			if (carry == 0)  // just copy
				return a;
			int add = a + carry;
			carry = add / 10;
			return add % 10;
		});
		if (carry)
			ans.insert(ans.begin(), carry);
		return ans;
	}

	// 492. Construct the Rectangle, W*L=area, W<=L, W+L is minimal, area <=10^7
	vector<int> constructRectangle(int area) { // greedy method, beat 100%
		int MaxW = static_cast<int>(sqrt(area));
		for (int i = MaxW; i > 1; i--) {
			if (area%i == 0) {
				return{ area / i, i };
			}
		}
		return{ area, 1 };
	}
};


TEST_CASE("789. Escape The Ghosts", "[MATH]")
{
	CHECK(Math().escapeGhosts(vector<vector<int>>{ {1, 9}, { 2, -5 }, { 3, 8 }, { 9, 8 }, { -1, 3 }}, vector<int>{8, -10}) == false);
}

TEST_CASE("319. Bulb Switcher", "[MATH]")
{
	CHECK(Math().bulbSwitch(3) == 1);
	CHECK(Math().bulbSwitch(1001) == 31);
}

struct len_comp {
	bool operator()(int a, int b) const {
		return countDecDigit(a) < countDecDigit(b);
	}
};
long long digitNormalize(int n) {  // convert each digit to power of 10
	static int pow10[] = { 1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000 };
	long long ret = 0;
	while (n) {
		ret += pow10[n % 10];
		n /= 10;
	}
	return ret;
}
class MathPower {
public:
	using intset = std::multiset<int, len_comp>;
	MathPower()
	{
		if (pow2.empty()) {
			for (int i = 0, p = 1; p <= 1000000000; p = 1 << (++i))
				pow2.insert(p);
		}
	}
	// 869. Reordered Power of 2, 1 <= N <= 10^9
	// borrow clever idea: normalize a number to sum of power of 10 for each digit, sum is unique if digits are unique regardless of ordering
	bool reorderedPowerOf2(int N) {  // beat 100%
		auto p2 = pow2.equal_range(N);
		long long target = digitNormalize(N);
		return any_of(p2.first, p2.second, [target](int n) {return target == digitNormalize(n); });
	}
private:
	static intset pow2;
};
MathPower::intset MathPower::pow2;


TEST_CASE("869. Reordered Power of 2", "[POW]")
{
	MathPower p;
	CHECK(p.reorderedPowerOf2(46));
}

TEST_CASE("367. Valid Perfect Square", "[MATH]")
{
	CHECK(Math().isPerfectSquare(1));
	CHECK(Math().isPerfectSquare(808201));
}
TEST_CASE("633. Sum of Square Numbers", "[MATH]")
{
	CHECK(Math().judgeSquareSum(2147483646) == false);
	CHECK(Math().judgeSquareSum(0));
	CHECK(Math().judgeSquareSum(1));
	CHECK(Math().judgeSquareSum(808202));
	CHECK(Math().judgeSquareSum(3) == false);
}


class MathEasy
{
public:
	// 728. Self Dividing Numbers, divisible by all of its digits
	vector<int> selfDividingNumbers(int left, int right) {
		auto good = [](int n) {
			int orig = n;
			while (n > 0) {
				int digit = n % 10;
				if (digit == 0)
					return false;
				if (orig%digit != 0)
					return false;
				n /= 10;
			}
			return true;
		};
		vector<int> ans;
		ans.reserve(200);
		for (int i = left; i <= right; i++) {
			if (good(i))
				ans.push_back(i);
		}
		return ans;
	}
	// 507. Perfect Number, =sum of all its factors, except itself
	bool checkPerfectNumber(int num) {
		if (num <= 1)
			return false;
		int total = 1;
		for (int i = 2; i*i < num; i++) {
			if (num%i == 0) {
				total += (i + num / i);
			}
		}
		return total == num;
	}
	// 883. Projection Area of 3D Shapes
	int projectionArea(vector<vector<int>>& grid) { // beat 99%
		if (grid.empty())
			return 0;
		int n = grid.size();
		int xy = n*n; // XY projection
		int yz = 0;
		for (const auto& r : grid)
			yz += *max_element(begin(r), end(r));
		int xz = 0;
		int zero = 0;
		for (int c = 0; c < n; c++) {
			int tall = grid[0][c];
			if (tall == 0)
				zero++;
			for (int r = 1; r < n; r++) {
				if (grid[r][c] == 0)
					zero++;
				else
					tall = max(tall, grid[r][c]);
			}
			xz += tall;
		}
		return xy - zero + yz + xz;
	}
	// 892. Surface Area of 3D Shapes
	int surfaceArea(vector<vector<int>>& grid) { // beat 99%
		if (grid.empty())
			return 0;
		int n = grid.size();
		int areatotal = 0;
		int areahidden = 0;
		for (int r = 0; r < n; r++) {
			areatotal += ((grid[r][0]>0?2:0) + grid[r][0] * 4); // total surface area
			for (int c = 1; c < n; c++)
			{
				areatotal += ((grid[r][c]>0 ? 2 : 0) + grid[r][c] * 4); // total surface area
				areahidden -= 2 * min(grid[r][c], grid[r][c - 1]); //hidden bettween left and right
			}
		}
		for (int c = 0; c < n; c++) {
			for (int r = 1; r < n; r++) {
				areahidden -= 2 * min(grid[r][c], grid[r-1][c]);  // hidden between top and bottom
			}
		}
		return areatotal+areahidden;
	}
	// 868. Binary Gap, largest distance between 2 consecutive 1, in binary form
	int binaryGap(int N) { // beat 97%
		int dist = -1;
		int maxDist = 0;
		while (N) {
			if (N & 1) {
				maxDist = max(maxDist, ++dist);
				dist = 0;
			}
			else if (dist>=0)  // don't count initial 0
				dist++;
			N >>= 1;
		}
		return maxDist;
	}
	// 263. Ugly Number, Ugly numbers are positive numbers whose prime factors only include 2, 3, 5
	bool isUgly(int num) { // beat 98%
		if (num < 1)
			return false;
		while (num % 2 == 0) {
			num /= 2;
		}
		while (num % 3 == 0) {
			num /= 3;
		}
		while (num % 5 == 0) {
			num /= 5;
		}
		return num == 1;
	}
	// 204. Count Primes, less than a non-negative number, n.
	int countPrimes(int n) { // beat 68%
		if (n < 2)
			return 0;
		vector<bool> sieve(n, true);
		int count = 0;
		for (int i = 2; i < n; i++) {
			if (!sieve[i])
				continue;
			count++;
			for (int j = i + i; j < n; j += i)
				sieve[j] = false;
		}
		return count;
	}
	// 400. Find the nth digit of the infinite integer sequence 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, ...
	int findNthDigit(int n) {  // beat 100%, watch overflow
		const static int pow_[] = { 1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000 };
		int digits = 0;
		int i = 1;
		for (; i < 9; i++)  // group number by digit, overflow if i==9
		{
			int digits_new = digits + (pow_[i] - pow_[i - 1])*i;
			if (n <= digits_new)   // compute  which int is the target
				break;
			else
				digits = digits_new;
		}
		int count = n - digits;  // e.g n=11, count==2, num=10
		int num = pow_[i - 1] + count / i - 1;
		if (count%i == 0) {  // last digit of this number
			return num % 10;
		}
		else {
			num++;
			count = i - count%i;
			while (count-- > 0)
				num /= 10;
			return num % 10;
		}
	}

	// 812. Largest Triangle Area
	double largestTriangleArea(vector<vector<int>>& points) { // beat 94%
		int n = points.size();
		if (n < 3)
			return 0;
		double area = 0;
		for (int i = 0; i < n - 2; i++) {
			for (int j = i + 1; j < n - 1; j++) {
				for (int k = j + 1; k < n; k++) {//formular Area=(Ax/(By-Cy)+Bx(Cy-Ay)+Cx(Ay-By))/2
					double a = points[i][0] * (points[j][1] - points[k][1]);
					a += points[j][0] * (points[k][1] - points[i][1]);
					a += points[k][0] * (points[i][1] - points[j][1]);
					area = max(area, abs(a / 2.0));
				}
			}
		}
		return area;
	}
	// 171. Excel Sheet Column Number, A->Z, AA->ZZ, ...
	int titleToNumber(string s) { // beat 98%
		return accumulate(begin(s), end(s), 0, [](int init, char c) { return init * 26 + c - 'A' + 1; });
	}
	// 168. Excel Sheet Column Title
	string convertToTitle(int n) { // beat 100%
		string ans;
		while (n > 0) {
			ans.append(1, (n-1) % 26 + 'A');
			n = (n-1)/26;
		}
		std::reverse(ans.begin(), ans.end());
		return ans;
	}
	// 645. Set Mismatch, given [1,n], one of the numbers in the set got duplicated to another number in the set, which results in repetition of one number and loss of another number
	// firstly find the number occurs twice and then find the number that is missing
	vector<int> findErrorNums(vector<int>& nums) {  // beat 89%
		int dup = -1;
		for (size_t i = 0; i < nums.size(); i++) {
			if (nums[i] == i + 1)
				continue;
			int mover = nums[i]; // number in wrong position
			nums[i] = -1; // blank slot
			while (nums[mover - 1] != mover && nums[mover - 1] != -1) {
				swap(mover, nums[mover - 1]); // swap number to right position until dup is found or blank
			}
			if (nums[mover - 1] == -1)
				nums[mover - 1] = mover;
			else
				dup = mover;
		}
		auto found = find(nums.begin(), nums.end(), -1);
		return{ dup, distance(nums.begin(), found) + 1 };
	}
	// 628. Maximum Product of Three Numbers
	int maximumProduct(vector<int>& nums) { // beat 48%
		sort(begin(nums), end(nums));
		int n = nums.size();
		if (nums[0] >= 0 || nums[n - 1] <= 0 || n == 3)  // all positive or negative
			return nums[n - 1] * nums[n - 2] * nums[n - 3];
		return nums[n - 1] * max(nums[0] * nums[1], nums[n - 2] * nums[n - 3]); // pick right most, then compare prod of left 2 and right 2
	}
	// 836. Rectangle Overlap, bottom-left, top-right points
	bool isRectangleOverlap(vector<int>& rec1, vector<int>& rec2) { // beat 100%
		if (rec1[1] >= rec2[3] || rec2[1] >= rec1[3])  // no overlap top to bottom
			return false;
		if (rec1[2] <= rec2[0] || rec2[2] <= rec1[0])  // no overlap lrft to right,
			return false;
		return true;
	}
	// 415. Add Strings
	string addStrings(string num1, string num2) { // beat 99%
		string *pLong = &num1;
		string *pShort = &num2;
		if (num1.size() < num2.size()) {
			pShort = &num1;
			pLong = &num2;
		}
		int carry = 0;
		transform(pShort->rbegin(), pShort->rend(), pLong->rbegin(), pLong->rbegin(), [&carry](char c1, char c2) {
			int sum = c1 + c2 - '0' - '0' + carry;
			carry = sum / 10;
			return sum % 10 + '0';
		});
		if (carry && pLong->size() > pShort->size()) {
			auto start = pLong->rbegin() + pShort->size();
			transform(start, pLong->rend(), start, [&carry](char c1) {
				int sum = c1 - '0' + carry;
				carry = sum / 10;
				return sum % 10 + '0';
			});
		}
		if (carry)
			pLong->insert(0, 1, '1');
		return *pLong;
	}
	// 172. Factorial Trailing Zeroes
	int trailingZeroes(int n) { // same count of factor 5. beat 100%
		int total = 0;
		int divisor = 5;
		int count = 0;
		while (count=n / divisor) {
			total += count;
			if (divisor > INT32_MAX / 5) // avoid overflow
				break;
			divisor *= 5; // e.g. 25 has 2 factor 5
		}
		return total;
	}

	// 326. Power of Three
	bool isPowerOfThree(int n) {  // beat 5 to 94%
		static const int max_power_3 = 1162261467;//static_cast<int>(pow(3, 19));
		return n>0 && max_power_3%n == 0;
	}
	// 7. Reverse Integer
	int reverse(int x) {  // beat 97%
		string s = to_string(abs(x));

		// why std::reverse don't work on string?
		reverse_string(s.begin(), s.end());
		int sign = x >= 0 ? 1 : -1;
		long ans = stol(s)*sign;
		if (ans > INT32_MAX || ans < INT32_MIN)
			return 0;
		return ans;
	}
	// Given string with 'I' or 'D', construct vector if S[i]='I', A[i]<A[i+1]
	// fill array with permutation of 0..N to conform to rule
	vector<int> diStringMatch(string S) {
		vector<int> count;   // by segments, up positive, down negative
		int seg_count = 0;
		for (char c : S) {
			if (c == 'I') {
				if (seg_count < 0) {
					count.push_back(seg_count);
					seg_count = 0;
				}
				seg_count++;
			}
			else {
				if (seg_count > 0) {
					count.push_back(seg_count);
					seg_count = 0;
				}
				seg_count--;
			}
		}
		if (seg_count!=0)
			count.push_back(seg_count);

		// strategy to fill, from left to right
		// if segment is up, fill the top numbers from low to high
		// if segment is down, fill from top num dowm except to use the bottom nuber as the last one, to prepare for next up segment
		// update the two pointers
		int top_num = S.size();
		int bottom_num = 0;
		vector<int> ans;
		ans.reserve(top_num + 1);
		auto fill_down = [&ans, &top_num, &bottom_num](int c) {
			while (c--) {
				ans.push_back(top_num--);
			}
			ans.push_back(bottom_num++);
		};
		auto fill_up = [&ans, &top_num](int c) {
			for (int i = top_num - c; i <= top_num; i++)
				ans.push_back(i);
			top_num -= (c + 1);
		};
		if (count.front() < 0)
			fill_down(-count.front());
		else
			fill_up(count.front());
		for (size_t i = 1; i < count.size(); i++) {
			if (count[i] < 0) 
				fill_down(-count[i]-1);
			else
				fill_up(count[i]-1);
		}
		return ans;
	}
	vector<int> diStringMatch2(string S) {  // borrowed idea
		int top_num = S.size();
		int bottom_num = 0;
		vector<int> ans;
		ans.reserve(top_num + 1);
		for (char c : S) {
			if (c == 'I')
				ans.push_back(bottom_num++);
			else
				ans.push_back(top_num--);
		}
		ans.push_back(bottom_num);
		return ans;
	}
};
TEST_CASE("942. DI String Match", "[NEW]")
{
	CHECK(MathEasy().diStringMatch("IDID") == vector<int>{3,4,0,2,1});
	CHECK(MathEasy().diStringMatch("DDI") == vector<int>{3, 2, 0, 1});
	CHECK(MathEasy().diStringMatch("III") == vector<int>{0, 1, 2, 3});

	CHECK(MathEasy().diStringMatch2("IDID") == vector<int>{0,4,1,3,2});
	CHECK(MathEasy().diStringMatch2("DDI") == vector<int>{3, 2, 0, 1});
	CHECK(MathEasy().diStringMatch2("III") == vector<int>{0, 1, 2, 3});
}
TEST_CASE("172. Factorial Trailing Zeroes", "[NEW]")
{
	CHECK(MathEasy().trailingZeroes(1808548329) == 452137076);
}
TEST_CASE("645. Set Mismatch", "[NEW]")
{
	CHECK(MathEasy().findErrorNums(vector<int>{ 1, 2, 2, 4 }) == vector<int>{2, 3});
}
TEST_CASE("168. Excel Sheet Column Title", "[NEW]")
{
	CHECK(MathEasy().convertToTitle(26) == "Z");
	CHECK(MathEasy().convertToTitle(701) == "ZY");
}
TEST_CASE("400. Find the nth digit", "[NEW]")
{
	CHECK(MathEasy().findNthDigit(298954297) == 5);
	CHECK(MathEasy().findNthDigit(1000000000) == 1);
	CHECK(MathEasy().findNthDigit(11) == 0);
	CHECK(MathEasy().findNthDigit(12) == 1);
	CHECK(MathEasy().findNthDigit(200) == 0);
}
TEST_CASE("204. Count Primes", "[PRIM]")
{
	CHECK(MathEasy().countPrimes(20000000) == 1270607);
}

TEST_CASE("892. Surface Area of 3D Shapes", "[NEW]")
{
	CHECK(MathEasy().surfaceArea(vector<vector<int>>{ {1, 1, 1}, { 1,0,1 }, { 1,1,1 }}) == 32);
}
TEST_CASE("883. Projection Area of 3D Shapes", "[MATH]")
{
	CHECK(MathEasy().projectionArea(vector<vector<int>>{ {1, 1, 1}, { 1,0,1 }, { 1,1,1 }})==14);
}