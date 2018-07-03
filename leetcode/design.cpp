#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

class Twitter {
	vector<pair<int, int>> allTweets;
	unordered_map<int, unordered_set<int>> followers;  // map person to his or her followers
	unordered_map<int, unordered_set<int>> followees;  // map person to people to whom he/she follow

public:
	/** Initialize your data structure here. */
	Twitter() {

	}

	/** Compose a new tweet. */
	void postTweet(int userId, int tweetId) {
		allTweets.emplace_back(userId, tweetId);
	}

	/** Retrieve the 10 most recent tweet ids in the user's news feed. Each item in the news feed must be posted by users who the user followed or by the user herself. Tweets must be ordered from most recent to least recent. */
	vector<int> getNewsFeed(int userId) {
		vector<int> ans;
		ans.reserve(10);
		int i = 0;
		auto& follow = followees[userId];  // cache map look up, big performance saver
		for (auto p = rbegin(allTweets); p != rend(allTweets); p++) {
			if (p->first == userId || follow.count(p->first)) {
				ans.push_back(p->second);
				if (++i == 10)
					break;
			}
		}
		return ans;
	}

	/** Follower follows a followee. If the operation is invalid, it should be a no-op. */
	void follow(int followerId, int followeeId) {
		if (followerId != followeeId) { // check invalid follow (error #2)
			followers[followeeId].insert(followerId);
			followees[followerId].insert(followeeId);
		}
	}

	/** Follower unfollows a followee. If the operation is invalid, it should be a no-op. */
	void unfollow(int followerId, int followeeId) {
		if (followerId != followeeId) {  // check invalid unfollow (error #3)
			followers[followeeId].erase(followerId);

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