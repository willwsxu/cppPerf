//Sliding Window Maximum(Maximum of all subarrays of size k)
//Given an array and an integer k, find the maximum for each and every contiguous subarray of size k.
// http://www.geeksforgeeks.org/sliding-window-maximum-maximum-of-all-subarrays-of-size-k/
#include <vector>
#include <deque>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>

using namespace std;
class SubArrayMax
{
	vector<int> maxes;  // pre-calc all max values

public:
	SubArrayMax(int a[], int s, int e, int k)
	{
		int n = e - s;  // array [s, e)
		if (k > n)
			k = n;
		deque<int> Qi;
		int i = 0;
		for (i = 0; i < k; i++) {
			while (!Qi.empty() && a[i+s] >= a[Qi.back()+s])  // remove smaller items
				Qi.pop_back();
			Qi.push_back(i);
		}
		for (; i < n; i++) {
			maxes.push_back(Qi.front());
			while (!Qi.empty() && Qi.front() <= i - k)
				Qi.pop_front();
			while (!Qi.empty() && a[i+s] >= a[Qi.back()+s])  // remove smaller items
				Qi.pop_back();
			Qi.push_back(i);
		}
		maxes.push_back(Qi.front());
	}
	void print()
	{
		for (auto i : maxes)
			cout << i << " ";
		cout << endl;
	}
	vector<int> getMax() {
		return move(maxes);
	}
};
class SubArraySum
{
protected:
	int *ksum;
	vector<int> maxes;
public:
	SubArraySum(int a[], int n, int k) {
		if (k > n)
			k = n;
		ksum = new int[n * 2];
		for (int i = 0; i < n; i++) {
			ksum[i] = ksum[i + n] = a[i];
		}
		for (int i = n*2 - 2; i >= 0; i--) { // prefix sum from right
			ksum[i] += ksum[i + 1];
			if (i + k<n*2)
				ksum[i] -= a[(i + k) % n];  // subtract last number fell out of window frame
		}
		int last = n + n-k;
		reverse(ksum+1, ksum+last+1);

		SubArrayMax	subMax(ksum, 1, last + 1, n-k+1);
		maxes = subMax.getMax();
	}
	~SubArraySum()
	{
		delete[]ksum;
	}
};

class SubArraySumQuery : public SubArraySum
{
public:
	SubArraySumQuery(int a[], int n, int k, string q) : SubArraySum(a, n, k)
	{
		/*for (int i = 0; i < n * 2; i++)
			cout << ksum[i] << " ";
		cout << endl;

		for (auto i : maxes)
			cout << i << "-" << ksum[i + 1] << " ";
		cout << endl;*/

		vector<int> ans;
		int pos = 0;
		for (char c : q) {
			if (c == '?') {
				ans.push_back(ksum[maxes[pos] + 1]);
			}
			else
				pos = (pos + 1) % n;
		}
		for (auto i : ans)
			cout << i << " ";
		cout << endl;
	}
};

void solve()
{
	int N, K, P;
	cin >> N;
	cin >> K;
	cin >> P;
	int *a = new int[N];
	for (int i = 0; i < N; i++)
		cin >> a[i];
	string p;
	getline(cin, p);
	if (p.empty())
		getline(cin, p);
	if (K > N)
		K = N;
	SubArraySumQuery subsum(a, N, K, p);
	delete[]a;
}

void test()
{
	int arr[] = { 12, 1, 78, 90, 57, 89, 56 };
	int n = sizeof(arr) / sizeof(arr[0]);
	int k = 3;
	SubArrayMax sa(arr, 0, n, k);
	sa.print();
	reverse(arr + 1, arr + 6);
	for (int i : arr)
		cout << i << " ";
	cout << endl;

	int arr2[] = { 1, 1, 0, 1, 0, 1, 1 };
	SubArraySumQuery subsum (arr2, 7, 4, "?!!?!!!?!?!?!?"); //3 4 4 3 3 3
	SubArraySumQuery subsum2(arr2, 7, 3, "?!!?!!!?!?!?!?"); //2 3 3 3 2 3
	SubArraySumQuery subsum3(arr2, 7, 5, "?!!?!!!?!?!?!?"); //3 4 4 4 3 4
	SubArraySumQuery subsum4(arr2, 7, 6, "?!!?!!!?!?!?!?"); //4 5 5 4 4 4
	SubArraySumQuery subsum5(arr2, 7, 7, "?!!?!!!?!?!?!?");
	SubArraySumQuery subsum6(arr2, 7, 8, "?!!?!!!?!?!?!?");
	SubArraySumQuery subsum7(arr2, 7, 1, "?!!?!!!?!?!?!?");
	SubArraySumQuery subsum8(arr2, 7, 2, "?!!?!!!?!?!?!?");
}