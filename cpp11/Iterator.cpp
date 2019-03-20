#include <vector>

using namespace std;

template<typename T>
class Iterator
{
	vector<T>  data;
	int idx = 0;
public:
	Iterator(vector<T>&& d) :data(move(d)) {}
	virtual T next() {
		return data[idx++];
	}
	virtual bool hasNext() {
		return idx < (int)data.size();
	}
	void reset()
	{
		idx = 0;
	}
};

template<typename T>
class IterIterator
{
	using Iters = vector<Iterator<T>>;
	Iters  iters;
	int idx = 0;
public:
	IterIterator(Iters&& d) :iters(move(d)) {
		for (auto& it : iters)
			it.reset();
	}
	T next() {
		return iters[idx].next();
	}
	bool hasNext() {
		while (idx < (int)iters.size() && !iters[idx].hasNext())
			idx++;
		return idx < (int)iters.size();
	}
};

#include "..\catch.hpp"

auto count_iter = [](auto& iter) {
	int count = 0;
	while (iter.hasNext()) {
		iter.next();
		count++;
	}
	return count;
};

TEST_CASE("Foursquare iterator test", "[FOUR]")
{
	vector<int> d1{}, d3{};
	vector<int> d2{ 1,2,3,4,5 };
	vector<int> d4{ 10,20,30,40 };
	Iterator<int> it1(move(d1));
	Iterator<int> it2(move(d2));
	Iterator<int> it3(move(d3));
	Iterator<int> it4(move(d4));
	CHECK(count_iter(it1) == 0);
	CHECK(count_iter(it4) == 4);

	IterIterator<int> iters1({ it1,it3 });
	CHECK(count_iter(iters1) == 0);
	IterIterator<int> iters2({ it1,it2,it3,it4 });
	CHECK(count_iter(iters2) == 9);
	IterIterator<int> iters3({ it1,it2,it4,it3 });
	CHECK(count_iter(iters3) == 9);
}

template<typename ITER>
class IteratorX
{
	using vii = vector<pair<ITER, ITER>>;
	vii iters;
	size_t idx = 0;
	ITER start;
	using value_type = typename iterator_traits<ITER>::value_type;
public:
	IteratorX(vii&& it) :iters(it)
	{
		if (iters.size() > 0) {
			idx = 0;
			start = iters[0].first;
		}
	}
	value_type next() {
		return *start++;
	}
	bool hasNext()
	{
		if (idx < iters.size() && start != iters[idx].second)
			return true;
		while (++idx < iters.size() && iters[idx].first == iters[idx].second)
			;
		if ( idx < iters.size() )
			start = iters[idx].first;
		return idx < iters.size();
	}
};


TEST_CASE("Foursquare iterator test 2", "[FOUR]")
{
	vector<int> d1{}, d3{};
	vector<int> d2{ 1,2,3,4,5 };
	vector<int> d4{ 10,20,30,40 };
	using ITER = vector<int>::iterator;
	using pii = pair<ITER, ITER>;
	IteratorX<ITER> iter1(vector<pii>{ {begin(d1),end(d1)} });
	CHECK(count_iter(iter1) == 0);
	IteratorX<ITER> iter2(vector<pii>{ {begin(d1), end(d1)}, { begin(d2), end(d2) }, { begin(d3), end(d3) }, { begin(d4), end(d4) } });
	CHECK(count_iter(iter2) == 9);

	IteratorX<ITER> iter3(vector<pii>{ {begin(d1), end(d1)}, { begin(d2), end(d2) }, { begin(d4), end(d4) }, { begin(d3), end(d3) } });
	CHECK(count_iter(iter3) == 9);
}