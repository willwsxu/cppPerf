#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

using namespace std;

class Twitter {
	unordered_map<int, vector<pair<int, int>>> userTweets;  // store tweets per user. Flaws: redundant, unable to keep time sorted
	unordered_map<int, vector<int>> followers;  // map person to people who follow
												//	unordered_map<int, vector<int>> followees;  // map person to people to whom he/she follow

public:
	/** Initialize your data structure here. */
	Twitter() {

	}

	/** Compose a new tweet. */
	void postTweet(int userId, int tweetId) {
		userTweets[userId].emplace_back(userId, tweetId);
		for (auto follow : followers[userId])
			userTweets[follow].emplace_back(userId, tweetId);
	}

	/** Retrieve the 10 most recent tweet ids in the user's news feed. Each item in the news feed must be posted by users who the user followed or by the user herself. Tweets must be ordered from most recent to least recent. */
	vector<int> getNewsFeed(int userId) {
		vector<int> ans;
		ans.reserve(10);
		int i = 0;
		auto &tweets = userTweets[userId];
		for (auto p = rbegin(tweets); p != rend(tweets); p++) {
			ans.push_back(p->second);
			if (++i == 10)
				break;
		}
		return ans;
	}

	/** Follower follows a followee. If the operation is invalid, it should be a no-op. */
	void follow(int followerId, int followeeId) {
		if (followerId == followeeId)   // check invalid follow (error #2)
			return;
		auto &follow = followers[followeeId];
		if (find(begin(follow), end(follow), followerId) != end(follow))  // already followed
			return;
		follow.push_back(followerId);

		// need to add old tweets from followee  (error #1)
		auto &tweets = userTweets[followeeId];
		copy_if(begin(tweets), end(tweets), back_inserter(userTweets[followerId]), [followeeId](auto& p) {return p.first == followeeId; });
	}

	/** Follower unfollows a followee. If the operation is invalid, it should be a no-op. */
	void unfollow(int followerId, int followeeId) {
		if (followerId == followeeId)   // check invalid unfollow (error #3)
			return;
		auto& follow = followers[followeeId];
		auto rem = remove_if(begin(follow), end(follow), [followerId](int f) {return followerId == f; });  // remove follower from followee
		if (rem != follow.end())
			follow.erase(rem);

		// remove tweets originated from followee
		auto &tweets = userTweets[followerId];
		auto rem2 = remove_if(begin(tweets), end(tweets), [followeeId](auto& p) {return followeeId == p.first; });
		if (rem2 != end(tweets))
			tweets.erase(rem2, end(tweets));
	}
};

