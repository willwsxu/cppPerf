#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
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
		if (loop<num)  // last even
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
				count[(n&mask)>0?1:0]++;
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
				continue;
			}
			if ((n&singl) == 0)   // 1 byte, start with 0
				continue;
			int t = n&tripl; // 1110 3 bytes
			switch (t) {
			case doubl:	bytes = 1;		break;
			case tripl:
				if ((n&quart) == tripl)
					bytes = 2;
				else if ((n&penta) == quart)
					bytes = 3;
				else
					return false;
				break;
			default:
				return false;
			}
		}
		return bytes == 0;
	}

	// 397. Integer Replacement. What is the minimum number of replacements needed for n to become 1?
	// If n is even, replace n with n/2.
	// If n is odd, you can replace n with either n + 1 or n - 1.
	int integerReplacement(int n) {
		int ones = 0;  // count 1 from right
		int ans = 0;
		while (n > 1) {
			if (n & 1)  // count consecutive ones
				ones++;
			else if (ones>0) {  // some consecutive ones up to this position
				if (ones == 1)  // do n-1 operation
					ones = 0;
				else
					ones = 1;   // do n+1 op, thus convert current 0 to 1
				ans++;  // extra operation regardless n+1 or n-1
			}
			ans++;
			n >>= 1;
		}
		return ones ? (ones>1 ? ans + 2 : ans + 1) : ans; // if ones>1, add 2 because n+1 causing n extra bit
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
		int lookup[20] = { 0 };  // define ACGT
		lookup['C' - 'A'] = 1;
		lookup['G' - 'A'] = 2;
		lookup['T' - 'A'] = 3;
		unordered_map<int, int> subDna;  // 10 letter substring key, map to count
		int key = 0;
		for (int i = 0; i < 10; i++) {
			key <<= 2;
			key |= lookup[s[i] - 'A'];
		}
		vector<string> ans;
		subDna[key] = 1;
		for (int i = 10; i < n; i++) {
			key <<= 2;
			key |= lookup[s[i] - 'A'];
			key &= 0xFFFFF;  // keep only 20 bits
			auto& count = subDna[key];
			if (count == 1)
				ans.push_back(s.substr(i - 9, 10));
			count++;
		}
		return ans;
	}
};


TEST_CASE("total hamming distance", "[NEW]")
{
	CHECK(Bit().totalHammingDistance(vector<int>{4, 14, 2}) == 6);
}
TEST_CASE("range bit and", "[NEW]")
{
	CHECK(Bit().rangeBitwiseAnd(1, 2) == 0);
	CHECK(Bit().rangeBitwiseAnd(0, 0) == 0);
	CHECK(Bit().rangeBitwiseAnd(0, 1) == 0);
	CHECK(Bit().rangeBitwiseAnd(3, 3) == 3);
	CHECK(Bit().rangeBitwiseAnd(5, 7) == 4);
	CHECK(Bit().rangeBitwiseAnd(6, 7) == 6);
	CHECK(Bit().rangeBitwiseAnd(14, 15) == 14);
}

TEST_CASE("valid UTF", "[NEW]")
{
	CHECK(Bit().validUtf8(vector<int>{197, 130, 1}) == true);
	CHECK(Bit().validUtf8(vector<int>{235, 140, 4}) == false);
}

TEST_CASE("integer replacements", "[NEW]")
{
	CHECK(Bit().integerReplacement(1) == 0);
	CHECK(Bit().integerReplacement(2) == 1);
	CHECK(Bit().integerReplacement(3) == 2);
	CHECK(Bit().integerReplacement(7) == 4);
	CHECK(Bit().integerReplacement(31) == 6);
	CHECK(Bit().integerReplacement(8) == 3);
}
