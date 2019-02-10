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
		while (idx < (int)iters.size() && !iters[idx].hasNext())
			idx++;
		return iters[idx].next();
	}
	bool hasNext() {
		while (idx < (int)iters.size() && !iters[idx].hasNext())
			idx++;
		return idx < (int)iters.size();
	}
};

#include "..\catch.hpp"
TEST_CASE("Foursquare iterator test", "[TEST]")
{
	vector<int> d1{}, d3{};
	vector<int> d2{ 1,2,3,4,5 };
	vector<int> d4{ 10,20,30,40 };
	Iterator<int> it1(move(d1));
	Iterator<int> it2(move(d2));
	Iterator<int> it3(move(d3));
	Iterator<int> it4(move(d4));
	auto count_iter = [](auto& iter) {
		int count = 0;
		while (iter.hasNext()) {
			iter.next();
			count++;
		}
		return count;
	};
	CHECK(count_iter(it1) == 0);
	CHECK(count_iter(it4) == 4);

	IterIterator<int> iters1({ it1,it3 });
	CHECK(count_iter(iters1) == 0);
	IterIterator<int> iters2({ it1,it2,it3,it4 });
	CHECK(count_iter(iters2) == 9);
	IterIterator<int> iters3({ it1,it2,it4,it3 });
	CHECK(count_iter(iters3) == 9);
}