#pragma once

#include <regex>

inline void print(std::vector<int>& v)
{
	std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;
}

inline int to_int(const std::string& S, int start, int last)
{
	int len = last - start + 1;
	if (len == 10 && S[start] > '2')  // overflow
		return -1;
	return atoi(S.substr(start, len).c_str());
}

inline std::vector<std::string> tokenizer(std::string::const_iterator sb, std::string::const_iterator se, const char *sep)
{
	std::regex rgx(sep);
	std::vector<std::string> tokens;
	copy(std::sregex_token_iterator(sb, se, rgx, -1), std::sregex_token_iterator(), std::back_inserter(tokens));
	return tokens;
}

inline std::pair<int, int> multiplyComplex(int r1, int i1, int r2, int i2)
{
	return{ r1*r2 - i1*i2, r1*i2 + r2*i1 };
}

inline int countDecDigit(int n) {
	int c = 0;
	while (n) {
		c++;
		n /= 10;
	}
	return c;
}
inline void prefix_sum(std::vector<int>& prefix)
{
	for (int i = 1; i < prefix.size(); i++)
		prefix[i] += prefix[i - 1];  // prefix sum
}