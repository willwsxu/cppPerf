#pragma once

#include <regex>
#include <vector>
#include <set>
#include <map>
#include <deque>
#include <unordered_map>
#include <iterator>
#include <iostream>

using std::vector;
using std::set;

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

inline int countDecDigit(int n) {  // prefer count_digits in digit helper
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

template<typename T>
inline std::unordered_map<T, int> vector2map(const std::vector<T>& v)
{
	std::unordered_map<T, int> pos_map;
	for (unsigned int i = 0; i < v.size(); i++)  // find position of value
		pos_map.emplace(v[i], i);
	return pos_map;
}
template<typename T>
inline auto value_iter_map(const std::vector<T>& v)
{
	std::unordered_map<T, std::vector<T>::const_iterator> pos_map;  // use const iterator because vector is const
	for (auto x = begin(v); x != end(v); ++x)  // find position of value
		pos_map[*x]=x;
	return pos_map;
}

inline int ceiling_int(int divdend, int divisor)
{
	return (divdend + divisor - 1) / divisor;
}
// matrix of value 0 or 1, 1 - land, 0 - water
inline vector<set<int>> connectedComponents(std::vector<vector<int>>& matrix, std::vector<vector<int>>& dir)
{
	int rows = matrix.size();
	if (rows == 0)
		return{};
	int cols = matrix[0].size();
	auto empty_cell = [](int val) { return val == 0; };
	vector<set<int>> components;
	set<int>		visited;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if (!empty_cell(matrix[r][c]) && !visited.count(r*cols + c)) {
				std::deque<std::pair<int, int>> bfs{ { r,c } };
				components.push_back(set<int>{});
				visited.insert(r*cols+c);
				while (!bfs.empty()) {
					int idx = bfs.front().first*cols + bfs.front().second;  // process front
					components.back().insert(idx);
					for (const auto& d : dir) {  // check 8 directions
						int r1 = bfs.front().first + d[0];
						int c1 = bfs.front().second + d[1];
						if (r1 >= 0 && r1 < rows &&c1 >= 0 && c1 < cols && !empty_cell(matrix[r1][c1])) {
							if (!visited.count(r1*cols + c1)) {
								bfs.emplace_back(r1, c1);
								visited.insert(r1*cols + c1);
							}
						}
					}
					bfs.pop_front();
				}
			}
		}
	}
	return components;
}

inline int gcd(int p, int q)
{
	return q == 0 ? p : gcd(q, p%q);
}

inline vector<vector<int>> rightTrianglesFundamental(long long N) // primitive right angled triangle
{
	// Euclid method: M^2-n^2, 2mn, m^2+n^2, p=2m(m+n)>=4n^2, m>n
	vector<vector<int>> triangles;
	const int MAX_N = static_cast<int>(sqrt(N / 4)) + 1; // 4n^2 <= p
	const int MAX_M = static_cast<int>(sqrt(N / 2)) + 1;  // 2m^2 <= p
	for (int n = 1; n < MAX_N; n++) {
		int n_sq = n*n;
		for (int m = n + 1; m < MAX_M; m += 2) { // m and n must be odd parity
			if (gcd(m, n) == 1 && 2 * m*(m + n) <= N) {
				int m_sq = m*m;
				int n_sq = n*n;
				triangles.push_back({ m_sq - n_sq, 2 * m*n, m_sq + n_sq });
			}
		}
	}
	//	cout << "fundamental triangles " << triangles.size() << "\n";
	return triangles;
}

template<typename T>  // count T and cache max count
class count_max
{
	std::map<T, int> counter;
	int max_count = 0;
	T max_count_val = 0;
public:
	void update(T&& n) {
		auto& x = counter[n];
		x++;
		if (x>max_count) {
			max_count = x;
			max_count_val = move(n);
		}
	}
	std::pair<T, int> get() const
	{
		return { max_count_val, max_count };
	}
};


namespace std 
{	// hash for pair class
	template<typename PAIR_T1, typename PAIR_T2> struct hash<pair<PAIR_T1, PAIR_T2>>
	{
		typedef pair<PAIR_T1, PAIR_T2> argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& s) const noexcept
		{
			result_type const h1(std::hash<PAIR_T1>{}(s.first));
			result_type const h2(std::hash<PAIR_T2>{}(s.second));
			return h1 ^ (h2 << 1);
		}
	};

	// overload for pair class
	template<typename T1, typename T2>
	ostream& operator<<(ostream& os, const pair<T1, T2>& p)
	{
		os << "(" << p.first << ", " << p.second << ")";
		return os;
	}
}

// Project Euler #61: Cyclical figurate numbers
// Problem description: n nodes (or polygon number type)
//   a pair of cyclic number generate an edge from node a to b
// find complete set of node with number in cycle, number can not duplicate
// print sum of all cyclic numbers
// store all edges from one polygon type to another, based on cyclic number
// use dfs to search all edges for result of a complete set without duplicate polygon or cyclic value
// Trick case to remove polygon from result when dfs return false
//   need to remember if it existed before dfs call
template<typename T1, typename T2>  // T1 is node index, T2 is node value
class Cycle_Finder
{
	map<pair<T1, T2>, set<pair<T1, T2>>>  edges;
public:
	void add(pair<T1, T2> left, pair<T1, T2> right) {
		//cout << " edge " << left << " -> " << right << "\n";
		edges[left].insert(right);
	}
	bool dfs(const pair<T1, T2>& from, const pair<T1, T2>& first, map<T1, T2>& result, int size) {
		if (result.find(from.first) != end(result)) { // exist already
			if ((int)result.size() != size || from != first)  // last one circle back to first
				return false;
			set<int> uniq_val;
			for (const auto& r : result)
				uniq_val.insert(r.second);
			return (int)uniq_val.size() == size; //result must have unique values
		}
		result[from.first] = from.second;
		//cout << "add " << from << " result size " << result.size() << "\n";
		for (const auto& p : edges[from]) {
			bool exist = result.find(p.first) != end(result); // BUG source: save state if it exists
			if (dfs(p, first, result, size))
				return true;
			auto last = result.find(p.first);
			if (!exist && last != end(result)) {  // BUG source: make sure last one was added!!!
				result.erase(p.first);  // remove last added
										//cout << "remove " << p << "\n";
			}
			//else
			//    cout << "no remove " << p << "\n";
		}
		return false;
	}
	set<int> sum_cycle(int size) {
		set<int> ans;
		//int loop=0;
		for (const auto& e : edges) {
			map<int, int> result;
			//cout << loop++ << " start\n";
			if (dfs(e.first, e.first, result, size)) {
				int sum = accumulate(begin(result), end(result), 0, [](int init, const auto& p) { return init + p.second; });
				ans.insert(sum);
			}
		}
		return ans;
	}
};
