#pragma once

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