// compare performance of string concatenation vs stringstream

#include <benchmark/benchmark.h>
#include <string>
#include <chrono>
#include <iostream>
#include <sstream>

#include "string_util.h"

using namespace std;

char cstr[200];
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
	char* dst = new char[sz];
	for (auto _ : state) {
		str = '"';
		str += "name";
		str += '"';
		str += '=';
		str += '"';
		str += "value";
		str += '"';
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
	delete[] dst;
}
void testStringConcat()
{
	int loops = 1000000;
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops; i++) {
		str = '"';
		str += "name";
		str += '"';
		str += '=';
		str += '"';
		str += "value";
		str += '"';
	}
	auto end = chrono::high_resolution_clock::now();
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "string nano seconds: " << nanos.count() << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops; i++) {
		strcpy_s(cstr, sizeof(cstr), "\"");
		strcat_s(cstr, sizeof(cstr), "name");
		strcat_s(cstr, sizeof(cstr), "\"");
		strcat_s(cstr, sizeof(cstr), "=");
		strcat_s(cstr, sizeof(cstr), "\"");
		strcat_s(cstr, sizeof(cstr), "value");
		strcat_s(cstr, sizeof(cstr), "\"");
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "strcpy nano seconds: " << nanos.count() << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops; i++) {
		sprintf_s(cstr, sizeof(cstr), "%c%s%c=%c%s%c", '"', "name", '"', '"', "value", '"');
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "sprintf_s nano seconds: " << nanos.count() << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops / 10; i++) {
		ostringstream ostr;
		ostr << '"' << "name" << "\"" << "=" << "\"" << "value" << "\"";
		//str = ostr.str();
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "stringstream nano seconds: " << nanos.count()*10 << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops*100; i++) {
		strcpy_s(cstr, sizeof(cstr), "name=value"); // copy 10 bytes
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "strcpy_s10 nano seconds: " << nanos.count()/100 << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops*100; i++) {
		memcpy(cstr, "name=value", 10); // copy 10 bytes
		cstr[10] = 0;
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "memcpy10 nano seconds: " << nanos.count()/100 << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops * 100; i++) {
		res = strncmp(cstr, "name=value", 10); // compare 10 bytes
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
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
string *newStr;
char *newCstr;
// testNewDelete
static void BM_new_delete_string(benchmark::State& state) {
	for (auto _ : state) {
		newStr = new string(static_cast<size_t>(state.range(0)), 'X');
		delete newStr;
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_new_delete_string)->Range(8, 8 << 10);

static void BM_new_delete_memset(benchmark::State& state) {
	size_t len = static_cast<size_t>(state.range(0));
	for (auto _ : state) {
		newCstr = new char[len];
		memset(newCstr, 'X', len);
		delete newCstr;
	}
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_new_delete_memset)->Range(8, 8 << 10);

static void BM_control_memset(benchmark::State& state) {
	size_t len = static_cast<size_t>(state.range(0));
	newCstr = new char[len];
	for (auto _ : state) {
		memset(newCstr, 'X', len);
	}
	delete newCstr;
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}
BENCHMARK(BM_control_memset)->Range(8, 8 << 10);