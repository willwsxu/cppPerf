#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <numeric>

using namespace std;
using pii = pair<int, int>;
namespace std {
	ostream& operator<<(ostream& os, pair<int, int> p) {
		os << p.first << " " << p.second;
		return os;
	}
}
pair<int, int> road_signs(vector<int> A, vector<int> B)
{
	int S = A.size();
	if (S<2)
		return { S, S };

	auto calc_seq = [](const vector<int>& a) {// track start and end of a sequence of same number
		vector<pair<int, int>> m;
		m.reserve(a.size());
		int start = 0;
		for (int end = 1; end < (int)a.size(); end++) {
			if (a[end] != a[start]) {
				for (int i = start; i < end; i++) {
					m.emplace_back(start, end - 1);
				}
				start = end;
			}
		}
		int end = a.size();
		for (int i = start; i < end; i++) {
			m.emplace_back(start, end - 1);
		}
		return m;
	};
	vector<pair<int, int>> M = calc_seq(A);
	vector<pair<int, int>> N = calc_seq(B);
	int max_seq = 1;
	set<pii> uniq_seq{ {0,0} };  // keep track of sign sequence
	// M= 9   9 18 22 22 3  5  5
	// n=-10 -5 7 -1 -1 -1 -1  2
	auto scan_max = [&max_seq, &uniq_seq](const vector<pair<int, int>>& m, const vector<pair<int, int>>& n, 
		const vector<int>& a, const vector<int>& b) {
		size_t start = 0;
		while (start < m.size()) {
			int M_val = a[start];  // fix M value
			size_t scan = m[start].second+1;  // scan east bound until val!=M
			if (scan < m.size()) {
				int N_val = b[scan];  // fix N value
				while (scan < m.size() && (a[scan] == M_val||b[scan]==N_val)) {  // repeat scan of sign that is same as M or N
					if (a[scan] == M_val)
						scan = m[scan].second + 1;
					else if ( b[scan] == N_val)
						scan = n[scan].second + 1;
				}
			}
			int new_len = scan-start;
			if (new_len >= max_seq) {
				if (new_len > max_seq)
					uniq_seq.clear();
				max_seq = new_len;
				uniq_seq.emplace(start, scan);
			}
			if (scan == m.size())  // done as next round won't find longer sequence
				break;
			start= m[start].second+1;
		}
	};
	scan_max(M, N, A, B);
	scan_max(N, M, B, A);
	return { max_seq , uniq_seq.size() };
}

void test_onlineB2()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int S;
		cin >> S;
		vector<int> D, A, B;
		D.reserve(S);
		A.reserve(S);
		B.reserve(S);
		for (int i = 0; i < S; i++) {
			int d, a, b;
			cin >> d >> a >> b;
			D.push_back(d);
			A.push_back(a);
			B.push_back(b);
		}
		// west side sign mean dist M=A+D,  east side sign means N=D-B
		for (int i = 0; i < (int)A.size(); i++) {
			A[i] += D[i];
			B[i] = D[i] - B[i];
		}
		cout << "Case #" << t;
		cout << ": " << road_signs(A, B) << "\n";
	}
}

// 1 gram of metal i can be created from 1 gram of metal j and 1 gram of metal k
// j and k are different, but one of them can be same as i
bool valid(const vector<pair<int, int>>& metals, vector<long long>& grams, int idx, set<int>&parent)
{
	if (grams[idx] >= 0)
		return true;
	if (idx>= metals.size() || parent.find(idx) != end(parent))
		return false;
	parent.insert(idx);
	if (idx == metals[idx].first - 1 || idx == metals[idx].second - 1) {  // a+b->a, if a<0, a will never be >=0
		parent.erase(idx);
		return false;
	}
	grams[metals[idx].first-1] += grams[idx];  // convert metal id to 0 based
	grams[metals[idx].second-1] += grams[idx];
	grams[idx] = 0;
	bool res= valid(metals, grams, metals[idx].first-1, parent) && valid(metals, grams, metals[idx].second-1, parent);
	parent.erase(idx);
	return res;
}

using vpii = std::vector<pair<int, int>>;
long long transmutation(vpii metals, vector<long long> grams)
{
	long long high = accumulate(begin(grams), end(grams), 0LL);  // metal 0 is the traget to maximize
	long long low = 0;
	while (low < high) {
		// low=0, high=1, mid=1 if valid, [1,1] else [0,0]
		long long mid = (high + low+1) / 2;  // binary search, choose mid so loo will end properly
		vector<long long> trial = grams;
		trial[0] -= mid;  // if top node is negative, propate down to see if become none negative
		set<int> parent;
		if (valid(metals, trial, 0, parent))
			low = mid;
		else
			high = mid - 1;
	}
	return low;
}
void test_onlineB3()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int M;
		cin >> M;
		vpii metals;
		metals.reserve(M);
		for (int i = 0; i < M; i++) {
			int a, b;
			cin >> a >> b;
			metals.emplace_back(a, b);
		}
		vector<long long> grams;
		grams.reserve(M);
		for (int i = 0; i < M; i++) {
			int g;
			cin >> g;
			grams.push_back(g);
		}
		cout << "Case #" << t;
		cout << ": " << transmutation(metals, grams) << "\n";
	}
}
#include "catch.hpp"
TEST_CASE("No 3. transmutation", "[J1B3]")
{
	//test_onlineB3();
	CHECK(transmutation(vpii{ {2,3}, {5,4},{4,2} }, vector<long long>{0, 4, 9, 20, 20}) == 14);
	CHECK(transmutation(vpii{ {2,3}, {5,4},{4,3} }, vector<long long>{0, 4, 9, 20, 20}) == 9);
	CHECK(transmutation(vpii{ {2,3}, {3,4},{4,2} }, vector<long long>{0, 4, 15, 20}) == 9);
	CHECK(transmutation(vpii{ {2,3}, {3,4},{4,2}}, vector<long long>{0, 5, 15, 20}) == 10);
	CHECK(transmutation(vpii{ {3,4}, {3,4},{4,5},{3,5},{1,3} }, vector<long long>{0, 8, 6, 2, 4}) == 4);
}

TEST_CASE("No 2. road sign numbering", "[J1B2]")
{
	CHECK(road_signs(vector<int>{9,9,8,22,22}, vector<int>{-10,-5,7,-1,-1}) == pair<int,int>{3, 2});
	CHECK(road_signs(vector<int>{6,8,7,8,8,9}, vector<int>{-1,-1,0,2,2,0}) == pair<int, int>{5, 1});
	CHECK(road_signs(vector<int>{6, 8, 7, 8, 8, 9}, vector<int>{2,2,1,2,2,1}) == pair<int, int>{5, 2});
	//test_onlineB2();
}