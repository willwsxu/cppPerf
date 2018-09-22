#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
#include "trie.h"

using namespace std;
class DictTrie : public Trie
{
	using Node_Bool = Node<bool>;
	Node_Bool root;
public:
	void put(const string& word)
	{
		root.put(word, 0);
	}
	bool search(string word) {
		return root.find(word, 0);
	}

	// 208. Implement Trie(Prefix Tree). Reuse class with #720. beat 57%
	bool startsWith(string prefix) {
		return root.find_prefix(prefix, 0)!=nullptr;
	}

	bool completeAll(Node_Bool& cur, const string& word, int idx) {
		if (idx == word.size())
			return true;
		const int next = word[idx] - 'a';
		if (!cur.child[next] || !cur.child[next]->val)
			return false;
		return completeAll(*cur.child[next].get(), word, idx + 1);
	}
	// 720. Longest Word in Dictionary, which can be built one letter at a time
	string longestWord(vector<string>& words) {  // beat 84%
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

	// allow exact one letter different
	bool magic_search(Node_Bool& cur, const string& word, int idx, int mod)
	{
		if (idx == word.size())
			return mod == 1 && cur.val;  // make sure it ends with word in dict
		if (mod == 1)
			return cur.find(word, idx);  // 1 letter modified, do normal search
		for (int i = 0; i < CHILD; i++) {  // no letter changed, go through each valid child
			if (cur.child[i]) {
				if (magic_search(*cur.child[i].get(), word, idx + 1, 'a' + i == word[idx] ? 0 : 1))  // update mod as appropriate
					return true;
			}
		}
		return false;
	}

	void buildDict(vector<string> dict) {
		for (const string& w : dict)
			root.put(w, 0);  // build trie        
	}
	// 676. Implement Magic Dictionary
	bool magic_search(string word) {  // beat 100%
		return magic_search(root, word, 0, 0);
	}
};

TEST_CASE("676. Implement Magic Dictionary", "[TRIE]")
{
	DictTrie magic;
	magic.buildDict(vector<string>{"hello", "leetcode"});
	CHECK(magic.magic_search("hello") == false);
	CHECK(magic.magic_search("hhllo") == true);
	CHECK(magic.magic_search("hell") == false);
	CHECK(magic.magic_search("leetcoded") == false);
}

TEST_CASE("676. Implement Magic Dictionary test 2", "[TRIE]")
{
	DictTrie magic;
	magic.buildDict(vector<string>{"a", "b", "ab", "abc", "abcabacbababdbadbfaejfoiawfjaojfaojefaowjfoawjfoawj", "abcdefghijawefe", "aefawoifjowajfowafjeoawjfaow", "cba", "cas", "aaewfawi", "babcda", "bcd", "awefj"});
	CHECK(magic.magic_search("ab") == false);
	CHECK(magic.magic_search("ba") == false);
	CHECK(magic.magic_search("abcd") == false);
}


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


TEST_CASE("421. Maximum XOR of Two Numbers in an Array", "[TRIE]")
{
	CHECK(TrieInt().findMaximumXOR(vector<int>{3, 10, 5, 25, 2, 8}) == 28);
}

class MapSum : public Trie {
	using Node_Int = Node<int>;
	Node_Int root;
	int sum(const Node_Int& cur)  // sum all values of all words from this prefix node
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
	MapSum() {	}

	void insert(string key, int val) {
		root.put(key, 0, val);
	}

	int sum(string prefix) { // beat 100%
		const auto *found = root.find_prefix(prefix, 0);
		return found? sum(*found):0;
	}
};

TEST_CASE("677. Map Sum Pairs", "[TRIE]")
{
	MapSum m;
	m.insert("a", 3);
	CHECK(m.sum("ap") == 0);
	m.insert("b", 2);
	CHECK(m.sum("a") == 3);
}

class TrieWords : public Trie
{
	using Node_Str = Node<string>;
	Node_Str root;
	const string * findRoot(Node_Str& cur, const string& word, int idx) {
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
public:
	// 648. Replace Words, replace word with root in dict
	string replaceWords(vector<string>& dict, string sentence) {  // beat 56%
		for (const string& w : dict)
			root.put(w, 0);  // build trie
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
};
TEST_CASE("648. Replace Words", "[TRIE]")
{
	CHECK(TrieWords().replaceWords(vector<string>{"cat", "bat", "rat", "af"}, "a cattle was rattled by the battery") == "a cat was rat by the bat");
}

class WordDictionary : Trie { // beat 50%
	using Node_Bool = Node<bool>;
	Node_Bool root;
public:
	/** Initialize your data structure here. */
	WordDictionary() {	}

	/** Adds a word into the data structure. */
	void addWord(string word) {
		if (!word.empty())
			root.put(word, 0);
	}

	/** Returns if the word is in the data structure. A word could contain the dot character '.' to represent any one letter. */
	bool search(string word) {
		if (word.empty())
			return true;
		return root.find_wild(word, 0);
	}
};

/**
* Your WordDictionary object will be instantiated and called as such:
* WordDictionary obj = new WordDictionary();
* obj.addWord(word);
* bool param_2 = obj.search(word);
*/
TEST_CASE("211. Add and Search Word - Data structure design", "[TRIE]")
{
	WordDictionary d;
	d.addWord("bad");
	d.addWord("dad");
	d.addWord("mad");
	CHECK(d.search("pad") == false);
	CHECK(d.search("bad") );
	CHECK(d.search(".ad") );
	CHECK(d.search("b..") );
	CHECK(d.search("ba") == false);
	CHECK(d.search(".") == false);
}
