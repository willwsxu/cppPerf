// perfTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <vector>
#include <algorithm>
using namespace std;

#include "tests.h"

bool test()
{
	return true;
}
int _tmain(int argc, _TCHAR* argv[])
{
	testString();

	int test1[9000] = { 1 };
	std::fill(begin(test1), end(test1), 1);
	vector<int> test2(9000);
	vector<int> &test3 = test2;
	test2.assign(9000, 1);
	DWORD start = GetTickCount();
	int temp = 0;
	__int64 loops = 1000000000;
	for (__int64 i = 0; i < loops; i++)
	{
		if (test1[8999])
			temp = 1;
		else
			temp = 2;
	}
	DWORD end = GetTickCount();
	fprintf(stdout, "array take %d msec temp=%d\n", end - start, temp);  //811

	temp = 0;
	start = GetTickCount();
	for (__int64 i = 0; i < loops; i++)
	{
		if (test2[8999])
			temp = 1;
		else
			temp = 2;
	}
	end = GetTickCount();
	fprintf(stdout, "vector take %d msec temp=%d\n", end - start, temp);  // 156

	temp = 0;
	start = GetTickCount();
	for (__int64 i = 0; i < loops; i++)
	{
		if (test3[8999])
			temp = 1;
		else
			temp = 2;
	}
	end = GetTickCount();
	fprintf(stdout, "vector reference take %d msec temp=%d\n", end - start, temp); //827

	temp = 0;
	start = GetTickCount();
	for (__int64 i = 0; i < loops; i++)
	{
		if (test)
			temp = 1;
		else
			temp = 2;
	}
	end = GetTickCount();
	fprintf(stdout, "function test take %d msec temp=%d\n", end - start, temp);  //936


	temp = 0;
	start = GetTickCount();
	for (__int64 i = 0; i < loops; i++)
	{
	}
	end = GetTickCount();
	fprintf(stdout, "loop take %d msec temp=%d\n", end - start, temp);  //0
	return 0;
}

