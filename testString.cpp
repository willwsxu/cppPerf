#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <memory>
using namespace std;
#include "tests.h"

// string class performance
string copyString(const char *test)
{
	return string(test);
}

shared_ptr<char[20]> copyShare(const char *test)
{
	shared_ptr<char[20]> buf = make_shared<char[20]>();
	int size = sizeof(*buf.get());
	char *dest = *buf.get();
	strncpy_s(dest, size, test, _TRUNCATE);
	return buf;
}

void testString()
{
	const char *pTetsCase = "TEST123456";

	DWORD start = GetTickCount();
	for (int i = 0; i < 1000000; i++)
		string temp = copyString(pTetsCase);
	DWORD end = GetTickCount();
	fprintf(stdout, "string take %d msec\n", end - start);  //811

	start = GetTickCount();
	for (int i = 0; i < 1000000; i++)
		shared_ptr<char[20]> temp2 = copyShare(pTetsCase);
	end = GetTickCount();
	fprintf(stdout, "shared_ptr take %d msec\n", end - start);  //811
}