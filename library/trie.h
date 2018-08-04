#pragma once
#include <memory>

class Trie
{
protected:
	static const int CHILD = 26;
	template<typename Label>
	struct Node
	{
		Label val{};
		std::unique_ptr<Node> child[CHILD];
		void setLabel(const std::string& v, Label v2 = Label{}) {
			val = v;
		}
		bool complete() { return false; }  // the node that completes a word

		void put(const std::string& word, int idx, const Label& b = Label{})
		{
			if (idx == word.size()) {
				setLabel(word, b);
				return;
			}
			const int next = word[idx] - 'a';
			if (!child[next])
				child[next] = make_unique<Node>();
			child[next]->put(word, idx + 1, b);
		}
		bool find_wild(const std::string& word, int pos)
		{
			if (pos == word.size())
				return complete();
			if (word[pos] == '.') {  // wild char matching
				for (auto& c : child) {
					if (c) {
						if (c->find_wild(word, pos + 1))
							return true;
					}
				}
				return false;
			}
			const int idx = word[pos] - 'a';
			return child[idx] ? child[idx]->find_wild(word, pos + 1) : false;
		}
	};
	//template<typename Label> Node<Label> root;

	template<typename Label>
	void put(Node<Label>& cur, const std::string& word, int idx, const Label& b = Label{})
	{
		if (idx == word.size()) {
			cur.setLabel(word, b);
			return;
		}
		const int next = word[idx] - 'a';
		if (!cur.child[next])
			cur.child[next] = make_unique<Node<Label>>();
		put(*cur.child[next].get(), word, idx + 1, b);
	}

	template<typename Label>
	const Node<Label>* find(Node<Label>& cur, const std::string& prefix, int idx) const  // find node with prefix
	{
		if (idx == prefix.size())
			return &cur;
		const int next = prefix[idx] - 'a';
		if (cur.child[next])
			return find(*cur.child[next].get(), prefix, idx + 1);
		return nullptr;
	}
};
template<>
void Trie::Node<bool>::setLabel(const std::string& v, bool)
{
	val = true;
}
template<>
void Trie::Node<int>::setLabel(const std::string&, int v2)
{
	val = v2;
}

template<>
bool Trie::Node<int>::complete()
{
	return true;
}
template<>
bool Trie::Node<bool>::complete()
{
	return val;
}
template<>
bool Trie::Node<std::string>::complete()
{
	return !val.empty();
}
