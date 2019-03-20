#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
#include <array>
using namespace std;

class SingleNumber
{
public:
	// 136 every element appears twice except for one.Find that single one.
	int singleNumber(vector<int>& nums) {  // beat 97%, interesting question
		// xor cancel out same number. xor has associative property and commutative property
		return accumulate(nums.begin(), nums.end(), 0, bit_xor<int>());
	}

	//137  every element appears three times except for one, which appears exactly once
	// bit counting ideas: 0, 1, 10, 11
	// first bit will alternate between 0 and 1, so x0=x0 ^ i
	// next bit change to 1 only if previous bit was 1 and new one is 1, x2=x2 ^(x1&i)
	// each bit of the integer is counted by same bit position using multpile int counters
	int singleNumber2(vector<int>& nums) {  // beat 99%
		int bit1Count = 0, bit2Count = 0;  // 2 bits needed to count k=3
		for (int n : nums) {
			bit2Count ^= (bit1Count&n);  // calculate second bit before first bit is updated
			bit1Count ^= n;
			int mask = ~(bit2Count &bit1Count);  // both bit 1, flip to 0 as we count base is 3 (0, 1, 2)
			bit1Count &= mask;  // set bit to 0 when count is 3 (binary 11)
			bit2Count &= mask;
		}
		return bit1Count;
	}

	// 260 exactly two elements appear only once and all the other elements appear exactly twice
	vector<int> singleNumber3(vector<int>& nums) {  // beat 98%
		int x = accumulate(nums.begin(), nums.end(), 0, bit_xor<int>()); // a^b
		int diff_bit = (x & (-x));  // a and b are different, pick right most bit that is different between and and b
		int a = 0, b = 0;
		for (int n : nums) {  // separate a and b into two groups, now that problem is reduced to problem 136
			if (n&diff_bit)
				a ^= n;
			else
				b ^= n;
		}
		return{ a, b };
	}
	vector<int> singleNumber3_stl(vector<int>& nums) {  // beat 98%
		int x = accumulate(nums.begin(), nums.end(), 0, bit_xor<int>()); // a^b
		int diff_bit = (x & (-x));  // a and b are different, pick right most bit that is different between and and b
		auto second = partition(nums.begin(), nums.end(), [diff_bit](int n) {return n&diff_bit; });
		return{ accumulate(nums.begin(), second, 0, bit_xor<int>()), accumulate(second, nums.end(), 0, bit_xor<int>()) };
	}
};


class Bit
{
public:
	// 338. Counting Bits from 0 to n
	// dp[0]=0, dp[1]=dp[0]+1, dp[2]=dp[2/2]+0, dp[3]=dp[3/2]+1
	vector<int> countBits(int num) {
		vector<int> ans;   // speed up to use push_back instead of initializing to 0. beat 99%
		ans.reserve(num + 1);
		ans.push_back(0);
		if (num)
			ans.push_back(1);
		else
			return ans;
		int loop = num % 2 ? num : num - 1;
		for (int i = 2; i <= loop; i += 2) {  // no speed difference using loop step 2
											  //ans.push_back(ans[i >> 1] + (i & 1));  // divide by 2, plus checking last digit
			ans.push_back(ans[i >> 1]);  // even number
			ans.push_back(ans[i] + 1);   // next odd
		}
		if (loop < num)  // last even
			ans.push_back(ans[num >> 1]);
		return ans;
	}

	// 461. Hamming Distance, the number of positions at which the corresponding bits are different
	int hammingDistance(int x, int y) {  // beat 100%
		x ^= y;
		int cnt = 0;
		while (x) {
			cnt++;
			x &= x - 1;  // remove right most 1
		}
		return cnt;
	}

	// 477. Total Hamming Distance, between all pairs of the given numbers
	int totalHammingDistance(vector<int>& nums) {  // beat 61%
		int ans = 0; // check last digit and divide num by 2, count num=0 to detect early completion, did not improve performance
		for (int i = 0; i < 31; i++) { // check each bit set or not set
			int count[2] = { 0 };
			int mask = 1 << i;
			for (int& n : nums) {
				count[(n&mask) > 0 ? 1 : 0]++;
			}
			ans += count[0] * count[1];  // hamming distance at each bit
		}
		return ans;
	}

