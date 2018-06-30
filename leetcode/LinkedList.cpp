﻿#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

struct ListNode
{
	int val;
	ListNode *next;
	ListNode(int x) : val(x), next(nullptr) {}

	static ListNode * createList(const vector<int>& nodes)
	{
		ListNode *head = new ListNode(nodes[0]);
		ListNode *cur = head;
		for (int i = 1; i < (int)nodes.size(); i++) {
			cur->next = new ListNode(nodes[i]);
			cur = cur->next;
		}
		return head;
	}
	static int count(ListNode* root) {
		int count = 0;
		while (root != nullptr) {
			count++;
			root = root->next;
		}
		return count;
	}
};
class LinkedList
{
public:
	// G is subset of node values of the list
	// node value range [0, n-1], if there are n nodes. 1<=n<=10000
	int numComponents(ListNode *head, vector<int>&G)
	{
		unordered_set<int> g(begin(G), end(G)); // faster than set, 36ms vs 46ms
		int components = 0;
		bool breaking = true;
		while (head != nullptr)
		{
			if (g.count(head->val)) {
				if (breaking) {
					breaking = false;
					components++;
				}
			}
			else {
				breaking = true;
			}
			head = head->next;
		}
		return components;
	}

	void deleteNode(ListNode* node) {  // delete current node. take next node value, delete next node
		if (node->next) {
			node->val = node->next->val;
			node->next = node->next->next;
		}
	}
	// split into k parts, k can be larger than list size, size difference<=1
	vector<ListNode*> splitListToParts(ListNode* root, int k) {
		vector<ListNode*> ans(k, nullptr);
		int size = ListNode::count(root);
		int mult = size / k;
		int rem = size%k;
		for (int i = 0; i < k && root != nullptr; i++) {
			ans[i] = root;
			int loop = mult;
			if (rem > 0) {  // add a node as early as possible
				loop++;
				rem--;
			}
			ListNode * prev = root;
			for (int j = 0; j < loop&& root != nullptr; j++) {
				prev = root;
				root = root->next;
			}
			prev->next = nullptr;
		}
		return ans;
	}

