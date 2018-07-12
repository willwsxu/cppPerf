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