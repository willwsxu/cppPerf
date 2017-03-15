// compare performance of string concatenation vs stringstream

#include <string>
#include <chrono>
#include <iostream>
#include <sstream>
using namespace std;

char cstr[200];
string str;
/*
string nano seconds:		 67000000
strcpy nano seconds:		 69000000
sprintf_s nano seconds:		307000000
stringstream nano seconds:	910000000
strcpy_s10 nano seconds:	  9450000
memcpy10 nano seconds:		   820000
strncmp10 nano seconds:		  7670000
memcmp10 nano seconds:		        0
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
		str = ostr.str();
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
		strncmp(cstr, "name=value", 10); // compare 10 bytes
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "strncmp10 nano seconds: " << nanos.count() / 100 << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops * 100; i++) {
		memcmp(cstr, "name=value", 10); // compare 10 bytes
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "memcmp10 nano seconds: " << nanos.count() / 100 << endl;

}