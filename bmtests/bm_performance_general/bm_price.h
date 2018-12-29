#pragma once
#include <cstdint>
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
