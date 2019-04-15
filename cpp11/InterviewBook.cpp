#include "..\catch.hpp"

#include <forward_list>
#include <map>

using namespace std;

// check if a linked list is palindrome
template<typename FwdIter>
bool is_palindrome(FwdIter first, FwdIter& second, int mid, int idx) {
	if (idx < mid) {
		auto prev = first;  // save last value for back tracking
		bool x = is_palindrome(++first, second, mid, idx + 1);
		return x && *prev == *second++;
	}
	return *first == *second++;
}
bool is_palindrome(forward_list<int> palin)
{
	int size = 0;
	auto slow = begin(palin);
	auto fast = begin(palin);
	while (fast != end(palin)) {
		size++;
		if (++fast == end(palin))  // odd len
			break;
		size++;
		++slow;
		++fast;
	}
	if (size % 2 != 0)
		++slow;
	return is_palindrome(begin(palin), slow, size / 2, 1); // count to mid [1-mid)
}

// 17.7 baby names
class UnionFindSynonyms
{
	map<string, string> parent_node;
	map<string, int> size;

public:
	const string& find(const string& u) {
		auto found = parent_node.find(u);
		if (found != end(parent_node)) {
			return find( found->second );
		}
		return u;
	}
	bool join(const string&u, const string&v) {
		const string& pu = find(u);
		const string& pv = find(v);
		if (pu == pv)
			return false;
	}
};

TEST_CASE("palindrome list", "[INTERVIEW]")
{
	CHECK(is_palindrome(forward_list<int>{ 1,2,3,4,5,4,3,2,1 } )==true);
	CHECK(is_palindrome(forward_list<int>{ 1, 2, 3, 4, 4, 3, 2, 1 }) == true);
	CHECK(is_palindrome(forward_list<int>{ 1,2,3,4,5,4,3,2 }) == false);
}