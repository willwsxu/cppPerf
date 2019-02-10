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
		int		start_time;
		int		people;
		bool	checkedIn=false;
		string	restaurant;
		string	location;
		Reservation() :Reservation(0, 0) {}
		Reservation(int t, int p) : start_time(t), people(p) {}
	};
	vector<int>  schedule;
	// map has memory alignment issue if it is before schedule in destructor call to deallocate string. not sure why??
	map<string, Reservation>  reservations; // per day
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
	Schedule() :Schedule(0, 24 * 60) {} // default open close hour, required by map container

	bool take_reservation(string user, int start_time, int people) {
		auto slot = time_to_slot(start_time);
		if (slot + 6 >= (int)schedule.size())
			return false;
		int sum = 0;
		for (int i = 0; i < slot; i++)
			sum += schedule[i];
		int tables_needed = ceil_int(people, persons_per_table);
		for (int i = slot; i < slot + 6; i++) {  // check next 6 slots (90 min) for tables available
			sum += schedule[i];
			if (sum + tables_needed > tables)
				return false;
		}
		schedule[slot] += tables_needed;
		schedule[slot+6] -= tables_needed;
		reservations[user] = Reservation(start_time, people);
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
	bool checkin(string user) {
		auto found = reservations.find(user);
		if (found == end(reservations))
			return false;
		found->second.checkedIn = true;
		return true;
	}
};

class ReservationSystem
{
	struct Date {
		short year;
		char  month;
		char  day;
		friend bool operator<(const Date& lhs, const Date& rhs) {
			if (lhs.year != rhs.year)
				return lhs.year < rhs.year;
			if (lhs.month != rhs.month)
				return lhs.month < rhs.month;
			return lhs.day < rhs.day;
		}
	};
	map<string, map<Date, Schedule>>  all_reservations;  // per restaurant, per day
public:
	bool take_reservation(string user, int start_time, int people, Date date, string restaurant) {
		return all_reservations[restaurant][date].take_reservation(move(user), start_time, people);
	}
	vector<int> alternate_reservation_within_2hr(string user, int start_time, int people, Date date, string restaurant)
	{
		return all_reservations[restaurant][date].alternate_reservation_within_2hr(move(user), start_time, people);
	}
	bool checkin(string user, Date date, string restaurant) {
		return all_reservations[restaurant][date].checkin(move(user));
	}
};


#include "..\catch.hpp"

TEST_CASE("Foursquare Restaurant reservation system design test", "[TEST]")
{
	Schedule sched(0, 330);  // 5.5 hours
	CHECK(sched.take_reservation("me", 90, 21)==true);
	CHECK(sched.take_reservation("me", 0, 40) == true);
	CHECK(sched.take_reservation("me", 150, 17)==false);
	// 10 0  0  0  0  0 -4 0 0 0 0 0 -6 0 0 0 0  0  0  0 0 0 0
	// 10 10 10 10 10 10 6 6 6 6 6 6  0 0 0 0 0  0  0  0 0 0 0
	CHECK(sched.take_reservation("me", 150, 17) == false);
	CHECK(sched.alternate_reservation_within_2hr("me", 150, 17) == vector<int>{12,13,14,15,16});
	CHECK(sched.alternate_reservation_within_2hr("me", 270, 16) == vector<int>{10, 11, 12, 13, 14, 15, 16});
	CHECK(sched.alternate_reservation_within_2hr("me", 75, 16) == vector<int>{6,7,8,9,10,11,12,13});

	CHECK(sched.take_reservation("me", 150, 16) == true);
	CHECK(sched.take_reservation("me", 210, 20)==true);
	CHECK(sched.take_reservation("me", 270, 5)==false);
	CHECK(sched.take_reservation("me", 240, 20) == true);
	// 10 0  0  0  0  0 -4 0 0 0 4  0 -6 0 5 0 1  0  0  0 -5 0 -5
	// 10 10 10 10 10 10 6 6 6 6 10 10 4 4 9 0 10 10 10 10 5 5 0
}