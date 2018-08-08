// compare performance of string concatenation vs stringstream

#include <benchmark/benchmark.h>
#include <string>
#include <chrono>
#include <iostream>
#include <sstream>

#include "string_util.h"

using namespace std;

static const int MAX_SIZE = (8 << 10)+100;
char cstr[MAX_SIZE];
string str;
int res=0;
/*
string nano seconds:        53000000
strcpy nano seconds:        55000000
sprintf_s nano seconds:    284000000
stringstream nano seconds: 740000000
strcpy_s10 nano seconds:     9760000
memcpy10 nano seconds:        820000
strncmp10 nano seconds:      7570000
memcmp10 nano seconds:             0
string==10 nano seconds:     2270000
*/

static void BM_string_plus(benchmark::State& state) {
	size_t sz = (size_t)state.range(0);
	auto s = memset_char('x', sz);
	char *src = s.get();
	str.reserve(MAX_SIZE);
	for (auto _ : state) {
		str = '"';
		str += "name";
		str += '"';
		str += '=';
		str += '"';
		str += src;
		str += '"';
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_string_plus)->Range(8, 8 << 10);

static void BM_string_append(benchmark::State& state) {
	size_t sz = (size_t)state.range(0);
	auto s = memset_char('x', sz);
	char *src = s.get();
	str.reserve(MAX_SIZE);
	for (auto _ : state) {
		str = '"';
		str.append("name").append(1, '"').append(1, '=').append(1, '"').append(src).append(1, '"');
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_string_append)->Range(8, 8 << 10);

StringBuilder sb(MAX_SIZE);
static void BM_string_builder(benchmark::State& state) {
	size_t sz = (size_t)state.range(0);
	auto s = memset_char('x', sz);
	char *src = s.get();
	for (auto _ : state) {
		sb.clear();
		sb.append(1, '"').append("name").append(1, '"').append(1, '=').append(1, '"').append(src).append(1, '"');
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_string_builder)->Range(8, 8 << 10);

static void BM_strcat_s(benchmark::State& state) {
	size_t sz = (size_t)state.range(0);
	auto s = memset_char('x', sz);
	char *src = s.get();
	for (auto _ : state) {
		strcpy_s(cstr, sizeof(cstr), "\"");
		strcat_s(cstr, sizeof(cstr), "name");
		strcat_s(cstr, sizeof(cstr), "\"");
		strcat_s(cstr, sizeof(cstr), "=");
		strcat_s(cstr, sizeof(cstr), "\"");
		strcat_s(cstr, sizeof(cstr), src);
		strcat_s(cstr, sizeof(cstr), "\"");
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_strcat_s)->Range(8, 8 << 10);


static void BM_sprintf_s(benchmark::State& state) {
	size_t sz = (size_t)state.range(0);
	auto s = memset_char('x', sz);
	char *src = s.get();
	for (auto _ : state) {
		sprintf_s(cstr, sizeof(cstr), "%c%s%c=%c%s%c", '"', "name", '"', '"', src, '"');
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_sprintf_s)->Range(8, 8 << 10);

static void BM_ostringstream(benchmark::State& state) {
	size_t sz = (size_t)state.range(0);
	auto s = memset_char('x', sz);
	char *src = s.get();
	for (auto _ : state) {
		ostringstream ostr;
		ostr << '"' << "name" << "\"" << "=" << "\"" << src << "\"";
		str = ostr.str();
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_ostringstream)->Range(8, 8 << 10);

void testStringComp()
{
	int loops = 1000000;
	auto start = chrono::high_resolution_clock::now();
	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops * 100; i++) {
		res = strncmp(cstr, "name=value", 10); // compare 10 bytes
	}
	auto end = chrono::high_resolution_clock::now();
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "strncmp10 nano seconds: " << nanos.count() / 100 << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops * 100; i++) {
		res = memcmp(cstr, "name=value", 10); // compare 10 bytes
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "memcmp10 nano seconds: " << nanos.count() / 100 << endl;
	
	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops * 100; i++) {
		res = (str == "name=value"); // compare 10 bytes
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "string==10 nano seconds: " << nanos.count() / 100 << endl;
}

/*
new string nano seconds:    171ns / 64 bytes
new char[] nano seconds:    88ns
memset char[] nano seconds: 6ns
*/