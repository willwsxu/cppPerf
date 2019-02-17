#include "..\catch.hpp"

#include <chrono>
#include <deque>
#include <map>
#include <numeric>

using namespace std;

using time_seconds = long long;
struct Count
{
	time_seconds time;
	int  count;
	Count(time_seconds t, int c):time(t),count(c) {  // required for emplace_back
	}
};

time_seconds now_in_seconds()
{
	auto epoch = chrono::steady_clock::now().time_since_epoch();
	return chrono::duration_cast<chrono::seconds>(epoch).count();
}
// Google screen
// objective: get request count for last 60s
class RequestCounter {
	deque<Count> counts;

	void remove_old(time_seconds time)
	{
		while (!counts.empty()) {
			if (counts.front().time + 60 < time)
				counts.pop_front();
		}
	}
public:
	void Increment()
	{
		auto time = now_in_seconds();
		remove_old(time);
		if (counts.empty() || time > counts.back().time)
			counts.emplace_back(time, 1);
		else
			counts.back().count++;
	}

	int Get60sCount() {
		remove_old(now_in_seconds());
		return accumulate(begin(counts), end(counts), 0, [](int init, const auto& cnt) { return init + cnt.count; });
	}
};


class RequestCounter2 {
	map<time_seconds, int>  counts;

public:
	void Increment()
	{
		counts[now_in_seconds()]++;
	}
	int Get60sCount() {
		auto found = counts.lower_bound(now_in_seconds() - 60);
		return accumulate(begin(counts), end(counts), 0, [](int init, const auto& cnt) { return init + cnt.second; });
	}
};
