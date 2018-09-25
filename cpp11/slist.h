#pragma once
#include <memory>
#include <atomic>

using std::move;
using std::forward;

template <typename T, bool use_share_ptr=true>
class slist
{
	struct Node {
		T	data;
		using NodeType = typename std::conditional<use_share_ptr, std::shared_ptr<Node>, std::unique_ptr<Node>>::type;
		NodeType	next;
		Node(T&& d) :data(move(d)) {}
		Node(const T& d) :data(d) {}
	};
	typename Node::NodeType	head;

	template<bool share, typename U, typename = std::enable_if_t<share>>
	auto create(U&& t) {
		return std::make_shared<Node>(forward<U>(t));
	}

	template<bool share, typename U, std::enable_if_t<!share, int> = 0>
	auto create(U&& t) {
		return std::make_unique<Node>(forward<U>(t));
	}
public:
	void push_front(const T& t)
	{
		auto n = create<use_share_ptr>(t);
		n->next = move(head);  // move is required for unique_ptr, harmless for shared_ptr
		head = move(n);
	}
	void push_front(T&& t)
	{
		auto n = create<use_share_ptr>(move(t));  // must add move
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
		~Node() { 
			if (next)
				delete next; 
		}
	};
	using NodeType = typename std::conditional<std::is_class_v<head_type>, std::atomic<Node*>, Node *>::type;
	NodeType head;

public:
	slist_r():head(nullptr)
	{}
	~slist_r()
	{
		Node *old = (Node *)head;
		if (old && old->next) {
			delete old->next;
			old->next = nullptr;
		}
		pop_front();
	}
	
	template<typename U, typename=std::enable_if_t<!is_class_v<U>>>  // template signature must be different from next overload func
	void exchange(Node *, Node *n) {
		head = n;
	}

	template<typename U, std::enable_if_t<std::is_class_v<U>, int> = 0>  // must add space > =, must use none void as second type
	void exchange(Node *expected, Node *desired) {
		while (!head.compare_exchange_weak(expected, desired))
		{
		}
	}
	
	void push_front(const T& t)
	{
		auto n =new Node(t);
		n->next = (Node *)head;
		exchange<head_type>(n->next, n);
	}
	void push_front(T&& t)
	{
		auto n = new Node(move(t));
		n->next = (Node *)head;
		exchange<head_type>(n->next, n);
	}
	void pop_front()
	{
		Node *old = (Node *)head;
		if (old!=nullptr) {
			exchange<head_type>(old, old->next);
			old->next = nullptr;
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

/* replaced by the slist class which support both shared_ptr and unique_ptr
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
*/