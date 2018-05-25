#pragma once

#include <string>
#include <iostream>

class Schedule
{
	friend bool operator==(const Schedule& s1, const Schedule& s2);
	friend std::ostream& operator<<(std::ostream&os, const Schedule& s2);
	friend bool operator<(const Schedule& s1, const Schedule& s2);
	enum class DayOfWeek
	{
		SUN, MON, TUE, WED, THU, FRI, SAT
	};
	static DayOfWeek convert(char d)
	{
		switch (d) {
		case 'M':		return DayOfWeek::MON;
		case 'T':		return DayOfWeek::TUE;
		case 'W':		return DayOfWeek::WED;
		case 'R':		return DayOfWeek::THU;
		case 'F':		return DayOfWeek::FRI;
		case 'S':		return DayOfWeek::SAT;
		default:
		case 'U':		return DayOfWeek::SUN;
		}

	}
private:
	std::string course;
	long start;
	long end;
	DayOfWeek	day;
	char		cDay;

public:
	Schedule() = default;
	Schedule(const std::string&&c, long s, long e) : course(c), start(s), end(e) {}
	Schedule(const char *argv[]) : course(argv[0]), cDay(*argv[1]), start(atoi(argv[2])), end(atoi(argv[3]))
	{
		day = convert(cDay);
	}

	Schedule& operator=(const Schedule& other) { // not used
		course = other.course;
		day = other.day;
		cDay = other.cDay;
		start = other.start;
		end = other.end;
		return *this;
	}
	Schedule& operator=(const Schedule&& other) {
		course = std::move(other.course);
		day = other.day;
		cDay = other.cDay;
		start = other.start;
		end = other.end;
		return *this;
	}
	Schedule(const Schedule& other) {  // not used
		*this = other;
	}
	Schedule(const Schedule&& other) {
		*this = std::move(other);
	}
};

bool operator==(const Schedule& s1, const Schedule& s2)  // used by adjacent_find
{
	if (s1.day != s2.day)
		return false;
	if (s1.start >= s2.end)
		return false;
	if (s2.start >= s1.end)
		return false;
	return true;
}

bool operator<(const Schedule& s1, const Schedule& s2)  // used by sort
{
	if (s1.day < s2.day)
		return true;
	if (s1.day == s2.day && s1.start < s2.start)
		return true;
	if (s1.day == s2.day && s1.start == s2.start && s1.end < s2.end)
		return true;
	return false;
}

std::ostream& operator<<(std::ostream&os, const Schedule& s2)
{
	os << s2.course << "," << s2.cDay << "," << s2.start << "-" << s2.end;
	return os;
}

template <typename T>
class cstring_iterator : public iterator<forward_iterator_tag, T>  // T is the underlying value type
{
	const char **str;
	int count;
	int increment;
	const char **end;
public:
	cstring_iterator(int c, const char *argv[], int inc) :str(argv), count(c), increment(inc), end(argv + c) {	}
	cstring_iterator() = default;// used as end()
	bool operator!=(const cstring_iterator&) {  // clever trick, check if it is end, from left hand only
		return str != end;
	}
	T operator*() { return T(str); }  // clever trick, construct T from char *
	void operator++() { str += increment; }
};