	int addTwoNumbers(ListNode* l1, ListNode* l2, int diff) {  // l2 is shorter
		if (l2 == nullptr)
			return 0;
		int carry = 0;
		if (diff > 0)
			carry = addTwoNumbers(l1->next, l2, --diff);
		else {
			carry = addTwoNumbers(l1->next, l2->next, --diff);
			carry += l2->val;
		}
		carry += l1->val;
		l1->val = carry % 10;
		return carry / 10;
	}
	// 445. Add Two Numbers II, from most significant digit to least 
	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) { // pass without test, beat 74%
		auto c1 = ListNode::count(l1);
		auto c2 = ListNode::count(l2);
		int carry = 0;
		ListNode *ans = l1;
		if (c1 > c2)
			carry = addTwoNumbers(l1, l2, c1 - c2);
		else {
			carry = addTwoNumbers(l2, l1, c2 - c1);
			ans = l2;
		}
		if (carry > 0) {
			ListNode *head = new ListNode(carry);
			head->next = ans;
			return head;
		}
		return ans;
	}

	// first node is odd, etc. group all odd nodes together followed by the even nodes
	ListNode* oddEvenList(ListNode* head) {  // beat 99.7%
		if (head == nullptr)
			return nullptr;
		ListNode *evenHead = head->next;
		ListNode *prevOdd = head;
		ListNode *prevEven = evenHead;
		while (prevEven && prevEven->next) { // more odd node
			prevOdd->next = prevEven->next;
			prevOdd = prevEven->next;
			prevEven->next = prevOdd->next; // even node
			prevEven = prevOdd->next;
		}
		prevOdd->next = evenHead;
		return head;
	}

	ListNode* swapPairs(ListNode* head) {  // recursive, beat 100%
		if (head == nullptr)
			return nullptr;
		auto second = head->next;
		if (second == nullptr)
			return head;
		head->next = swapPairs(second->next);  // connect to next pair
		second->next = head;		// swap second and head
		return second;
	}

	// 206. Reverse Linked List
	auto reverseHelper(ListNode* head)
	{
		if (head == nullptr || head->next == nullptr)
			return std::make_tuple(head, head);
		auto trailer = reverseHelper(head->next); // recursively compute rest of the list
		std::get<1>(trailer)->next = head;		  // append current head after last node
		head->next = nullptr;                     // head become tail
		return std::make_tuple(std::get<0>(trailer), head);  // return both head and tail of list
	}

	ListNode* reverseList(ListNode* head) {//beat 98%, recursion
		auto ans = reverseHelper(head);
		return std::get<0>(ans);
	}
	// 92. Reverse Linked List II	
	auto reverseHelper2(ListNode* head, int n)  // ->[head, tail, rest after nth node]
	{
		if (head == nullptr)
			return std::make_tuple(head, head, head);
		if (n == 1)
			return std::make_tuple(head, head, head->next);  // find nth node, 3rd tuple is the rest 
		auto trailer = reverseHelper2(head->next, --n); // recursively compute rest of the list
		std::get<1>(trailer)->next = head;		  // append current head after last node
		head->next = nullptr;                     // head become tail
		return std::make_tuple(std::get<0>(trailer), head, std::get<2>(trailer));  // return both head and tail of list
	}
	// reverse nodes from position m to n, inclusive, 1 based
	ListNode* reverseBetween(ListNode* head, int m, int n) {// beat 67%
		if (m == 1) {
			auto trailer = reverseHelper2(head, n);
			std::get<1>(trailer)->next = std::get<2>(trailer);  // connect nth node and the rest
			return std::get<0>(trailer);
		}
		int len = n - m + 1;
		ListNode* tail = head;
		while (head && m-- > 2) {
			tail = tail->next;  // tail is (m-1)th node
		}
		auto trailer = reverseHelper2(tail->next, len);
		std::get<1>(trailer)->next = std::get<2>(trailer);
		tail->next = std::get<0>(trailer);
		return head;
	}

	// Given a linked list and a value x, partition it such that all nodes less than x come before nodes greater than or equal to x.
	ListNode* partition(ListNode* head, int x) {  // beat 87%
		tuple<ListNode*, ListNode*> less{ nullptr, nullptr };
		tuple<ListNode*, ListNode*> more{ nullptr, nullptr };
		auto add = [](auto& t, ListNode *n) {
			auto& tail = get<1>(t);  // must use reference
			if (tail) {
				tail->next = n;
				tail = n;
			}
			else
			{
				get<0>(t) = n;
				get<1>(t) = n;
			}
		};
		while (head) {
			if (head->val < x)
				add(less, head);
			else
				add(more, head);
			head = head->next;
		}
		auto t2 = get<1>(more);
		if (t2)
			t2->next = nullptr; // terminate list on last node
		auto tail = get<1>(less);
		if (tail == nullptr)
			return get<0>(more);
		tail->next = get<0>(more); // link less part to more part
		return get<0>(less);
	}
	// 83. Remove Duplicates from Sorted List
	ListNode* deleteDuplicates(ListNode* head) {// beat 63%
		ListNode * cur = head;
		while (cur) {
			auto next = cur->next;
			while (next && next->val == cur->val)
				next = next->next;
			cur->next = next;
			cur = next;
		}
		return head;
	}

	ListNode* deleteDuplicates3(ListNode* head) { // recursive, beat 94%
		ListNode * cur = head;
		if (!cur || !cur->next)
			return head;

		if (cur->next->val != head->val)  // no duplicate, connect to next node
		{
			head->next = deleteDuplicates3(head->next);
			return head;
		}
		else { // remove all duplciate nodes, try gain
			while ((cur = cur->next) && cur->val == head->val)
				;
			return deleteDuplicates3(cur);
		}
	}
	// Given a sorted linked list, delete all nodes that have duplicate numbers, leaving only distinct numbers from the original list.
	ListNode* deleteDuplicates2(ListNode* head) { // iterative, beat 7%
		ListNode * cur = head;
		ListNode * prev = head;
		while (cur) {
			auto next = cur->next;
			bool dup = false;
			while (next && next->val == cur->val) {
				dup = true;
				next = next->next;
			}
			if (!dup) {  // previous is not duplicated, add next
				cur->next = next;
				prev = cur;
			}
			else if (prev == cur) {  // first node is duplicated, move to next
				prev = next;
				head = next;
			}
			else {
				prev->next = next;
			}
			cur = next;
		}
		return head;
	}
	// 143. Reorder List, L1..Ln ->L1LnL2Ln-1...
	void reorderList(ListNode* head) {
		if (!head || !head->next)
			return;// no more than one node
		ListNode *fast = head;
		ListNode *slow = head; // slow point to middle
		ListNode *tail = head; // tail of first half
		int count = 0; // count = (N+1)/2;
					   // find head of second half, count of first half, and tail of first half
		while (fast) {
			++count;
			fast = fast->next; // fast move 2 steps
			if (fast)
				fast = fast->next;
			tail = slow;
			slow = slow->next;
		}
		tail->next = nullptr;  // terminate first half
		slow = reverseList(slow); // reverse second half
		fast = head; // reset to head of first half of list
		for (int i = 0; i < count && slow; i++) { // alternate node betweem first and second half
			auto temp = slow->next;
			slow->next = fast->next;  // connect Ln to L2
			fast->next = slow;			// connect L1 to Ln
			fast = slow->next;		// move both pointer
			slow = temp;  // break out when second half has no more node
		}
	}

	// 61 Given a linked list, rotate the list to the right by k places, where k is non-negative
	ListNode* rotateRight(ListNode* head, int k) { // beat 62%
		if (!head)
			return head;
		int count = 1;  // count 1st one
		ListNode * tail = head;
		while (tail->next) {  // chck if next exists
			count++;
			tail = tail->next;
		}
		k = k%count;
		if (k == 0)
			return head;
		tail->next = head;  // connect last node to first node
		k = count - k; // if k==1, count-1th node will be new head
		while (--k > 0) {  // shift to right count-k-1 times
			head = head->next;
		}
		tail = head;  // rotate to tail
		head = head->next;
		tail->next = nullptr;
		return head;
	}

	ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {  // beat 16%, easy
		if (!l1)
			return l2;
		if (!l2)
			return l1;
		if (l1->val <= l2->val) {
			l1->next = mergeTwoLists(l1->next, l2);
			return l1;
		}
		l2->next = mergeTwoLists(l1, l2->next);
		return l2;
	}

	ListNode* mergeTwoLists2(ListNode* l1, ListNode* l2) { // beat 61%
		ListNode dummy(INT_MIN);
		ListNode *cur = &dummy;
		while (l1 && l2) {
			if (l1->val < l2->val) {
				cur->next = l1;
				l1 = l1->next;
			}
			else {
				cur = l2;
				l2 = l2->next;
			}
			cur = cur->next;
		}
		cur->next = l1 ? l1 : l2;
		return dummy.next;
	}

	bool hasCycle(ListNode *head) { // easy, beat 98%
		ListNode *fast = head;
		while (head && fast) {
			head = head->next;
			fast = fast->next;
			if (fast) {
				fast = fast->next;  // fast move two steps
				if (fast == head)   // fast is caught up by slow, in cycle
					return true;
			}
		}
		return false;
	}
};


