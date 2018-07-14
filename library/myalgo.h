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

template< class InputIt, class OutputIt, class UnaryOp, class Pred>
OutputIt transform_if(InputIt first1, InputIt last1, OutputIt d_first, UnaryOp unary_op, Pred pred)
{
	while (first1 != last1) {
		if (pred(*first1))
			*d_first++ = unary_op(*first1);
		++first1;
	}
	return d_first;
}

inline std::vector<std::string> tokenizer(std::string::const_iterator sb, std::string::const_iterator se, const char *sep)
{
	std::regex rgx(sep);
	std::vector<std::string> tokens;
	copy(std::sregex_token_iterator(sb, se, rgx, -1), std::sregex_token_iterator(), std::back_inserter(tokens));
	return tokens;
}

inline std::pair<int, int> multiplyComplex(int r1, int i1, int r2, int i2)
{
	return{ r1*r2 - i1*i2, r1*i2 + r2*i1 };
}

// hash function independent of element order
// unique_hash

template<class ForwardIterator1, class ForwardIterator2>
bool subsequence(ForwardIterator1 first1, ForwardIterator1 last1,  // first sequence contain subsequence of the second
	ForwardIterator2 first2, ForwardIterator2 last2)
{
	while (first1 != last1 && first2 != last2) {
		if (*first1 == *first2) {
			first1++;
			first2++;
		}
		else
			first1++;
	}
	return first2 == last2;
}
