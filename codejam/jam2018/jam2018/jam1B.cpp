#include <vector>
using namespace std;
pair<int, int> road_signs(vector<int>& A, vector<int>& B, const vector<int>& D)
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
	int max_count = 1;
	// M= 9   9 18 22 22 3  5  5
	// n=-10 -5 7 -1 -1 -1 -1  2
	auto scan_max = [&max_seq, &max_count](const vector<pair<int, int>>& m, const vector<pair<int, int>>& n) {
		int scan = 0;  // scan sequence from 0
		while (scan < m.size()) {
			int len = m[scan].second - m[scan].first + 1;  // len of same distance
			scan += len;
			if (scan >= m.size())
				break;
			//check next seq after
			int new_len = n[scan].second - scan + 1 + len;
			if (new_len > max_seq) {
				max_seq = new_len;
				max_count = 1;
			}
		}
	};
	scan_max(M, N);
	scan_max(N, M);
	return { max_seq , max_count };
}