	// 201 Given a range [m, n] where 0 <= m <= n <= 2147483647, return the bitwise AND of all numbers in this range, inclusive
	int rangeBitwiseAnd(int m, int n) { // beat 98%
		if (m == n || m == 0)  // speacial cases
			return m;
		int d1 = (int)log2(m);    // find highest bit set
		int d2 = static_cast<int>(log2(n));
		if (d1 != d2)        // 1111 0111 in between there is 1000 so all bit AND is 0
			return 0;
		int ans = 1 << d1;
		for (int i = d1 - 1; i >= 0; i--) {  // beat 40% without code above to deal with special case
			int mask = 1 << i;
			if ((mask&m) && (mask&n))  // check each bit from left to right, if both 1, add to answer
				ans |= mask;
			else if ((mask&m) != (mask&n))  // when we reach a position one bit is 1, another is 0, AND must be 0 for rest of the bits
				break;
		}
		return ans;
	}

	// 318. Maximum Product of Word Lengths, 2 words must not share any letter. all lower cases
	int maxProduct(vector<string>& words) {  // beat 98% O(n^2) after not calc size in separate loop
		vector<int> masks; // mask letters present in a word, using 26 bits, one for each letter
		vector<int> lens;
		int n = words.size();
		if (n == 0)
			return 0;
		masks.reserve(n);
		lens.reserve(n);
		transform(words.begin(), words.end(), back_inserter(masks), [](const string &w) {
			return accumulate(w.begin(), w.end(), 0, [](int acc, char c) { return acc | (1 << (c - 'a')); });
		});
		int ans = 0;
		lens.push_back(words[0].size());
		for (int i = 1; i < n; i++) {
			lens.push_back(words[i].size());  // cache size within loop, more efficient than using a separate loop
			for (int j = 0; j < i; j++) {  // check words pair up to i
				if ((masks[i] & masks[j]) == 0)  // no overlap, watch out for operator precedence
					ans = max(ans, lens[i] * lens[j]);
			}
		}
		return ans;
	}

	// 393. UTF-8 Validation, each int in the vector is a byte
	bool validUtf8(vector<int>& data) {  // beat 12%
		const int singl = 0x80;
		const int doubl = 0xC0;
		const int tripl = 0xE0;
		const int quart = 0xF0;
		const int penta = 0xF8;
		int bytes = 0;
		for (int n : data) {
			if (bytes > 0) {
				if ((n&doubl) != singl)
					return false;
				bytes--;
			}
			else if ((n&singl) != 0) {  // 1 byte, start with 0
				if ((n&tripl) == doubl)	// 1100 2 bytes
					bytes = 1;
				else if ((n&quart) == tripl) // 1110 3 bytes
					bytes = 2;
				else if ((n&penta) == quart)
					bytes = 3;
				else
					return false;
			}
		}
		return bytes == 0;
	}

	// 397. Integer Replacement. What is the minimum number of replacements needed for n to become 1?
	// If n is even, replace n with n/2.
	// If n is odd, you can replace n with either n + 1 or n - 1.
	int integerReplacement(int n) { // beat 100%
		int ones = 0;  // count 1 from right
		int ans = 0;
		while (n > 1) {
			if (n & 1)  // count consecutive ones
				ones++;
			else if (ones > 0) {  // some consecutive ones up to this position
				if (ones == 1)  // do n-1 operation
					ones = 0;
				else
					ones = 1;   // do n+1 op, thus convert current 0 to 1
				ans++;  // extra operation regardless n+1 or n-1
			}
			ans++;
			n >>= 1;
		}
		return ones ? (ones > 1 ? ans + 2 : ans + 1) : ans; // if ones>1, add 2 because n+1 causing n extra bit
	}
	// 187. Repeated DNA Sequences, A C G T
	// Write a function to find all the 10-letter-long sequences (substrings) that occur more than once in a DNA molecule
	// idea: map ACGT to 0123, 2 bits
	// scan string from left to right, convert each letter to 2 bit and add to an accumulator (shift left 2 bits before adding new bits)
	// store the 20 bits int in a map
	vector<string> findRepeatedDnaSequences(string s) { // beat 93%
		int n = s.size();
		if (n < 10)
			return{};
		int lookup[85] = { 0 };  // define ACGT
		lookup['C'] = 1; // use large array and eliminate subtraction did not improve performance
		lookup['G'] = 2;
		lookup['T'] = 3;
		unordered_map<int, int> subDna;  // 10 letter substring key, map to count
		int key = 0;
		for (int i = 0; i < 10; i++) {
			key <<= 2;
			key |= lookup[s[i]];
		}
		vector<string> ans;
		subDna[key] = 1;
		for (int i = 10; i < n; i++) {
			key <<= 2;
			key |= lookup[s[i]];
			key &= 0xFFFFF;  // keep only 20 bits
			auto& count = subDna[key];
			if (count == 1)
				ans.push_back(s.substr(i - 9, 10));
			count++;
		}
		return ans;
	}
};

