#include <memory>
#include <atomic>
using namespace std;

template <typename T>
class SharedPtr {
	struct Ref {
		atomic<int> count;
		Ref(int i):count(i){
		}
	};
	T * naked=nullptr;
	Ref * counter = nullptr;
public:
	SharedPtr() { }
	SharedPtr(nullptr_t ):SharedPtr() {  }  // delegate
	SharedPtr(T* data) : naked (data){
		if (naked)
			counter = new Ref(1);
	}
	~SharedPtr() noexcept
	{
		delete_me();
	}
	SharedPtr(const SharedPtr& other) noexcept {
		if (other.naked) {
			naked = other.naked;
			counter = other.counter;
			counter->count++;
		}
	}
	SharedPtr(SharedPtr&& other) noexcept {
		naked = other.naked;
		counter = other.counter;
		other.naked = nullptr;  // old pointer must now be empty
		other.counter = nullptr;
	}
	SharedPtr& operator=(const SharedPtr& other) noexcept
	{
		if (this != &other && naked != other.naked) {  // not self, same pointer
			delete_me();
			copy(other);
		}
		return *this;
	}
	SharedPtr& operator=(SharedPtr&& other) noexcept
	{
		if (this != &other) {  // not self
			if (naked == other.naked) { //  same pointer
				if (naked)  // make sure pointer is valid
					counter->count--;
			}
			else {
				delete_me();
				naked = other.naked;  // even if other is empty
				counter = other.counter;
			}
			other.naked = nullptr;
			other.counter = nullptr;
		}
		return *this;
	}

	template<typename Y>
	SharedPtr(const SharedPtr<Y>& other) noexcept {  // why this is discarded by compiler
		naked = other.naked;
		counter = other.counter;
		counter->count++;
	}

	T& operator*() const noexcept {
		return *naked;
	}
	T* operator->() const noexcept {
		return naked;
	}
	int use_count() const {
		return counter == nullptr ? 0: counter->count.load();
	}
private:
	void delete_me() {
		if (naked) {
			if (counter->count.load() == 1) {
				delete naked;
				delete counter;
			}
			else
				counter->count--;
			counter = nullptr;
			naked = nullptr;
		}
	}	
	void copy(const SharedPtr& other) noexcept {
		naked = other.naked;
		counter = other.counter;
		if (other.naked) {
			counter->count++;
		}
	}
};

#include "..\catch.hpp"
TEST_CASE("Jump Shared Ptr test", "[JUMP]")
{
	SECTION("test null pointer") {
		SharedPtr<long long> empty;
		CHECK(empty.use_count() == 0);
		SharedPtr<long long> empty2(nullptr);
		CHECK(empty2.use_count() == 0);
		long long *x = nullptr;
		SharedPtr<long long> empty3(x);
		CHECK(empty3.use_count() == 0);
		SharedPtr<long long> empty4=empty3;  // copy
		CHECK(empty4.use_count() == 0);
		SharedPtr<long long> empty5 = move(empty3);  // move
		CHECK(empty5.use_count() == 0);

		shared_ptr<long long> std_empty;
		CHECK(std_empty.use_count() == 0);
		shared_ptr<long long> std_empty2(nullptr);
		CHECK(std_empty2.use_count() == 0);
		shared_ptr<long long> std_empty3(x);  // ownership of nullptr
		CHECK(std_empty3.use_count() == 1);   // std seems wrong
	}

	SECTION("test constructor and destructor") {
		SharedPtr<long long> long_int1(new long long(2));
		CHECK(long_int1.use_count() == 1);

		SharedPtr<long long> long_int2 = long_int1;  // copy
		CHECK(long_int1.use_count() == 2);
		CHECK(long_int2.use_count() == 2);

		SharedPtr<long long> long_int3 = long_int1;  // copy
		CHECK(long_int1.use_count() == long_int2.use_count());
		CHECK(long_int3.use_count() == 3);

		CHECK(*long_int3 == 2);
		*long_int3 = 3;
		CHECK(*long_int2 == 3);

		long_int1.~SharedPtr();  // delete
		CHECK(long_int1.use_count() == 0);
		CHECK(long_int2.use_count() == 2);

		long_int2.~SharedPtr();
		CHECK(long_int2.use_count() == 0);
		CHECK(long_int3.use_count() == 1);

		SharedPtr<long long> long_int4(move(long_int1)); // move empty
		CHECK(long_int4.use_count() == 0);

		SharedPtr<long long> long_int5(move(long_int3));
		CHECK(long_int5.use_count() == 1);  // move long_int3 to long_int5
		CHECK(long_int3.use_count() == 0);

		long_int5.~SharedPtr();
		CHECK(long_int5.use_count() == 0);
	}
	SECTION("test move and copy assignment") {
		SharedPtr<long long> to_int1(new long long(2));
		SharedPtr<long long> to_int2;
		SharedPtr<long long> to_int3;
		to_int1 = to_int1;   // self
		CHECK(to_int1.use_count() == 1);
		to_int3 = to_int2;   // empty
		CHECK(to_int2.use_count() == 0);
		to_int2 = to_int1;
		CHECK(to_int2.use_count() == 2);
		to_int2 = to_int2;               // self
		CHECK(to_int2.use_count() == 2);

		SharedPtr<long long> from_int1(new long long(3));
		to_int3 = from_int1;
		CHECK(from_int1.use_count() == 2);
		CHECK(to_int2.use_count() == 2);

		to_int3 = from_int1;  // same pointer, no op
		CHECK(to_int3.use_count() == 2);
		to_int2 = from_int1;
		CHECK(from_int1.use_count() == 3);
		CHECK(to_int1.use_count() == 1);

		to_int1 = from_int1;  // original pointer in to_int1  will be destroyed
		CHECK(from_int1.use_count() == 4);
		CHECK(to_int1.use_count() == 4);

		to_int1 = move(to_int1);  // self, no op
		CHECK(to_int1.use_count() == 4);

		to_int1 = move(from_int1);
		CHECK(from_int1.use_count() == 0);
		CHECK(to_int1.use_count() == 3);
		to_int1 = move(from_int1);  // from_int1 is null
		CHECK(to_int1.use_count() == 0); // to_int1 become null
		CHECK(to_int2.use_count() == 2);

		shared_ptr<long long> std_from;
		shared_ptr<long long> std_to(new long long(2));
		std_to = move(std_from);
		CHECK(std_to.use_count() == 0);

		SharedPtr<long long> from_int2(new long long(4));
		to_int1 = move(from_int2);
		CHECK(from_int1.use_count() == 0);
		CHECK(to_int1.use_count() == 1);
		CHECK(to_int2.use_count() == 2);
	}

}