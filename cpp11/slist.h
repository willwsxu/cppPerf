#pragma once
#include <memory>

template <typename T>
class slist
{
	struct Node {
		T	data;
		shared_ptr<Node>	next;
		Node(const T&& d) :data(move(d)) {}
		Node(const T& d) :data(d) {}
	};
	shared_ptr<Node>	head;

public:
	void push_front(const T& t)  // cannot use &&
	{
		auto n = make_shared<Node>(t);
		n->next = head;
		head = n;
	}
	void pop_front()
	{
		if (head) {
			head = head->next;
		}
	}

	const T* peek()
	{
		if (head)
			return &head->data;
		return nullptr;
	}
};