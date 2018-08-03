#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
using namespace std;

// 208. Implement Trie(Prefix Tree). Reuse class with #720. beat 4%
class DictTrie
{
	struct Node
	{
		string val;
		unique_ptr<Node> child[26];
	};
	Node root;
	void put(Node& cur, const string& word, int idx)
	{
		if (idx == word.size()) {
			cur.val = word;
			return;
		}
		const int next = word[idx] - 'a';
		if (!cur.child[next])
			cur.child[next] = make_unique<Node>();
		put(*cur.child[next].get(), word, idx + 1);
	}
	Node * search(Node& cur, const string& word, int idx)
	{
		if (idx == word.size())
			return &cur;
		const int next = word[idx] - 'a';
		return cur.child[next] ? search(*cur.child[next].get(), word, idx + 1) : nullptr;
	}
public:
	void put(const string& word)
	{
		put(root, word, 0);
	}
	bool search(string word) {
		Node *found = search(root, word, 0);
		return found ? !found->val.empty() : false;
	}
	bool startsWith(string prefix) {
		return search(root, prefix, 0) != nullptr;
	}

	bool completeAll(Node& cur, const string& word, int idx) {
		if (idx == word.size())
			return true;
		const int next = word[idx] - 'a';
		if (!cur.child[next] || cur.child[next]->val.empty())
			return false;
		return completeAll(*cur.child[next].get(), word, idx + 1);
	}

	// 720. Longest Word in Dictionary, which can be built one letter at a time
	string longestWord(vector<string>& words) {  // beat 94%
		for (const string& w : words)
			put(w);  // build trie

		string ans;
		int len = 0;
		for (const string& w : words) {
			if ((int)w.size() < len || w.size() == len && w > ans)
				continue;
			if (completeAll(root, w, 0)) {
				ans = w;
				len = ans.size();
			}
		}
		return ans;
	}

	const string * findRoot(Node& cur, const string& word, int idx) {
		if (idx == word.size() || !cur.val.empty())
			return &cur.val;
		const int next = word[idx] - 'a';
		return cur.child[next] ? findRoot(*cur.child[next].get(), word, idx + 1) : nullptr;
	}
	const string & findRoot(const string& word) {
		const string* x = findRoot(root, word, 0);
		if (!x || x->empty())  // bug source: check null or  empty string
			return word;
		return *x;
	}
	// 648. Replace Words, replace word with root in dict
	string replaceWords(vector<string>& dict, string sentence) {  // beat 56%
		for (const string& w : dict)
			put(w);  // build trie
		vector<string> words;
		stringstream iss(sentence);
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(words));
		transform(words.begin(), words.end(), words.begin(), [this](const string& word) {
			return this->findRoot(word);
		});
		stringstream oss;
		copy(words.begin(), words.end(), ostream_iterator<string>(oss, " "));
		string ret = oss.str();
		return ret.erase(ret.find_last_of(' '));
	}

	// allow exact one letter different
	bool search(Node& cur, const string& word, int idx, int mod)
	{
		if (idx == word.size())
			return mod == 1 && !cur.val.empty();  // make sure it ends with word in dict
		if (mod == 1) {
			Node *found = search(cur, word, idx);  // 1 letter modified, do normal search
			return found ? !found->val.empty() : false;
		}
		for (int i = 0; i < 26; i++) {  // no letter changed, go through each valid child
			if (cur.child[i]) {
				if (search(*cur.child[i].get(), word, idx + 1, 'a' + i == word[idx] ? 0 : 1))  // update mod as appropriate
					return true;
			}
		}
		return false;
	}

	void buildDict(vector<string> dict) {
		for (const string& w : dict)
			put(w);  // build trie        
	}
	// 676. Implement Magic Dictionary
	bool magic_search(string word) {  // beat 100%
		return search(root, word, 0, 0);
	}
};

