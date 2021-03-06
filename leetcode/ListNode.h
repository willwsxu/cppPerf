#pragma once

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

	static ListNode *forward(ListNode* head, int n) {
		while (n-- && head) {
			head = head->next;
		}
		return head;
	}
	static pair<ListNode *, ListNode*> reverse(ListNode* head) {
		if (!head || !head->next)  // 0 or 1 node
			return{ head, head };
		auto p = reverse(head->next);
		p.second->next = head;
		head->next = nullptr;
		return{ p.first, head };
	}
	static auto reverseHelper(ListNode* head)  // same as reverse above
	{
		if (head == nullptr || head->next == nullptr)
			return std::make_tuple(head, head);
		auto trailer = reverseHelper(head->next); // recursively compute rest of the list
		std::get<1>(trailer)->next = head;		  // append current head after last node
		head->next = nullptr;                     // head become tail
		return std::make_tuple(std::get<0>(trailer), head);  // return both head and tail of list
	}

	
	static void deleteCurrentNode(ListNode* node) {  // delete current node. take next node value, delete next node
		if (node->next) {
			node->val = node->next->val;
			ListNode *next = node->next;
			node->next = node->next->next;
			next->next = nullptr;
			delete next;
		}
	}
};
