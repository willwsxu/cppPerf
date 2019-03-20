#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

struct Tweet  // immutable class
{
	Tweet(int id, int s) :tid(id), seq(s) {
	}
	const int tid;
	const int seq;
};

struct TweetIterator
{
	TweetIterator(const vector<Tweet>& vt)
	{
		begin = rbegin(vt);
		end = rend(vt);
	}
	typedef vector<Tweet>::const_reverse_iterator rit;
	rit begin;
	rit end;
};

class Twitter {
	unordered_map<int, vector<Tweet>>		allTweets;
	//unordered_map<int, unordered_set<int>>	followers;  // map person to his or her followers
	unordered_map<int, unordered_set<int>>	followees;  // map person to people to whom he/she follow

public:
	/** Initialize your data structure here. */
	Twitter() {

	}

	/** Compose a new tweet. */
	void postTweet(int userId, int tweetId) {
		static int gid = 0;
		allTweets[userId].emplace_back(tweetId, ++gid);
		followees[userId].insert(userId);  //follow self
	}

	/** Retrieve the 10 most recent tweet ids in the user's news feed. Each item in the news feed must be posted by users who the user followed or by the user herself. Tweets must be ordered from most recent to least recent. */
	vector<int> getNewsFeed(int userId) { // use heap speed up from 73 to 33 ms, beat 99%
		vector<int> ans;
		ans.reserve(10);
		auto& follow = followees[userId];  // cache map look up, big performance saver
		vector<TweetIterator> tweet_heap;  // store latest tweet from each followee into a heap
		for (auto f : follow) {
			auto& tweet = allTweets[f];
			if (!tweet.empty())
				tweet_heap.push_back(TweetIterator(tweet));
		}
		auto comp = [](auto&a, auto&b) {return a.begin->seq < b.begin->seq; };
		make_heap(begin(tweet_heap), end(tweet_heap), comp);
		int i = 0;
		while (!tweet_heap.empty() && i++<10) {
			pop_heap(begin(tweet_heap), end(tweet_heap), comp);
			auto t = tweet_heap.back();
			tweet_heap.pop_back();      // must pop out then push back, why?
			ans.push_back(t.begin->tid);
			if (++t.begin != t.end) {  // check if there is next tweet from this user
				tweet_heap.push_back(t);
				push_heap(begin(tweet_heap), end(tweet_heap), comp);
			}
		}
		return ans;
	}

	/** Follower follows a followee. If the operation is invalid, it should be a no-op. */
	void follow(int followerId, int followeeId) {
		if (followerId != followeeId) { // check invalid follow (error #2)
			followees[followerId].insert(followeeId);
		}
	}

	/** Follower unfollows a followee. If the operation is invalid, it should be a no-op. */
	void unfollow(int followerId, int followeeId) {
		if (followerId != followeeId) {  // check invalid unfollow (error #3)
			// remove followee from follower set
			followees[followerId].erase(followeeId);
		}
	}
};


TEST_CASE("Tweeter design", "[NEW]")
{
	Twitter tw;
	CHECK(tw.getNewsFeed(1) == vector<int>{});
	tw.postTweet(1, 5);
	CHECK(tw.getNewsFeed(1) == vector<int>{5});
	tw.unfollow(1, 2);
	tw.follow(1, 2);
	CHECK(tw.getNewsFeed(2) == vector<int>{});
	tw.postTweet(2, 6);
	CHECK(tw.getNewsFeed(1) == vector<int>{6, 5});
	tw.postTweet(1, 7);
	tw.postTweet(2, 8);
	CHECK(tw.getNewsFeed(1) == vector<int>{8, 7, 6, 5});

	tw.follow(3, 1);
	tw.follow(1, 1);
	tw.follow(3, 1);
	CHECK(tw.getNewsFeed(3) == vector<int>{7, 5});

	tw.unfollow(1, 2);
	CHECK(tw.getNewsFeed(1) == vector<int>{7, 5});
	tw.postTweet(2, 9);
	CHECK(tw.getNewsFeed(1) == vector<int>{7, 5});
	CHECK(tw.getNewsFeed(2) == vector<int>{9, 8, 6});

	tw.follow(1, 2);
	CHECK(tw.getNewsFeed(1) == vector<int>{9, 8, 7, 6, 5});  // maintain relative order of past tweets
}




