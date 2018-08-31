#pragma once
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

// testing construction
struct Str
{
	enum CTOR_STATE{STRING_MOVE_CTOR, STRING_CTOR, MOVE_CTOR, COPY_CTOR, MOVE_ASSIGN, COPY_ASSIGN};
	const CTOR_STATE state;
	Str(string&& s) :_s(move(s)), state(STRING_MOVE_CTOR){
		cout << "string move ctor " << _s << this << endl;
	}
	Str(const string& s) :_s(s), state(STRING_CTOR) {
		cout << "string ctor " << _s << this << endl;
	}
	Str(Str&& s) :_s(move(s._s)), state (MOVE_CTOR) {
		cout << "move ctor " << _s << this << endl;
	}
	Str(const Str& s) :_s(s._s), state(COPY_CTOR) {
		cout << "copy ctor " << _s << this << endl;
	}
	~Str() { cout << "~tor <" << _s << ">" << this << endl; }

	Str& operator=(Str&& s) {
		_s = move(s._s);
		cout << "move assignment " << _s << this << endl;
		return *this;
	}
	Str& operator=(const Str& s) {
		_s = s._s;
		cout << "copy assignment " << _s << this << endl;
		return *this;
	}
private:
	string _s;
};

Str make_str(const char *s)
{
	cout << "RVO " << endl;
	return Str(s); // need move constructor to compile, but not actually used due to RVO
}
void copy_elision(Str s)
{
	cout << "pas_by_value / copy_elision " << s.state << endl;
}
struct Str2
{
	Str2(Str s) { cout << "Str2 ctor" << endl; }
};