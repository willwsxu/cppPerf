#include <cassert>
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "Graph.h"
#include "helper.h"
#include "myalgo.h"
#include <map>
using namespace std;

struct Node {
	Node* next;
	Node* prev;
	int value;
	int key;
	Node(Node* p, Node* n, int k, int val) :prev(p), next(n), key(k), value(val) {};
	Node(int k, int val) :prev(NULL), next(NULL), key(k), value(val) {};
};

class Cache {

protected:
	map<int, Node*> mp; //map the key to the node in the linked list
	int cp;  //capacity
	Node* tail; // double linked list tail pointer
	Node* head; // double linked list head pointer
	virtual void set(int, int) = 0; //set function
	virtual int get(int) = 0; //get function
};

class LRUCache : public Cache {
	void removeNode(Node *node)
	{
		if (node->prev)
			node->prev->next = node->next;
		if (node->next)
			node->next->prev = node->prev;
	}
	void update(int key, int val) {
		auto& new_head = mp[key];
		if (new_head != head) {
			removeNode(new_head);
			if (new_head == tail)
				tail = new_head->prev;
			new_head->next = head;
			new_head->prev = nullptr;
			head = new_head;
		}
		new_head->value = val;
	}
public:
	LRUCache(int c) {
		cp = c;
		head = nullptr;
		tail = nullptr;
	}
	void set(int key, int val) override {
		if (mp.count(key))
			update(key, val);
		else {
			auto old_head = head;
			head = new Node(nullptr, head, key, val);
			if (!tail)
				tail = head;
			else if (old_head)
				old_head->prev = head;
			mp[key] = head;
			if ((int)mp.size()>cp) {
				mp.erase(tail->key);
				auto old = tail;
				tail = old->prev;
				tail->next = nullptr;
				delete old;
			}
		}
	}
	int get(int key) override {
		auto found = mp.find(key);
		if (found == end(mp))
			return -1;
		if (found->second)
			return found->second->value;
		return -1;
	}
};