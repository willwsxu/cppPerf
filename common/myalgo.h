#pragma once

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

// hash function independent of element order
// unique_hash