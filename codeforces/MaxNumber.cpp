#include<iostream>
#include <string>
using namespace std;

// 795C,  VK Cup 2017 - Wild Card Round 1
// Digital display batter can show n sections
// Find the max number it can display
// IDEA display the digit requiring least sections, which is digit 1.
//  if n is odd, display first digit as 7
// use string for number as it can be very large
std::string maxNum(int sections)
{
	char section[] = { 6, 2, 5, 5, 4, 5, 6, 3, 7, 6 }; // sections needed to display digit 0 to 9
	int digits = sections / 2; // max digits
	string str(digits, '1');
	if (sections % 2 > 0) {
		str.replace(0, 1, "7");
	}
	return str;
}

int main()
{
	int s;
	cin >> s;
	cout << maxNum(s) << endl;
	return 0;
}