#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// constraints: 40 seats, 10 tables
// reservation can be made at 15 min of the hour
// reservation duration is 90 min
class Schedule
{
	struct Reservation
	{
		string	user;
		int		start_time;
		int		people;
		string	restaurant;
		string	location;
	};
	map<string, Reservation>  reservations; // per day
	vector<int>  schedule;
	int			 open_time;
	int			 tables = 10;
	int			 persons_per_table = 4;
	int time_to_slot(int min)
	{
		if (min < open_time)
			return 0;
		return (min - open_time) / 15;
	}
	int ceil_int(int num, int denom) {
		return (num + denom - 1) / denom;
	}
public:
	Schedule(int open, int close) : open_time(open), schedule((close - open) / 15 + 1) {}

	bool take_reservation(string user, int start_time, int people) {
		auto slot = time_to_slot(start_time);
		int sum = 0;
		for (int i = 0; i < slot; i++)
			sum += schedule[i];
		int tables_needed = ceil_int(people, persons_per_table);
		auto end = min((int)schedule.size() - 1, slot + 6);  // [start, end)
		for (int i = slot; i < end; i++) {  // check next 6 slots (90 min) for tables available
			sum += schedule[i];
			if (sum + tables_needed > tables)
				return false;
		}
		schedule[slot] += tables_needed;
		schedule[slot+6] -= tables_needed;
		return true;
	}

	vector<int> alternate_reservation_within_2hr(string user, int start_time, int people)
	{
		auto slot = time_to_slot(start_time);
		auto start = max(0, slot - 8);
		auto end = min((int)schedule.size() - 1, slot+ 8 + 6);  // [start, end)
		int sum = 0;
		for (int i = 0; i < start; i++)
			sum += schedule[i];
		int tables_needed = ceil_int(people, persons_per_table);
		int good_start = -1;
		vector<int> alternate;
		for (int i = start; i < end; i++) {
			sum += schedule[i];
			if (sum + tables_needed > tables) {
				good_start = -1;
			}
			else {
				if (good_start < 0)
					good_start = i;
				else if (i - good_start == 5) {
					alternate.push_back(good_start++);
				}
			}
		}
		return alternate;
	}
};

#include "..\catch.hpp"

TEST_CASE("Foursquare Restaurant reservation system design test", "[TEST]")
{
	Schedule sched(0, 330);  // 5.5 hours
	CHECK(sched.take_reservation("me", 90, 21)==true);
	CHECK(sched.take_reservation("me", 0, 40) == true);
	CHECK(sched.take_reservation("me", 150, 17)==false);
	CHECK(sched.take_reservation("me", 150, 16) == true);
	CHECK(sched.take_reservation("me", 210, 20)==true);
	CHECK(sched.take_reservation("me", 270, 5)==false);
	CHECK(sched.take_reservation("me", 240, 20) == true);
}