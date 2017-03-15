// compare performance of string concatenation vs stringstream

#include <string>
#include <chrono>
#include <iostream>
#include <sstream>
using namespace std;

char cstr[200];
string str;
ostringstream ostr;
/*
string nano seconds :       53000000
strcpy nano seconds :       29000000
sprintf_s nano seconds :    292000000
stringstream nano seconds : 3400000000
*/
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
		strcpy_s(cstr, sizeof(str), "\"");
		strcat_s(cstr, sizeof(str), "name");
		strcpy_s(cstr, sizeof(str), "\"");
		strcpy_s(cstr, sizeof(str), "=");
		strcpy_s(cstr, sizeof(str), "\"");
		strcpy_s(cstr, sizeof(str), "value");
		strcpy_s(cstr, sizeof(str), "\"");
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "strcpy nano seconds: " << nanos.count() << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops; i++) {
		sprintf_s(cstr, sizeof(str), "%c%s%c=%c%s%c", '"', "name", '"', '"', "value", '"');
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "sprintf_s nano seconds: " << nanos.count() << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops/100; i++) {
		ostr << '"' << "name" << "\"" << "=" << "\"" << "value" << "\"";
		str = ostr.str();
		ostr.clear();
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "stringstream nano seconds: " << nanos.count()*100 << endl;

}