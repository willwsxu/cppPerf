#include "..\catch.hpp"

#include <chrono>
#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <numeric>

using namespace std;

// google phone screen
using phone_num_type = long long;

class PhoneRegistry
{
	set<phone_num_type> phone_used;
	map<phone_num_type, phone_num_type> phone_unused;  // set<pair>==map!!!!!!!!

public:
	PhoneRegistry() {
		phone_unused[9999999999] = 100; // valid number [100, 9999999999], map end of interval to start
	}
	bool IsNumberGiven(phone_num_type phoneNumber) {
		return phone_used.count(phoneNumber) > 0;
	}
	void markNumberAsGiven(phone_num_type phoneNumber) {
		phone_used.insert(phoneNumber);

		auto unused = phone_unused.lower_bound(phoneNumber); // update unused number interval to speed up look up
		if (unused == end(phone_unused)) {
			return; // exceed largest allowed
		}
		//3 cases [100,200] phoneNumber=100, 200, or 150
		if (phoneNumber < unused->second) // not in the interval, error
		{
			cout << "error " << phoneNumber << " not in [" << unused->second << "," << unused->first << "\n";
		}
		else if (phoneNumber == unused->second) {
			if (unused->second== unused->first)  // last number in this interval
				phone_unused.erase(unused);
			else
				unused->second++;  // [101, 200]
		}
		else if (phoneNumber == unused->first) {
			phone_unused[phoneNumber - 1] = unused->second;  // [100, 199]
			phone_unused.erase(unused); // remove entry as key is reduced
		}
		else {
			phone_unused[phoneNumber - 1] = unused->second;  // [100, 149]
			unused->second = phoneNumber + 1;                // [151, 200]
		}
	}
	phone_num_type getAFreeNumber() {
		if (phone_unused.empty())
			return -1;  // error or exception
		auto unused = begin(phone_unused);
		phone_num_type r = unused->second;
		if (unused->second == unused->first)  // last number in this interval
			phone_unused.erase(unused);
		else
			unused->second++;
		return r;
	}
};