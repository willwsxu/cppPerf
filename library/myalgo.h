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

// get map value or return default if key does not exist
template<typename Map, typename Key, typename Value>
Value map_get_or_default(const Map& m, Key k, Value deft)
{
	auto found = m.find(k);
	if (found == m.end())
		return deft;
	return found->second;
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

// untested!!
template<typename InpI, typename Pred>
int continuousLongestBy(InpI first, InpI last, Pred isGood)  // long substring that conform to a condition
{
	int max = -1;
	int start = 0;
	int next = 0;
	for (auto x = first; x != last; ++x) {
		if (isGood(*x)) {  // reset
			if (next - start > max)
				max = next - start;
			next = 0;
			start = 0;
		}
		else {
			next++;
		}
	}
	if (next - start > max)
		max = next - start;
	return max;
}

template<typename RandIter, typename ValueT>
std::pair<ValueT, ValueT> maxSubSum(RandIter first, RandIter last, ValueT _max_seed, ValueT init_sum=0)
{
	while (first != last) {
		init_sum += *first;
		++first;
		_max_seed = max(_max_seed, init_sum);
		if (init_sum < 0) { // start subarray anew if sum is negative
			init_sum = 0;
		}
	}
	return{ _max_seed,init_sum };
}

// 344. Reverse String
template<typename Iter>
void reverse_string(Iter first, Iter last)
{
	while (first != last && first != --last)
		iter_swap(first++, last);
}

template<typename Iter, typename Pred>
void reverse_string_if(Iter first, Iter last, Pred cond)
{
	if (first == last)
		return;
	--last;
	while (first != last) {
		if (!cond(*first))
			++first;
		else if (!cond(*last))
			--last;
		else {
			iter_swap(first++, last);
			if (first == last--)
				break;
		}
	}
}

// count of consecutive values in an array, return all counts
template<typename RaIter, typename T>
std::vector<int> count_consecutive(RaIter first, RaIter last, T target)
{
	std::vector<int> counts;
	int cnt = 0;
	while (first != last) {
		if (*first == target)
			cnt++;
		else if (cnt>0) {
			counts.push_back(cnt);
			cnt = 0;
		}
		++first;
	}
	if (cnt>0)
		counts.push_back(cnt);
	return counts;
}