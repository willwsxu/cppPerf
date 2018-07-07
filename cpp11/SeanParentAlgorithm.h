#pragma once

template <typename In, typename Pred>
In stable_partition(In first, In last, Pred p)
{
	auto n = last - first;
	if (n == 0)
		return first;
	if (n == 1)
		return first + p(*first);
	auto m = first + n / 2;
	return rotate(stable_partition(first, m, p), m,
		stable_partition(m, last, p));
}


template <typename In, typename Pred>
In stable_partition_position(In first, In last, Pred p)
{
	auto n = last - first;
	if (n == 0)
		return first;
	if (n == 1)
		return first + p(first);
	auto m = first + n / 2;
	return rotate(stable_partition_position(first, m, p), m,
		stable_partition_position(m, last, p));
}

template <typename In>
void sort_subrange(In first, In last, In sf, In sl)
{
	if (sf == sl)
		return;
	if (sf != first) {
		nth_element(first, sf, last);
		++sf; // sf is already sorted
	}
	partial_sort(sf, sl, last);
}
