#pragma once

#include <string>
/*
pass by ref nano seconds :       9156100 count 100000  best perf, but too many overloads
forward reference nano seconds : 9359000 count 100000  very good perf,  least overload
pass by value nano seconds :    15196700 count 100000  OK perf, 2 overloads
*/
using namespace std;
class Cust1
{
	string first;
	string last;
	int id;
public:
	Cust1(string f, string l = "", int i = 0):first(move(f)), last(move(l)),id(i) {}
	Cust1(const char *f) :first(f), last(""), id(0) {}  // e.g Cust1 c="t";
};

// universal/forwarding reference
class Cust
{
	string first;
	string last;
	int id;
public:
	template <typename S1, typename S2=string,  // need default type because type cannot be deduced from default arg, cannot use const char*
		typename=enable_if_t<is_convertible_v<S1, string>>>  // first param must be string or const char*, else match copy constructor, when construct from single param
		Cust(S1&& f, S2&& l = "", int i = 0) : first(forward<S1>(f)), last(forward<S2>(l)), id(i) {}
};

class Cust2
{
	string first;
	string last;
	int id;
public:
	Cust2(const string& f, const string& l, int i = 0):first(f),last(l),id(i) {}  // no default to avoid duplicate match
	Cust2(const string& f, string&& l = "", int i = 0) :first(f), last(move(l)), id(i) {}
	Cust2(const string& f, const char *l, int i = 0) :first(f), last(l), id(i) {}
	Cust2(string&& f, const string& l, int i = 0) :first(move(f)), last(l), id(i) {}
	Cust2(string&& f, string&& l="", int i = 0) :first(move(f)), last(move(l)), id(i) {}
	Cust2(string&& f, const char* l, int i = 0) :first(move(f)), last(l), id(i) {}
	Cust2(const char* f, const string& l, int i = 0) :first(f), last(l), id(i) {}
	Cust2(const char* f, string&& l = "", int i = 0) :first(f), last(move(l)), id(i) {}
	Cust2(const char* f, const char* l, int i = 0) :first(f), last(l), id(i) {}
};

class VIP : public Cust
{
	using Cust::Cust;
};