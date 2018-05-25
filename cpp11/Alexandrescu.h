#pragma once
template <typename T, typename U>
class Conversion
{
	typedef char Small;
	//typedef char Big[2];  // won't work
	class Big { char dummy[2]; };
	static Small convert(U u);
	static Big convert(...);
	static T makeT();

public:
	enum { result = sizeof(convert(makeT())) == sizeof(Small) };  // can only call static methods
};
