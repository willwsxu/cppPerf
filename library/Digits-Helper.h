#pragma once
#include <cstdint>

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

// fill value to string [szValue, pEnd), pad left with 0 if more space left
inline int itoa_pos_pad_left(int64_t val, char *szValue, char *pEnd)
{
	int fill_zero = static_cast<int>((pEnd--) - szValue);
	int len = std::min(count_digits(val), fill_zero);
	fill_zero -= len;
	while (len--) {  // test 0 is fast than compare other values
		*pEnd-- = val % 10 + '0';
		val /= 10;
	}
	while (fill_zero--)  // less data dependency, two while loop can be done in parallel
		*szValue++ = '0';
	return (int)val;
}