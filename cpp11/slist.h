#pragma once
#include <memory>
#include <atomic>

template <typename T>
class slist
{
	struct Node {
		T	data;
		shared_ptr<Node>	next;
		Node(const T&& d) :data(move(d)) {}
		Node(const T& d) :data(move(d)) {}
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


template <bool atom, typename T>
class slist_r
{
	struct Node {
		T	data;
		Node*	next;
		Node(const T&& d) :data(move(d)), next(nullptr){}
		Node(const T& d) :data(move(d)), next(nullptr) {}
		~Node() { delete next; }
	};
	using NodeType = typename conditional<atom, std::atomic<Node*>, Node *>::type;
	NodeType head;

public:
	slist_r():head(nullptr)
	{}
	~slist_r()
	{
		delete head;
	}
	void push_front(const T& t)  // cannot use &&
	{
		auto n =new Node(t);
		n->next = head;
		head = n;
	}
	void pop_front()
	{
		if (head) {
			auto old = head;
			head = head->next;
			delete old;
		}
	}

	const T* peek()
	{
		if (head)
			return &head->data;
		return nullptr;
	}
};


template <typename T>
class slist_u
{
	struct Node {
		T	data;
		unique_ptr<Node>	next;
		Node(const T&& d) :data(move(d)) {}
		Node(const T& d) :data(move(d)) {}
	};
	unique_ptr<Node>	head;

public:
	void push_front(const T& t)  // cannot use &&
	{
		auto n = make_unique<Node>(t);
		n->next = move(head);
		head = move(n);
	}
	void pop_front()
	{
		if (head) {
			head = move(head->next);
		}
	}

	const T* peek()
	{
		if (head)
			return &head->data;
		return nullptr;
	}
};