class Bits
{
	int bitCount(int n)
	{
		int bits = 0;
		while (n > 0) {
			bits++;
			n >>= 1;
		}
		return bits;
	}

	int mask(int n)
	{
		int m = 0;
		while (n-- > 0)
		{
			m <<= 1;
			m |= 1;
		}
		return m;
	}
public:
	// 191. Number of 1 Bits
	int hammingWeight(uint32_t n) { // candidate for sharing
		int bits = 0;
		while (n) {
			n &= n - 1;  // flip least significant 1 to 0, e.g. 1100 & 1011 = 1000
			bits++;
		}
		return bits;
	}
public:
	bool isPowerOfTwo_v0(int n) {  // beat 97%
		return hammingWeight(n) == 1;
	}
	//231. Power of Two
	bool isPowerOfTwo(int n)  // beat 97%
	{
		if (n <= 0)
			return false;
		return (n&(n - 1)) == 0;  // single 1 in binary representation
	}

	// 476. Number Complement, num>0
	int findComplement(int num) {  // beat 100%
		return num ^ (mask(max(bitCount(num), 1)));  // generate n bits mask, minimum 1
	}

	//342. Power of Four
	bool isPowerOfFour(int num) {
		/*if (!isPowerOfTwo(num))
		return false;
		int zero = 0;
		while (num > 0 && (num & 1) == 0) {
		zero++;
		num >>= 1;
		}
		return zero % 2 == 0;*/
		return num > 0 && (num&(num - 1)) == 0 && (0x55555555 & num);  // 0x55555555 mask 1 at odd positions
	}
	// 693. Binary Number with Alternating Bits, n>0
	bool hasAlternatingBits(int n) {  // beat 100%
		int or_ = (n | (n >> 1));  // or_should be all ones
		return (n&(n >> 1)) == 0 && (or_&(or_ + 1)) == 0;
	}

	// 762. Prime Number of Set Bits in Binary Representation
	// L, R will be integers L <= R in the range [1, 10^6]
	int countPrimeSetBits_v1(int L, int R) {
		array<bool, 32> primes{ false, false,true,true,false,true,false,true,false,false,false,true,false,true,false,false,false,true,false,true };//23, 29, 31
		vector<int> dp(1000001);
		for (int i = 1; i < L; i++)
			dp[i] = dp[i >> 1] + (i & 1);
		int count = 0;
		for (int i = L; i <= R; i++) {
			dp[i] = dp[i >> 1] + (i & 1);
			if (primes[dp[i]])
				count++;
		}
		return count;
	}

	int countPrimeSetBits(int L, int R) {  // beat 86%, faster than dp solution above
		array<bool, 32> primes{ false, false,true,true,false,true,false,true,false,false,false,true,false,true,false,false,false,true,false,true };//23, 29, 31
		int count = 0;
		for (int i = L; i <= R; i++) {
			if (primes[hammingWeight(i)])
				count++;
		}
		return count;
	}
	// 190. Reverse Bits
	uint32_t reverseBits(uint32_t n) {  // beat 84%
		uint32_t ans = 0;
		for (int i = 0; i<32; i++) {
			ans = (ans << 1) + (n & 1);
			n >>= 1;
		}
		return ans;
	}

	// 389. Find the Difference, only lowercase letters
	// String t is generated by random shuffling string s and then add one more letter at a random position
	char findTheDifference(string s, string t) {  // same idea as '136. Single Number', beat 100%
		int ans = accumulate(begin(s), end(s), 0, bit_xor<>());  // xor annihilate matching char between s and t except the extra one
		ans = accumulate(begin(t), end(t), ans, bit_xor<>());
		return static_cast<char>(ans);
	}

