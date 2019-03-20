#pragma once
template<typename T>
class SortedVector
{
	vector<T>  data;
public:
	SortedVector() {}
	SortedVector(std::initializer_list<T> l) : data(l)
	{
		sort(begin(data), end(data));
	}
	template<typename InputIter>
	SortedVector(InputIter first, InputIter last) : data(first, last)
	{
		sort(begin(data), end(data));
	}
	void insert(T val) {
		if (data.empty())
			data.push_back(val);
		else {
			data.insert(lower_bound(begin(data), end(data), val), val);
		}
	}
	void erase(T val) noexcept
	{
		auto found = lower_bound(begin(data), end(data), val);
		if (found != end(data) && *found == val)
			data.erase(found);
	}
	const vector<T>& get() const {
		return data;
	}
};
