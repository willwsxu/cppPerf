#include <vector>
using namespace std;

double gaussion_distribution(double point)
{
	if (point<=1 && point>=-1)  // simplified, 
		return 1;
	return 0;
}

// frequency measuring on even buckets, histogram
// or we can assume event occur at point a, follows gaussion distribution
// given points where event happended, i.e. center of the Gaussion distribution, find the frequency at a point
double frequency(vector<double> events, double point) {  // O(n)
	double total = 0;
	for (double e : events) {
		total += gaussion_distribution(point - e);
	}
	return total;
}


#include "..\catch.hpp"
TEST_CASE("Jump frequency measuring test", "[TEST]")
{
	CHECK(frequency(vector<double>{3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 5.1, 5.2, 5.3, 5.4, 5.5, 5.6}, 4.5) == 11);
}
// what is the best data structure to insert new events?
// A: vector, as ordering is not required, new point can be added to the end

// What if we only care value of Gaussian in range Z=+-10
// that means we should sort the events, best data structure is set
// find point-10 and point+10 using set lower_bound method, then do similar frequency sum as above
// run time dependend on events [point-10, point+10]