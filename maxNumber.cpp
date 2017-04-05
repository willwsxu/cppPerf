#include<iostream>
using namespace std;

//http://codeforces.com/contest/795/submission/26154916
//http://codeforces.com/contest/795/problem/C

char section[] = { 6, 2, 5, 5, 4, 5, 6, 3, 7, 6 };
void maxNum(int sections)
{
	int digits = sections / 2; // max digits
	int rem = sections % 2;
	cout << (rem > 0 ? 7 : 1);
	while (--digits > 0)
		cout << 1;;
	cout << endl;;
}

//int main()
int testNum()
{
	/*
	__int64 result = 8198552921648689607;
	int sections = 0;
	while (result > 0) {
		sections += section[result % 10 - '0'];
		result /= 10;
	}
	cout << sections << endl;
	*/
	int s = 85651;
	cin >> s;
	maxNum(s);

	return 0;
}