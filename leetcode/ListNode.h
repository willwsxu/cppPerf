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
	ListNode *forward(ListNode* head, int n) {
		while (n-- && head) {
			head = head->next;
		}
		return head;
	}
	pair<ListNode *, ListNode*> reverse(ListNode* head) {
		if (!head || !head->next)  // 0 or 1 node
			return{ head, head };
		auto p = reverse(head->next);
		p.second->next = head;
		head->next = nullptr;
		return{ p.first, head };
	}
};
