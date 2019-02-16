#pragma once
#include <memory>
#include <vector>

class Trie
{
protected:
	static const int CHILD = 26;
	template<typename Label>
	struct Node
	{
		Label val{};  // support 3 types of labels, string, bool, int
		std::unique_ptr<Node> child[CHILD];
		void setLabel(const std::string& v, Label v2 = Label{}) {  // require specialization for each type
			val = v;
		}
		bool complete() { return false; }  // the node that completes a word, require specialization for each type

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


class BinaryTrie  // binary number digit
{
	struct Node {
		char val = 0;
		std::unique_ptr<Node> children[2];
		Node(char v = 0) :val(v) {}
	};
	Node root; // root value is always 0
	std::vector<int> binary;    // used for converting number to binary digits
	void num_to_binary(unsigned int num) {
		fill(begin(binary), end(binary), 0);
		int index = binary.size() - 1;
		while (num>0) {
			binary[index--] = num % 2;
			num /= 2;
		}
		//copy(begin(binary),end(binary), ostream_iterator<int>(cout, " "));
		//cout << "\n";
	}
	void put(Node& node, const vector<int>& digits, int idx)
	{
		if (idx == digits.size())
			return;
		if (!node.children[digits[idx]])
			node.children[digits[idx]] = std::make_unique<Node>(digits[idx]);
		put(*node.children[digits[idx]].get(), digits, idx + 1);
	}
	int find_xor_max(Node& node, const vector<int>& digits, int idx, int value)
	{
		if (idx == digits.size())
			return value;
		int d = 1 - digits[idx];
		Node * child = node.children[d].get();  // look for child of opposite
		if (!child) {
			d = 1 - d;
			child = node.children[d].get();
		}
		return find_xor_max(*child, digits, idx + 1, value * 2 + d);
	}
public:
	BinaryTrie(int bits = 31) :binary(bits, 0) {}
	void add(unsigned int num) {
		num_to_binary(num);
		put(root, binary, 0);
	}
	int find_xor_max(unsigned int num) {
		num_to_binary(num);
		int found = find_xor_max(root, binary, 0, 0);
		//cout << num << " " << found << "\n";
		return num^found;
	}
};