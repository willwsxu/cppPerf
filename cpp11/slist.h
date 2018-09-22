#pragma once
#include <memory>
#include <atomic>

template <typename T, template<typename> class NodeType>
class slist
{
	struct Node {
		T	data;
		NodeType<Node>	next;
		Node(T&& d) :data(move(d)) {}
		Node(const T& d) :data(d) {}
	};
	NodeType<Node>	head;

	template<template<typename> class Node_t, typename U, 
		typename = std::enable_if_t<std::is_same_v<Node_t<Node>, std::shared_ptr<Node>>>>
	auto create(U&& t) {
		return make_shared<Node>(forward<U>(t));
	}

	template<template<typename> class Node_t, typename U,
		std::enable_if_t<std::is_same_v<Node_t<Node>, std::unique_ptr<Node>>, int> = 0>
		auto create(U&& t) {
		return make_unique<Node>(forward<U>(t));
	}
public:
	void push_front(const T& t)
	{
		auto n = create<NodeType>(t);
		n->next = head;
		head = n;
	}
	void push_front(T&& t)
	{
		auto n = create<NodeType>(t);
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


// raw pointer list, no lock or use atomic
template <typename T, typename head_type>
class slist_r
{
protected:
	struct Node {
		T	data;
		Node*	next;
		Node(T&& d) :data(move(d)), next(nullptr){}
		Node(const T& d) :data(d), next(nullptr) {}
		~Node() { delete next; }
	};
	using NodeType = typename std::conditional<std::is_class_v<head_type>, std::atomic<Node*>, Node *>::type;
	NodeType head;

public:
	slist_r():head(nullptr)
	{}
	~slist_r()
	{
		while (peek())
			pop_front();
	}
	
	template<typename U, typename=std::enable_if_t<!is_class_v<U>>>  // template signature must be different from next overload func
	void exchange(Node *n) {
		head = n;
	}

	template<typename U, std::enable_if_t<std::is_class_v<U>, int> = 0>  // must add space > =, must use none void as second type
	void exchange(Node *n) {
		while (!head.compare_exchange_weak(n->next, n))
		{
		}
	}
	
	void push_front(const T& t)
	{
		auto n =new Node(t);
		n->next = head;
		exchange<head_type>(n);
	}
	void push_front(T&& t)
	{
		auto n = new Node(t);
		n->next = head;
		exchange<head_type>(n);
	}
	void pop_front()
	{
		if ((Node*)head!=nullptr) {
			auto old = (Node *)head;
			exchange<head_type>(old->next);
			delete old;
		}
	}

	const T* peek()
	{
		if ((Node*)head)
			return &((Node*)head)->data;
		return nullptr;
	}
};


template <typename T>
class slist_u
{
	struct Node {
		T	data;
		std::unique_ptr<Node>	next;
		Node(T&& d) :data(move(d)) {}
		Node(const T& d) :data(d) {}
	};
	std::unique_ptr<Node>	head;

public:
	void push_front(const T& t)
	{
		auto n = make_unique<Node>(t);
		n->next = move(head);
		head = move(n);
	}
	void push_front(T&& t)
	{
		auto n = std::make_unique<Node>(t);
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
