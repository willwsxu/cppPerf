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
	}

	/** Retrieve the 10 most recent tweet ids in the user's news feed. Each item in the news feed must be posted by users who the user followed or by the user herself. Tweets must be ordered from most recent to least recent. */
	vector<int> getNewsFeed(int userId) { // use heap speed up from 73 to 33 ms, beat 99%
		vector<int> ans;
		ans.reserve(10);
		followees[userId].insert(userId);  //follow self
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
			tweet_heap.pop_back();
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