TEST_CASE("Connected Components", "[LinkedListCC]")
{
	LinkedList t;
	ListNode *head1 = ListNode::createList(vector<int>{0, 1, 2, 3});
	ListNode *head2 = ListNode::createList(vector<int>{0, 1, 2, 3, 4});
	SECTION("normal case") {
		CHECK(t.numComponents(head1, vector<int>{0, 1, 3}) == 2);
		REQUIRE(t.numComponents(head2, vector<int>{0, 1, 3, 4}) == 2);
	}
	SECTION("normal case") {
		CHECK(t.numComponents(head1, vector<int>{3}) == 1);
		REQUIRE(t.numComponents(head1, vector<int>{0}) == 1);
	}
}

TEST_CASE("Linked List Partition", "[PART]")
{
	auto head = ListNode::createList(vector<int>{1, 4, 3, 2, 5, 2});
	LinkedList t;
	auto ans = t.partition(head, 3);
	int c = ListNode::count(ans);

	REQUIRE(c == 6);
}

TEST_CASE("Linked List Rotate Right", "[ROTATE]")
{
	auto head = ListNode::createList(vector<int>{1, 2, 3, 4, 5});
	LinkedList t;
	auto ans = t.rotateRight(head, 2);
	CHECK(ans->val == 4);
}
