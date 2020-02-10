
using size_type = size_t;
class SimplifiedStorage
{
public:
    SimplifiedStorage(void* block, size_type size, size_type partition) :m_head(nullptr), m_partition_size(partition)
    {
        m_head = segregate(block, size, m_head);
    }
    void* alloc_n(size_type n_blocks)
    {
        void* start = &m_head;   // pointer to next start block
        void* iter = nullptr;
        do {
            if (!next_of(start))
                return nullptr;
            iter = try_alloc_n(start, n_blocks);
        } while (iter==nullptr);  // did not find n_blocks of continuous memory
        void * ret = next_of(start);  // save the start of block
        next_of(start) = next_of(iter);  // link previous block to next block
        return ret;
    }
    void free_n(void *ptr, size_type n)
    {
        if (n == 0)
            return;
        auto insertion = find_prev(&m_head, ptr);
        next_of(insertion) = segregate(ptr, n* m_partition_size, next_of(insertion));
    }
    bool empty() const
    {
        return m_head == nullptr;
    }
    size_type num_chunks() {
        void* iter = m_head;
        size_type count = 0;
        while (iter) {
            count++;
            iter = next_of(iter);
        }
        return count;
    }
private:
    void*       m_head;
    size_type   m_partition_size;

    // segreagte block into partitions, each partition is linked to its next neighbor
    void* segregate(void* block, size_type size, void* end)
    {
        signed long long blocks = size / m_partition_size;
        char * iter_back = static_cast<char*>(block) + (blocks - 1) * m_partition_size;
        while (blocks-- > 0) {
            next_of(iter_back) = end;
            end = iter_back;
            iter_back -= m_partition_size;
        }
        return end;
    }
    void*& next_of(void* const block)
    {
        return *(static_cast<void**>(block));
    }
    // input pointer to next start
    // return last item of a block, or nullptr
    void* try_alloc_n(void*& start, size_type n_blocks) {
        if (!start)
            return nullptr;
        void* iter = next_of(start);  // first block
        while (--n_blocks > 0) {  // expect at least one more block
            void* next = next_of(iter);
            void* expected = static_cast<char*>(iter) + m_partition_size;
            if (next != expected) {
                start = iter;  // next block is not sequential, reset as next start
                return nullptr;
            }
            iter = next;
        }
        return iter;
    }
    // input: pointer to start node
    void* find_prev(void * start, void * insert)
    {
        void* iter = next_of(start);
        while (iter !=nullptr && iter < insert) {
            start = iter;
            iter = next_of(start);
        }
        return start;
    }
};

#include <thread>
#include <random>
#include <iostream>
bool allocator_concurrency()
{
    auto alloc = std::make_shared<std::allocator<int>>();
    auto n1 = alloc->allocate(1);   // new 4 bytes
    *n1 = 100;
    alloc->deallocate(n1, 1);       // delete 4 bytes
    auto task = [alloc](int id) {
        int count = 0;
        std::random_device rd;
        std::mt19937 g(rd());
        while (count < 1000000) {
            auto elem = std::uniform_int_distribution<>(1, 10)(g);
            std::vector<int> v(elem, *alloc);
            count += elem;
        }
        std::cout << id << " done\n";
    };
    auto t1 = std::thread(task, 1);
    auto t2 = std::thread(task, 2);
    t1.join();
    t2.join();
    return true;
}

#include "..\catch.hpp"
TEST_CASE("allocator thread safety", "[CONCUR]") {
    // std::allocator just check alignment and forward to new and delete, so it is thread safe
    REQUIRE(allocator_concurrency());
}
TEST_CASE("simplified allocate storage tests", "[STORE]") {
    SECTION("constructor test: empty") {
        size_t size = 10;
        void* chunk = ::malloc(size);
        SimplifiedStorage store(chunk, size, 32);
        REQUIRE(store.empty());
        ::free(chunk);
    }
    SECTION("constructor test: size 1") {
        size_t size = 63;
        void* chunk = ::malloc(size);
        SimplifiedStorage store(chunk, size, 32);
        REQUIRE(store.num_chunks()==1);
        ::free(chunk);
    }
    SECTION("constructor test: size 32") {
        size_t size = 1024+16;
        void* chunk = ::malloc(size);
        SimplifiedStorage store(chunk, size, 32);
        REQUIRE(store.num_chunks() == 32);
        ::free(chunk);
    }

    SECTION("alloc test: alloc all in one") {
        size_t size = 1024 + 16;
        void* chunk = ::malloc(size);
        SimplifiedStorage store(chunk, size, 32);
        REQUIRE(store.alloc_n(33) == nullptr);
        REQUIRE(store.alloc_n(32) != nullptr);
        REQUIRE(store.alloc_n(1) == nullptr);
        REQUIRE(store.empty());
        ::free(chunk);
    }
    SECTION("alloc test: many alloc") {
        size_t size = 1024 + 16;
        void* chunk = ::malloc(size);
        SimplifiedStorage store(chunk, size, 32);
        REQUIRE(store.alloc_n(5) != nullptr);
        REQUIRE(store.alloc_n(20) != nullptr);
        REQUIRE(store.alloc_n(7) != nullptr);
        REQUIRE(store.alloc_n(1) == nullptr);
        REQUIRE(store.empty());
        ::free(chunk);
    }
    SECTION("free test: right to left") {
        size_t size = 1024 + 16;
        void* chunk = ::malloc(size);
        SimplifiedStorage store(chunk, size, 32);
        auto a1 = store.alloc_n(5);
        auto a2 = store.alloc_n(20);
        auto a3 = store.alloc_n(7);
        REQUIRE(store.alloc_n(1) == nullptr);
        store.free_n(a3, 7);
        REQUIRE(store.num_chunks()==7);
        store.free_n(a2, 20);
        REQUIRE(store.num_chunks() == 27);
        store.free_n(a1, 5);
        REQUIRE(store.num_chunks() == 32);
        ::free(chunk);
    }
    SECTION("free test: left to right") {
        size_t size = 1024 + 16;
        void* chunk = ::malloc(size);
        SimplifiedStorage store(chunk, size, 32);
        auto a1 = store.alloc_n(5);
        auto a2 = store.alloc_n(20);
        auto a3 = store.alloc_n(7);
        REQUIRE(store.alloc_n(1) == nullptr);
        store.free_n(a1, 5);
        REQUIRE(store.num_chunks() == 5);
        store.free_n(a2, 20);
        REQUIRE(store.num_chunks() == 25);
        store.free_n(a3, 7);
        REQUIRE(store.num_chunks() == 32);
        ::free(chunk);
    }
    SECTION("free test: middle") {
        size_t size = 1024 + 16;
        void* chunk = ::malloc(size);
        SimplifiedStorage store(chunk, size, 32);
        auto a1 = store.alloc_n(5);
        auto a2 = store.alloc_n(20);
        auto a3 = store.alloc_n(7);
        REQUIRE(store.alloc_n(1) == nullptr);
        store.free_n(a1, 5);
        REQUIRE(store.num_chunks() == 5);
        store.free_n(a3, 7);
        REQUIRE(store.num_chunks() == 12);
        store.free_n(a2, 20);
        REQUIRE(store.num_chunks() == 32);
        ::free(chunk);
    }
}