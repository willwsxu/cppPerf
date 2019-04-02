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
pair<int, int> road_signs(vector<int> A, vector<int> B, const vector<int> D)
{
	int S = A.size();
	if (S<2)
		return { S, S };
	// west side sign mean dist M=A+D,  east side sign means N=D-B
	for (int i = 0; i < (int)A.size(); i++) {
		A[i] += D[i];
		B[i] = D[i] - B[i];
	}

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
	auto scan_max = [&max_seq, &uniq_seq](const vector<pair<int, int>>& m, const vector<pair<int, int>>& n) {
		size_t scan = 0;  // scan sequence from 0
		while (scan < m.size()) {
			int len = m[scan].second - m[scan].first + 1;  // len of same distance
			if (scan > 0) {
				int new_len = scan - n[scan - 1].first + len;
				if (new_len > max_seq) {
					max_seq = new_len;
					uniq_seq.clear();
					uniq_seq.emplace(n[scan - 1].first, scan+len-1);
				}
				else if (new_len == max_seq)
					uniq_seq.emplace(n[scan - 1].first, scan + len - 1);
			}
			scan += len;
			if (scan < m.size()) {
				//check next seq after
				int new_len = n[scan].second - scan + 1 + len;
				if (new_len > max_seq) {
					max_seq = new_len;
					uniq_seq.clear();
					uniq_seq.emplace(scan - len, n[scan].second);
				}
				else if (new_len == max_seq)
					uniq_seq.emplace(scan - len, n[scan].second);
			}
		}
	};
	scan_max(M, N);
	scan_max(N, M);
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
		cout << "Case #" << t;
		cout << ": " << road_signs(A, B, D) << "\n";
	}
}
#include "catch.hpp"

TEST_CASE("No 2. road sign numbering", "[J1B2]")
{
	CHECK(road_signs(vector<int>{7,3,10,11,9}, vector<int>{12,11,1,12,14}, vector<int>{2,6,8,11,13}) == pair<int,int>{3, 2});
	//test_onlineB2();
}