	// 268. Missing Number, [0,n]
	int missingNumber(vector<int>& nums) { // same idea as #136, beat 86%
		int ans = accumulate(begin(nums), end(nums), 0, bit_xor<>());
		for (int i = 1; i <= (int)nums.size(); i++)  // xor on all numbers from 1 to n
			ans ^= i;
		return ans;
	}
	// 371. Sum of Two Integers, not allowed to use the operator + and -
	int getSum(int a, int b) {
		while (b != 0) {
			int c = a&b;// both bit are 1
			a ^= b;  // add up 0 and 1
			b = c << 1;  // if both bits are 1, shift left
		}
		return a;
	}
	// 401. Binary Watch, 4 bits for hours, 6 bits for minutes. 
	// Given a non-negative integer n which represents the number of LEDs that are currently on, return all possible times the watch could represent
	vector<string> readBinaryWatch(int num) {  // beat 100%
		vector<string> ans;
		for (int i = 0; i <= 1024; i++) {  // iterate all 10 bits int, 4 bits hour + 6 bits min
			if (hammingWeight(i) == num) {
				int hour = i >> 6;
				int min = i & 0x3F;
				if (hour < 12 && min < 60) {
					string s = to_string(hour);
					s.append(1, ':');
					if (min < 10)
						s.append(1, '0');
					s.append(to_string(min));
					ans.push_back(s);
				}
			}
		}
		return ans;
	}
	// 405. Convert a Number to Hexadecimal
	string toHex(int num) {  // beat 100%
		if (num == 0)
			return "0";
		static const string hex_char = "0123456789abcdef";
		vector<int> hexNum;
		hexNum.reserve(8);
		for (int i = 0; i < 8; i++) {
			hexNum.push_back(num & 0xF);
			num >>= 4;
		}
		auto start = rbegin(hexNum);
		while (*start == 0)
			++start;
		string ans;
		transform(start, rend(hexNum), back_inserter(ans), [](int i) {return hex_char[i]; });
		return ans;
	}

	// 458. Poor Pigs, one of the bucket is poison, pig will die in x min, if allowed test time is y
	// how many pigs are needed
	// Each Pig represent 1 bit, test round is base, 1 test is base 2.
	int poorPigs(int buckets, int minutesToDie, int minutesToTest) {  // borrow clever idea, beat 100%
		int tests = minutesToTest / minutesToDie;
		//		if (buckets == 1)
		//			return 1;
		return (int)ceil(log2(buckets) / log2(tests + 1));
	}
};
TEST_CASE("405. Convert a Number to Hexadecimal", "[NEW]")
{
	CHECK(Bits().toHex(26) == "1a");
}

TEST_CASE("191. Number of 1 Bits", "[NEW]")
{
	CHECK(Bits().hammingWeight(11) == 3);
	CHECK(Bits().hammingWeight(0) == 0);
	CHECK(Bits().hammingWeight(128) == 1);
}
TEST_CASE("190. Reverse Bits", "[NEW]")
{
	CHECK(Bits().reverseBits(43261596) == 964176192);
}



TEST_CASE("total hamming distance", "[BIT]")
{
	CHECK(Bit().totalHammingDistance(vector<int>{4, 14, 2}) == 6);
}
TEST_CASE("range bit and", "[BIT]")
{
	CHECK(Bit().rangeBitwiseAnd(1, 2) == 0);
	CHECK(Bit().rangeBitwiseAnd(0, 0) == 0);
	CHECK(Bit().rangeBitwiseAnd(0, 1) == 0);
	CHECK(Bit().rangeBitwiseAnd(3, 3) == 3);
	CHECK(Bit().rangeBitwiseAnd(5, 7) == 4);
	CHECK(Bit().rangeBitwiseAnd(6, 7) == 6);
	CHECK(Bit().rangeBitwiseAnd(14, 15) == 14);
}

TEST_CASE("valid UTF", "[BIT]")
{
	CHECK(Bit().validUtf8(vector<int>{197, 130, 1}) == true);
	CHECK(Bit().validUtf8(vector<int>{235, 140, 4}) == false);
}

TEST_CASE("integer replacements", "[BIT]")
{
	CHECK(Bit().integerReplacement(1) == 0);
	CHECK(Bit().integerReplacement(2) == 1);
	CHECK(Bit().integerReplacement(3) == 2);
	CHECK(Bit().integerReplacement(7) == 4);
	CHECK(Bit().integerReplacement(31) == 6);
	CHECK(Bit().integerReplacement(8) == 3);
}