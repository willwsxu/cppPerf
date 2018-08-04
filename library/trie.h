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
		const Node* find_prefix(const std::string& prefix, int idx) const
		{
			if (idx == prefix.size())
				return this;
			const int next = prefix[idx] - 'a';
			return child[next] ? child[next]->find_prefix(prefix, idx + 1) : nullptr;
		}
		bool find(const std::string& word, int idx) const
		{
			auto * n = find_prefix(word, idx);
			return n && n->val;
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