class TrieInt {  // [0, 2^31)
	struct Node
	{
		unique_ptr<Node> child[2];
	};
	Node root;
	void put(Node& cur, const int val, const int b)  // from high to low bit
	{
		if (b<0) {
			return;
		}
		const int next = (val >> b) & 1;
		if (!cur.child[next])
			cur.child[next] = make_unique<Node>();
		put(*cur.child[next].get(), val, b - 1);
	}
	int findMax(Node& cur, const int val, const int b, int ans) {
		if (b<0)
			return ans;
		const int next = ((val >> b) & 1) ^ 1;
		if (!cur.child[next])
			return findMax(*cur.child[1 - next].get(), val, b - 1, (ans << 1));
		if (cur.child[next])
			return findMax(*cur.child[next].get(), val, b - 1, (ans << 1) | 1);
		return 0;
	}
public:
	int findMaximumXOR(vector<int>& nums) {  // beat 32%
		for (const int v : nums)
			put(root, v, 30);  // build trie

		int ans = 0;
		for (const int v : nums)
			ans = max(ans, findMax(root, v, 30, 0));
		return ans;
	}
};


TEST_CASE("421. Maximum XOR of Two Numbers in an Array", "[NEW]")
{
	CHECK(TrieInt().findMaximumXOR(vector<int>{3, 10, 5, 25, 2, 8}) == 28);
}

class MapSum {
	struct Node
	{
		int val;
		unique_ptr<Node> child[26];
		Node() :val(0) {}
	};
	Node root;
	void put(Node& cur, const string& word, int idx, const int v)
	{
		if (idx == word.size()) {
			cur.val = v;
			return;
		}
		const int next = word[idx] - 'a';
		if (!cur.child[next])
			cur.child[next] = make_unique<Node>();
		put(*cur.child[next].get(), word, idx + 1, v);
	}
	Node* find(Node& cur, const string& word, int idx)  // find node with prefix
	{
		if (idx == word.size())
			return &cur;
		const int next = word[idx] - 'a';
		if (cur.child[next])
			return find(*cur.child[next].get(), word, idx + 1);
		return nullptr;
	}
	int sum(Node& cur)  // sum all values of all words from this prefix node
	{
		int total = cur.val;
		for (auto& c : cur.child) {
			if (c)
				total += sum(*c.get());
		}
		return total;
	}
public:
	/** Initialize your data structure here. */
	MapSum() {

	}

	void insert(string key, int val) {
		put(root, key, 0, val);
	}

	int sum(string prefix) { // beat 100%
		auto *found = find(root, prefix, 0);
		if (found)
			return sum(*found);
		return 0;
	}
};

TEST_CASE("677. Map Sum Pairs", "[NEW]")
{
	MapSum m;
	m.insert("a", 3);
	CHECK(m.sum("ap") == 0);
	m.insert("b", 2);
	CHECK(m.sum("a") == 3);
}


TEST_CASE("648. Replace Words", "[NEW]")
{
	CHECK(DictTrie().replaceWords(vector<string>{"cat", "bat", "rat", "af"}, "a cattle was rattled by the battery") == "a cat was rat by the bat");
}

TEST_CASE("676. Implement Magic Dictionary", "[MAGIC]")
{
	DictTrie magic;
	magic.buildDict(vector<string>{"hello", "leetcode"});
	CHECK(magic.magic_search("hello") == false);
	CHECK(magic.magic_search("hhllo") == true);
	CHECK(magic.magic_search("hell") == false);
	CHECK(magic.magic_search("leetcoded") == false);
}

TEST_CASE("676. Implement Magic Dictionary test 2", "[NEW]")
{
	DictTrie magic;
	magic.buildDict(vector<string>{"a", "b", "ab", "abc", "abcabacbababdbadbfaejfoiawfjaojfaojefaowjfoawjfoawj", "abcdefghijawefe", "aefawoifjowajfowafjeoawjfaow", "cba", "cas", "aaewfawi", "babcda", "bcd", "awefj"});
	CHECK(magic.magic_search("ab") == false);
	CHECK(magic.magic_search("ba") == false);
	CHECK(magic.magic_search("abcd") == false);
}
