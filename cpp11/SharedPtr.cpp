#include "..\catch.hpp"

template <typename T>
class SharedPtr {
	T * naked=nullptr;
	int *counter = nullptr;
public:
	SharedPtr() { counter = new int(0); }
	SharedPtr(nullptr_t ):SharedPtr() {  }  // delegate
	SharedPtr(T* data) : naked (data){
		counter = new int;
		*counter = 1;
	}
	SharedPtr(const SharedPtr& other) noexcept {
		naked = other.naked;
		counter = other.counter;
		(*counter)++;
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
		return *counter;
	}
};

TEST_CASE("Jump Shared Ptr test", "[TEST]")
{
	SharedPtr<long long> empty;
	CHECK(empty.use_count() == 0);
	SharedPtr<long long> empty2(nullptr);
	CHECK(empty2.use_count() == 0);
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