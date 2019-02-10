#include <vector>

using namespace std;

template<typename T, int N>
class vector_small_op : public vector<T>
{
	char raw[N * sizeof(T)];  //  raw bytes to store T
	bool	use_small = true;
	int		count = 0;

	T *get(int i) {
		return reinterpret_cast<T*>(raw + i * sizeof(T));
	}
	T& (vector_small_op:: * p_back)();
	T& back_small() {
		return *get(count - 1);
	}
	void (vector_small_op::*p_pop_back) ();
	void pop_back_small() {
		get(--count)->~T();  // destruct last object
	}
public:
	vector_small_op() {
		p_back = &vector_small_op::back_small;
		p_pop_back = &vector_small_op::pop_back_small;
	}
	void push_back(const T& d) {
		if (use_small) {
			if (count == N) {  // exceed small storage
				reserve(N+1);
				for (int i = 0; i < N; i++) {
					vector<T>::push_back(move(*get(i)));
				}
				vector<T>::push_back(d);
				use_small = false;
				p_back = &vector<T>::back;
				p_pop_back = &vector<T>::pop_back;
			}
			else
				new (raw + sizeof(T)*count++)T(d);  // use placement new to construct object
		}
		else {
			vector<T>::push_back(d);
		}
	}
	void pop_back()
	{
		(this->*p_pop_back)();
	}
	T& back() {  // optimize out the branching, by using function pointer
		return (this->*p_back)();
	}

	bool small() const { // for testing
		return use_small;
	}
};

struct TEST
{
	const int x;
	TEST(int y) :x(y) {}
};
#include "..\catch.hpp"
TEST_CASE("Jump small vector optimization test", "[TEST]")
{
	vector_small_op<TEST, 2> small;
	small.push_back(TEST(1));
	CHECK(small.back().x == 1);
	small.push_back(TEST(2));
	CHECK(small.back().x == 2);
	small.pop_back();
	CHECK(small.back().x == 1);
	small.pop_back();
	small.push_back(TEST(1));
	small.push_back(TEST(2));
	CHECK(small.back().x == 2);
	CHECK(small.small() == true);

	small.push_back(TEST(3));
	CHECK(small.back().x == 3);
	CHECK(small.small() == false);
	small.pop_back();
	small.pop_back();
	small.pop_back();
	small.push_back(TEST(4));
	CHECK(small.back().x == 4);
	CHECK(small.small() == false);
}