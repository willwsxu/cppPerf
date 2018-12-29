#pragma once
#include <cstdint>

// quickbench.com show both price functions are fast, 1ns.
using LONGLONG = long long;
using INT32 = long;
using UINT16 = unsigned short;
using WORD = unsigned short;
using LONG = long;
using uint8_t = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;
//using uint64_t = unsigned long long;

const char szDecimalBC[] = "eimnopwyz";
static const int maxDecimal = sizeof(szDecimalBC) - 2;
const char Bc2DP[] = { -1,-1,-1,-1,0,-1,-1,-1,1,-1,-1,-1,2,3,4,5,-1,-1,-1,-1,-1,-1,6,-1,7,8 };

static const int PriceExp = 10000000;
static const int maxPriceExpDp = 7;
static const int PriceExpDp = 7;

static const uint8_t Int8Null = 127;
static const uint8_t UInt8Null = 255;
static const uint16_t UInt16Null = 65535;
static const uint32_t Int32Null = 2147483647;
static const uint32_t UInt32Null = 4294967295;
static const uint64_t Int64Null = 9223372036854775807;
static const uint64_t UInt64Null = 18446744073709551615;

static const int64_t Pow10[19] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000,  10000000000,  100000000000,  1000000000000, 10000000000000, 100000000000000, 1000000000000000, 10000000000000000, 100000000000000000, 1000000000000000 };
static const int Pow10L = sizeof(Pow10) / sizeof(Pow10[0]);

typedef struct _LPACK_
{
	UINT16 wBase;
	INT32 lVal;
}
LPACK;
LPACK ZeroPrice() {
	return{ (UINT16)szDecimalBC[0], (long)0 };
}

LPACK Int64ToPrice(int64_t priceRaw, int  decimalPoints, int maxExp = 7) {

	int8_t ind = 0;

	while (ind < decimalPoints) { //remove trailing 0s
		if (priceRaw%Pow10[ind + 1] != 0)
			break;
		++ind;
		if (ind >= Pow10L)
			return ZeroPrice();
	}

	while (decimalPoints > maxExp + ind || priceRaw / Pow10[ind] > INT32_MAX || priceRaw / Pow10[ind] < INT32_MIN) { // check for overflow
		++ind;
		if (ind > decimalPoints || ind >= Pow10L) // decimalPoints can't be negative, ind >= CmePow10L is invalid
			return ZeroPrice();
	}

	return{ (UINT16)szDecimalBC[decimalPoints - ind], (long)(priceRaw / Pow10[ind]) };
}

LPACK ToLPACK(int64_t priceRaw, int64_t displayFactor, short priceAdjust, bool isNewTemplate) {

	if (!priceRaw) {
		return ZeroPrice();
	}

	if (isNewTemplate)
		priceRaw /= 100;

	int decimalPoints = PriceExpDp; // default (CME defined) number of decimal points

	if (priceAdjust != Int8Null) { //need to adjust using CmeGbxPriceAdjust file
		decimalPoints += priceAdjust;
	}
	else if (displayFactor) { // use DisplayFactor
		decimalPoints = (size_t)(PriceExpDp + log10((double)PriceExp / displayFactor));
	}

	return Int64ToPrice(priceRaw, decimalPoints);
}

static void BM_ToLPACK(benchmark::State& state) {
	LONGLONG longPrice2 = 1234567890000000000;
	int lVal = 0;
	for (auto _ : state) {
		benchmark::DoNotOptimize(lVal = ToLPACK(longPrice2, 1000, Int8Null, false).lVal);
	}
}
BENCHMARK(BM_ToLPACK);// ->Arg(1)->Arg(10)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Arg(10000000)->Arg(100000000);


#define DP_OK 0
#define DP_NOCHANGE 1
#define DP_MODIFIED 2
#define DP_OVERFLOW 3
#define DP_INVALID  4

#define NO_BASECODE (WORD)-1
#define BC_UNITS	'e'
#define BC_1DP  	'i'
#define BC_2DP  	'm'
#define BC_3DP  	'n'
#define BC_4DP  	'o'
#define BC_5DP  	'p'
#define BC_6DP  	'w'
#define BC_7DP  	'y'
#define BC_8DP  	'z'
#define BC_256THS  	'r'


__inline int pow10(int i)
{
	const int _Pow10[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,1000000000 };
	return _Pow10[i];
}
void PriceTrimInPlace(INT32 * lVal, int *denomDp, int minDp)
{
	while ((*denomDp > minDp) && ((*lVal % 10) == 0))
	{
		*lVal /= 10L;
		--(*denomDp);
	}
}
int	GetInt64Price(LONGLONG i64Val, int numDecimals, INT32 *plVal, WORD *pwBase, int minDp = -1)
{
	LONGLONG tmpNum = i64Val;
	int rv = DP_OK;

	if (numDecimals < 0) {
		*pwBase = szDecimalBC[0];
		*plVal = 0;
		return DP_INVALID;
	}

	// Make sure that the number of decimals is less than or equal to 8(which is 
	// what is current supported
	if (numDecimals > maxDecimal)
	{
		if (tmpNum != 0)
		{
			WORD divIndex = (numDecimals - maxDecimal);// < maxDecimal ? (numDecimals - maxDecimal) : maxDecimal;
			rv = (tmpNum % pow10(divIndex) != 0) ? DP_MODIFIED : DP_NOCHANGE;
			tmpNum /= pow10(divIndex);
		}
		numDecimals = maxDecimal;
	}

	// Initialize default values
	*pwBase = szDecimalBC[numDecimals];
	*plVal = 0;

	if (i64Val == 0)
		return rv;

	// Avoid overflow
	while (((tmpNum >= 0x000000007FFFFFFF) || (-tmpNum >= 0x000000007FFFFFFF)) && numDecimals > 0)
	{
		rv = tmpNum % 10 == 0 ? DP_NOCHANGE : DP_MODIFIED;
		tmpNum /= 10;
		numDecimals -= 1;
	}

	// Make sure that we are still not in the overflow condition
	if ((tmpNum >= 0x000000007FFFFFFF) || (-tmpNum >= 0x000000007FFFFFFF))
	{
		//How shall we handle the case were we cant reduce the precision and we still are overflowed
		//		tb_sev_msg(SEV_WARN, "Price Overflow:%llu", i64Val);
		*pwBase = NO_BASECODE;
		*plVal = (LONG)0x7FFFFFFF;
		rv = DP_OVERFLOW;
	}
	else
	{
		*pwBase = szDecimalBC[numDecimals];
		*plVal = (LONG)tmpNum;
	}

	if (minDp >= 0 && (*pwBase - 'a' <= 25))
	{
		int denomDp = Bc2DP[*pwBase - 'a'];
		PriceTrimInPlace(plVal, &denomDp, minDp);
		*pwBase = szDecimalBC[denomDp];
	}

	return rv;
}

static void BM_GetInt64Price(benchmark::State& state) {
	int rv = 0;
	LONGLONG longPrice2 = 1234567890000000000;
	long lVal = 0;
	WORD wBase = 0;
	for (auto _ : state) {
		benchmark::DoNotOptimize(rv = GetInt64Price(longPrice2, 11, &lVal, &wBase));
	}
}
BENCHMARK(BM_GetInt64Price);// ->Arg(9)->Arg(10)->Arg(11)->Arg(12)->Arg(13)->Arg(14)->Arg(15)->Arg(16);