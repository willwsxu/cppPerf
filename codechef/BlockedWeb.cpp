
#include <string>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Trie26
{
	static const int R = 26;
	static const char base = 'a';

	class Node
	{
		string name;
		Node *next[R];
	public:
		Node()
		{
			for (int i = 0; i < R; i++)
				next[i] = nullptr;
        }

		Node *createNext(char n)
		{
			int ind = n - base;
			if (ind >= R || ind < 0)
				return nullptr;
			if (next[ind] == nullptr)
				next[ind] = new Node();
			return next[ind];
		}

		Node *getNext(char n)
		{
			int ind = n - base;
			if (ind >= R || ind < 0)
				return nullptr;
			return next[ind];
		}

		void set(string &n)
		{
			name = n;
		}
	};
	Node root;

public:
	void put(string key)
	{
		Node *cur = &root;
		for (size_t i = 0; i < key.size(); i++) {
			cur = cur->createNext(key[i]);
			if (cur == nullptr) {
				cerr << "trie put error" << endl;
				return;
			}
		}
		cur->set(key);
	}
	
    int match(string key)
	{
		Node *cur = &root;
		for (size_t i = 0; i < key.size(); i++) {
			cur = cur->getNext(key[i]);
			if (cur == nullptr) {
				return i;
			}
		}
		return key.size();
	}
};

void trieTest()
{
	Trie26 tr;
	tr.put("google");
	cout << (tr.match("goo") == 3) << endl;
	cout << (tr.match("oog") == 0) << endl;
	cout << (tr.match("googler") == 6) << endl;
	cout << (tr.match("gap") == 1) << endl;
}

void solve(Trie26& tr, vector<string> blocked)
{
	set<string> ans;
	for (string s : blocked) {
		int m = tr.match(s);
		if (m == s.size()) {
			cout << -1 << endl;
			return;
		}
		else
			ans.insert(s.substr(0, m + 1));
	}
	cout << ans.size() << endl;
	for (string s : ans)
		cout << s << endl;
}

void BlockWebTest()
{
	Trie26 tr;
	vector<string> blocked = { "codeforces" };
	solve(tr, blocked);  // 1: c
	blocked.clear();
	tr.put("google");
	solve(tr, blocked); // 0
	blocked.push_back("codeforces");
	blocked.push_back("codefool");
	tr.put("codechef");
	solve(tr, blocked);  //codef
	blocked.push_back("codechill");
	solve(tr, blocked);
	blocked.push_back("youtube");
	blocked.push_back("yoyo");
	solve(tr, blocked);
	tr.put("codechefxyz");
	blocked.push_back("codechefu");
	solve(tr, blocked);
	blocked.push_back("codechefx");
	solve(tr, blocked);
}

void solve()
{
	Trie26 tr;
	vector<string> blocked;
	int n = 0;
	cin >> n;
	for (int i = 0; i < n; i++){
		string p;
		getline(cin, p);
		if (p.size() < 2) {
			getline(cin, p);
		}
		if (p[0] == '+')
			tr.put(p.substr(2));
		else
			blocked.push_back(p.substr(2));
	}
	solve(tr, blocked);
}

int main(int argc, char * argv[])
{
	solve();
	return 0;
}