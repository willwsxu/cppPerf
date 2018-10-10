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
	for (size_t i = 1; i < prefix.size(); i++)
		prefix[i] += prefix[i - 1];  // prefix sum
}

class mode_calc  // modes of a sequence
{
public:
	void try_new_mode(int prev, int same_count)
	{
		if (modes.empty()) {
			mode_count = same_count;
		}
		else if (mode_count > same_count) // not a mode
			return;
		else if (mode_count < same_count) {
			modes.clear();
			mode_count = same_count;
		}
		modes.push_back(prev);
	}
	std::vector<int> move()
	{
		return std::move(modes);
	}
private:
	std::vector<int> modes;
	int mode_count = 0;
};

//  method returns Nth power of A 
inline double nthRoot(int A, int N)
{
	// intially guessing a random number between 
	// 0 and 9 
	double xPre = rand() % 10;

	//  smaller eps, denotes more accuracy 
	double eps = 1e-3;

	// initializing difference between two 
	// roots by INT_MAX 
	double delX = INT_MAX;

	//  xK denotes current value of x 
	double xK;

	//  loop untill we reach desired accuracy 
	while (delX > eps)
	{
		//  calculating current value from previous 
		// value by newton's method 
		xK = ((N - 1.0) * xPre +
			(double)A / pow(xPre, N - 1)) / (double)N;
		delX = abs(xK - xPre);
		xPre = xK;
	}

	return xK;
}

inline std::vector<int> count_letter(const std::string&s, char base)  // all letter, either lower, or upper case
{
	std::vector<int> count(26, 0);
	for (char c : s)
		count[c - base]++;
	return count;
}