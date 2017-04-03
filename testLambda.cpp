
#include <string>
#include <chrono>
#include <iostream>
using namespace std;

class LambdaTest
{
public:
	LambdaTest(){
		state1 = 0;
		state2 = false;
		state3 = "TEST";
	}
	
	inline bool test1() { return state2; }
	bool test2() {
		auto test = [&]()->bool
		{
			return state2;
		};
		return test();
	}

protected:
	int state1;
	bool state2;
	string state3;
};

/*
count nano seconds: 0 count 100000000
count/if nano seconds: 0 count 200000000
count/lambda nano seconds: 0 count 300000000
count/if/test1 nano seconds: 56000000 count 300000000
count/if/test2 nano seconds: 56000000 count 300000000
lambda performance is comparable to straight code
*/
void testLamda()
{
	LambdaTest test;
	int loops = 10000;
	int count=0;
	auto counting = [loops, &count](){
		if (loops>0)
			++count; 
	};
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops; i++) {
		for (int j = 0; j < loops; j++) {
			++count;
		}
	}
	auto end = chrono::high_resolution_clock::now();
	auto nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "count nano seconds: " << nanos.count() << " count " << count << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops; i++) {
		for (int j = 0; j < loops; j++) {
			if (loops>0)
				++count;
		}
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "count/if nano seconds: " << nanos.count() << " count " << count << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops; i++) {
		for (int j = 0; j < loops; j++) {
			counting();
		}
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "count/lambda nano seconds: " << nanos.count() << " count " << count << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops; i++) {
		for (int j = 0; j < loops; j++) {
			if (test.test1())
				++count;
		}
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "count/if/test1 nano seconds: " << nanos.count() << " count " << count << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < loops; i++) {
		for (int j = 0; j < loops; j++) {
			if (test.test2())
				++count;
		}
	}
	end = chrono::high_resolution_clock::now();
	nanos = chrono::duration_cast<chrono::nanoseconds> (end - start);
	cout << "count/if/test2 nano seconds: " << nanos.count() << " count " << count << endl;
}