class Bucket
{
public:
	// 451 Given a string, sort it in decreasing order based on the frequency of characters.
	string frequencySort_old(string s) { // beat 34%
		int count[128] = { 0 };
		for (char c : s)
			count[c]++;
		sort(begin(s), end(s), [&count](char a, char b) {return a != b && (count[a] > count[b] || (count[a] == count[b] && a>b)); });
		return s;
	}
	string frequencySort(string s) { // beat 99% using vector sort, faster than heap 93%
		int count[128] = { 0 };
		for (char c : s)
			count[c]++;
		vector<pair<int, char>> vic;  // count to char pair
		vic.reserve(26);
		for (int i = 0; i < sizeof(count) / sizeof(int); i++) {
			if (count[i]) {
				vic.emplace_back(count[i], (char)i);
			}
		}
		//sort(begin(vic), end(vic), [](auto&a, auto&b) { return a.first > b.first; });
		make_heap(begin(vic), end(vic), [](auto&a, auto&b) { return a.first < b.first; });
		int pos = 0;
		//for (auto& p : vic) {
		while (!vic.empty()) {
			pop_heap(begin(vic), end(vic));
			auto p = vic.back();
			vic.pop_back();
			s.replace(pos, p.first, p.first, p.second);
			pos += p.first;
		}
		return s;
	}

	// 347. Top K Frequent Elements
	vector<int> topKFrequent(vector<int>& nums, int k) { // beat 99%
		unordered_map<int, int> count;
		for (int i : nums)
			count[i]++;
		vector<pair<int, int>> vpii;
		vpii.reserve(count.size());
		move(begin(count), end(count), back_inserter(vpii));
		nth_element(begin(vpii), begin(vpii) + k - 1, end(vpii), [](auto&a, auto&b) { return a.second > b.second; });
		vector<int> ans;
		ans.reserve(k);
		//transform(begin(vpii), begin(vpii) + k, back_inserter(ans), [](auto&p) {return p.first; }); // slower
		for (auto x = begin(vpii); x != begin(vpii) + k; x++)
			ans.push_back(x->first);
		return ans;
	}

	// 692. Top K Frequent Words, sorted by frequency from highest to lowest. 
	// If two words have the same frequency, then the word with the lower alphabetical order comes first.
	vector<string> topKFrequent(vector<string>& words, int k) { // beat 99%
		unordered_map<string, int> count;
		for (auto& w : words)
			count[w]++;
		vector<pair<string, int>> vp;
		vp.reserve(count.size());
		move(begin(count), end(count), back_inserter(vp)); // move elements from map to vector
		partial_sort(begin(vp), begin(vp) + k, end(vp), [](auto&a, auto&b) { return a.second > b.second || (a.second == b.second && a.first<b.first); });

		vector<string> ans;
		ans.reserve(k);
		for (auto x = begin(vp); x != begin(vp) + k; x++)
			ans.push_back(move(x->first));
		return ans;
	}
};

TEST_CASE("frequencySort string", "[FREQ]")
{
	Bucket b;
	CHECK(b.frequencySort("loveleetcode") == "eeeeoollvtdc");
}

class Heap
{
	public:
	// Given two integer arrays nums1 and nums2 sorted in ascending order and an integer k
	// Find the k pairs (u1,v1),(u2,v2) ...(uk,vk) with the smallest sums
	vector<pair<int, int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
		size_t len1 = nums1.size();
		size_t len2 = nums2.size();
		vector<int>& longOne = len1 > len2 ? nums1 : nums2;
		vector<int>& shortOne = len1 <= len2 ? nums1 : nums2;
		using pii = pair<int, int>;
		vector<pii> heap;
		for (int n2 : nums2)  // initialize heap with k pairs, 
			heap.emplace_back(nums1[0], n2);

		auto comp = [](pii&p1, pii&p2) { return p1.first + p1.second > p2.first + p2.second; };
		//if (k >= heap.size())
			return heap;
	}
};
