#include "..\catch.hpp"

#include <memory>
using namespace std;

template <typename T>
class SharedPtr {
	T * naked=nullptr;
	int *counter = nullptr;
public:
	SharedPtr() { }
	SharedPtr(nullptr_t ):SharedPtr() {  }  // delegate
	SharedPtr(T* data) : naked (data){
		if (naked)
			counter = new int(1);
	}
	SharedPtr(const SharedPtr& other) noexcept {
		naked = other.naked;
		counter = other.counter;
		(*counter)++;
	}
	SharedPtr(SharedPtr&& other) noexcept {
		naked = other.naked;
		counter = other.counter;
		other.naked = nullptr;  // old pointer must now be empty
		other.counter = nullptr;
	}
	template<typename Y>
	SharedPtr(const SharedPtr<Y>& other) noexcept {  // why this is discarded by compiler
		naked = other.naked;
		counter = other.counter;
		(*counter)++;
	}
	T& operator*() const noexcept {
		return *naked;
	}
	T* operator->() const noexcept {
		return naked;
	}
	int use_count() const {
		return counter == nullptr ? 0: *counter;
	}
};

TEST_CASE("Jump Shared Ptr test", "[TEST]")
{
	SECTION("test null pointer") {
		SharedPtr<long long> empty;
		CHECK(empty.use_count() == 0);
		SharedPtr<long long> empty2(nullptr);
		CHECK(empty2.use_count() == 0);
		long long *x = nullptr;
		SharedPtr<long long> empty3(x);
		CHECK(empty3.use_count() == 0);

		shared_ptr<long long> std_empty;
		CHECK(std_empty.use_count() == 0);
		shared_ptr<long long> std_empty2(nullptr);
		CHECK(std_empty2.use_count() == 0);
		shared_ptr<long long> std_empty3(x);  // ownership of nullptr
		CHECK(std_empty3.use_count() == 1);   // std seems wrong
	}
	SharedPtr<long long> long_int1(new long long(2));
	CHECK(long_int1.use_count() == 1);

	SharedPtr<long long> long_int2 = long_int1;
	CHECK(long_int1.use_count() == 2);
	CHECK(long_int2.use_count() == 2);

	SharedPtr<long long> long_int3 = long_int1;
	CHECK(long_int1.use_count() == long_int2.use_count());
	CHECK(long_int3.use_count() == 3);

	CHECK(*long_int3 == 2);
	*long_int3 = 3;
	CHECK(*long_int2 == 3);

}