#pragma once
#include <cstdint>
#include <algorithm>

// int to string conversion
inline int count_digits(int64_t val) {
	if (val >= 10000) {
		if (val >= 1000000) {
			if (val >= 100000000) {
				if (val >= 1000000000)
					return 10;
				return 9;
			}
			else {
				if (val >= 10000000)
					return 8;
				return 7;
			}
		}
		else {
			if (val >= 100000)
				return 6;
			return 5;
		}
	}
	else {
		if (val >= 100) {
			if (val >= 1000)
				return 4;
			return 3;
		}
		else {
			if (val >= 10)
				return 2;
			return 1;
		}
	}
}

inline int itoa_fill(int64_t val, char *pBack, int len)
{
	while (len--) { // test 0 is fast than compare other values
		*pBack-- = val % 10 + '0';
		val /= 10;
	}
	return (int)val;
}
// fill value to string [szValue, pEnd), pad left with 0 if more space left
inline int itoa_pos_pad_left(int64_t val, int int_len, char *szValue, char *pEnd)
{
	int fill_zero = static_cast<int>((pEnd--) - szValue);
	int len = std::min(int_len, fill_zero);
	fill_zero -= len;
	val=itoa_fill(val, pEnd, len);
	while (fill_zero--)  // less data dependency, two while loop can be done in parallel
		*szValue++ = '0';
	return (int)val;
}
// fill value to string [szValue, pEnd), pad left with 0 if more space left
inline int itoa_pos_pad_left(int64_t val, char *szValue, char *pEnd)
{
	return itoa_pos_pad_left(val, count_digits(val), szValue, pEnd);
}

inline char* itoa_new2(int64_t val, char *szValue, int size)
{
	//assert size>12
	if (val == 0) {
		szValue[0] = '0';
		szValue[1] = 0;
	}
	else {
		char *pEnd = nullptr;
		if (val < 0) {
			szValue[0] = '-';
			val = -val;
			pEnd = szValue + count_digits(val);
		}
		else
			pEnd = szValue + count_digits(val) - 1;
		pEnd[1] = 0;
		while (val > 0) {
			*pEnd-- = val % 10 + '0';
			val /= 10;
		}
	}
	return szValue;
}

// convert int to string, truncate to fit size
inline char* itoa_(int64_t val, char *szValue, int size) // slightly better than itoa_new2
{
	if (val < 0) {
		val = -val;
		*szValue++ = '-';
	}
	int len = std::min(count_digits(val), size-1);  // save last byte for nil
	itoa_fill(val, szValue + len-1, len);
	szValue[len] = 0;
	return szValue;
}

// convert int to string, right justified, pad left with 0
inline int itoa_fix(int64_t val, char *szValue, char *pEnd)
{
	if (val < 0) {
		*szValue++ = '-';
		val = -val;
	}
	return itoa_pos_pad_left(val, szValue, pEnd);
}

inline bool ftoa_fix(int64_t val, unsigned short dp, char *szValue, char *pEnd)
{
	if (dp >= 10)
		return false;
	const static int pow10_[10] = { 1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000 };
	if (val < 0) {
		*szValue++ = '-';
		val = -val;
	}
	itoa_fill(val%pow10_[dp], pEnd - 1, dp); // data dependency
	*(pEnd - dp - 1) = '.';
	itoa_pos_pad_left(static_cast<int>(val / pow10_[dp]), szValue, pEnd - dp - 1);
	return true;
}
