#include <vector>
#include <set>
#include <iostream>
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
#include "catch.hpp"

TEST_CASE("No 2. road sign numbering", "[J1B2]")
{
	CHECK(road_signs(vector<int>{9,9,8,22,22}, vector<int>{-10,-5,7,-1,-1}) == pair<int,int>{3, 2});
	CHECK(road_signs(vector<int>{6,8,7,8,8,9}, vector<int>{-1,-1,0,2,2,0}) == pair<int, int>{5, 1});
	CHECK(road_signs(vector<int>{6, 8, 7, 8, 8, 9}, vector<int>{2,2,1,2,2,1}) == pair<int, int>{5, 2});
	//test_onlineB2();
}