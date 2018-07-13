#pragma once

#include <regex>

template<typename FwdI>
void bucket_sort(FwdI first, FwdI last, int buckets)
{
	std::vector<int> b(buckets, 0);
	for (auto m = first; m != last; ++m) {
		int v = *m;
		b[v]++;
	}
	for (int i = 0; i < buckets; i++)
	{
		if (b[i]) {
			fill_n(first, b[i], i);
			first += b[i];
		}
	}
}


template<class ForwardIt>
ForwardIt max_element_last(ForwardIt first, ForwardIt last)  // max element, last occurrence
{
	if (first == last) return last;

	ForwardIt largest = first;

	++first;
	for (; first != last; ++first) {
		if (*largest <= *first) {  // <= is what is different from max_element
			largest = first;
		}
	}
	return largest;
}

template<typename K, typename V, template<typename K, typename V, class Comp = std::less<K>, class A = std::allocator<std::pair<const K, V>>> class Map>
std::vector<V> map2vec(Map<K,V> &m)
{
	vector<V> ans;
	ans.reserve(m.size());
	transform(begin(m), end(m), back_inserter(ans), [](auto&p) {return move(p.second); });  // move groups from map to vector
	return ans;
}

std::vector<std::string> tokenizer(std::string::const_iterator sb, std::string::const_iterator se, const char *sep)
{
	std::regex rgx(sep);
	std::vector<std::string> tokens;
	copy(std::sregex_token_iterator(sb, se, rgx, -1), std::sregex_token_iterator(), std::back_inserter(tokens));
	return tokens;
}

std::pair<int, int> multiplyComplex(int r1, int i1, int r2, int i2)
{
	return{ r1*r2 - i1*i2, r1*i2 + r2*i1 };
}

// hash function independent of element order
// unique_hash