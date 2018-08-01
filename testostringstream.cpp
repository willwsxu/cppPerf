// compare performance of string concatenation vs stringstream

#include <benchmark/benchmark.h>
#include <string>
#include <chrono>
#include <iostream>
#include <sstream>
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


static void BM_memcpy(benchmark::State& state) {
	char* src = new char[state.range(0)];
	char* dst = new char[state.range(0)];
	memset(src, 'x', state.range(0));
	for (auto _ : state)
		memcpy(dst, src, state.range(0));
	state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
	delete[] src;
	delete[] dst;
}
//BENCHMARK(BM_memcpy)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);

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
new string nano seconds:    53000000
new char[] nano seconds:    50000000
memset char[] nano seconds: 0
*/
string *newStr;
char *newCstr;
void testNewDelete()
{
	int loops = 1000000;
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops; i++) {
		newStr = new string(10, 'x');
		delete newStr;
	}
	auto end = chrono::high_resolution_clock::now();
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "new string nano seconds: " << nanos.count() << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops; i++) {
		newCstr = new char[11];
		memset(newCstr, 0, 11);
		delete newCstr;
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "new char[] nano seconds: " << nanos.count() << endl;

	newCstr = new char[11];
	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops; i++) {
		memset(newCstr, 0, 11);
	}
	delete newCstr;
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "memset char[] nano seconds: " << nanos.count() << endl;
}