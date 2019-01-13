#include "..\catch.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <map>
using namespace std;

// find uncovered intervals
using value_type = long long;
// short type def for convenience, also easy to change type, such as from long to int if value is small
using vii = vector<pair<value_type, value_type>>;
vii uncovered_invervals(vii&& intervals)
{
	if (intervals.empty())
		return {};
	sort(begin(intervals), end(intervals));
	vii uncovered;
	auto prev_interval_end = intervals.front().second;
	for (const auto& i : intervals) {
		if (i.first <= prev_interval_end) {  // merge overlapping
			prev_interval_end = max(i.second, prev_interval_end);
		}
		else {
			uncovered.emplace_back(prev_interval_end, i.first);  // detect uncovered interval
			prev_interval_end = i.second;
		}
	}
	return uncovered;
}
// Sort interval by start value, if start of next interval is bigger than end value of previous interval, uncovered is found
// else merge two intervals and only need to remember the end value
// assume the intervals value can be very large, so long long type is used. If we know the value is bounded by int
//   we can change type to int to reduce memory footprint
// c++11 or above std::sort runtime complexity is O(NlogN), loop through all intervals use O(N) 
// overall runtime complexity is O(NlogN+N)=O(NlogN)
void read_stdin()
{
	vii intervals;
	vector<value_type> interval(2, 0);
	for (string line; getline(cin, line); ) {
		istringstream istr(line);
		copy(istream_iterator<value_type>(istr), istream_iterator<value_type>(), begin(interval));
		intervals.emplace_back(interval[0], interval[1]);
	}
	auto uncovered = uncovered_invervals(move(intervals));
	for (const auto& i : uncovered)
		cout << i.first << " " << i.second << "\n";
}
TEST_CASE("four square interview Jan 12", "[NEW]")
{
	CHECK(uncovered_invervals(vii{ { 2, 6 } }) == vii{});
	CHECK(uncovered_invervals(vii{ {2, 6}, { 9,12 }, { 8,9 }, { 18,21 }, {4,7}, {10,11} }) == vii{ {7,8},{12,18} });
	CHECK(uncovered_invervals(vii{ { 2, 60 },{ 9,12 },{ 8,9 },{ 18,21 },{ 4,7 },{ 10,11 } }) == vii{ });
	CHECK(uncovered_invervals(vii{ { 2, 60 },{ 900,1200 },{ 8,9 },{ 18,21 },{ 4,7 },{ 10,11 } }) == vii{ {60,900} });

	//read_stdin();
}

struct Component
{
	string endPoint;
	map<string, Component> next;  // mapping child component
};

// string tokenizer
vector<string> get_tokens(string&& str, char sep=' ') {
	vector<string> tokens;
	istringstream strm(move(str));
	for (string token; getline(strm, token, sep); ) {
		tokens.push_back(move(token));
	}
	return tokens;
}

class EndPointDispatch
{
	Component	root;
	// recursively create child node
	void put(Component& node, vector<string>&& patterns, int idx, string&& endpoint)
	{
		if (idx == patterns.size()) {
			node.endPoint = endpoint;
			return;
		}
		put(node.next[move(patterns[idx])], move(patterns), idx + 1, move(endpoint));
	}
	// recursively look up component node
	string find(Component& node, const vector<string>& requestPath, int idx) {
		if (idx == requestPath.size())
			return node.endPoint;
		auto next = node.next.find(requestPath[idx]);
		string result;
		if ( next != end(node.next) ) {  // first try to match component exactly
			result= find(next->second, requestPath, idx + 1);
		}
		if (result.empty()) { // no exact match
			next = node.next.find("X");  // wildcard search
			if (next != end(node.next))
				return find(next->second, requestPath, idx + 1);
		}
		return result;
	}
public:
	void add_config(string&& config) {
		vector<string> tokens = get_tokens(move(config));
		vector<string> patterns= get_tokens(move(tokens[0]), '/');
		put(root, move(patterns), 0, move(tokens[1]));
	}
	string match(string&& request) {
		vector<string> requestPath= get_tokens(move(request), '/');
		auto result = find(root, requestPath, 0);
		if (result.empty())
			return "404";
		return result;
	}
};

// assume number of components in request path are relatively small
//   comparing to total requests
// assume wildcard matches one whole component, not partial, not more than one
// if at each level there is 1000 different components, 
// using trie to store endpoint by component patterns
// from root, each component is child node of previous component, store end point at leaf child
// wildcard component is created with component name X
// to find, from root, and follow next pointer to find the leaf component child.
// at each level, if component is not found or result is empty, search for wildcard
// runtime analysis:
// N=number of endpoints, K=number of components in request path, M=total requests
// because we potentially have to search both exact and wildcard match at each component level, 
// it takes O(logK) to find child in map
// so dispatch find routine complexity is O(LogK * 2^K)
// It is only advantageuous to old regex match if K is much smaller than N
// and there is very few wildcard along the search path
// if that is the case, complexity can be close to O(KlogK)
// overall program complexity is O(MKlogK), since logK is small, it is roughly O(MK)
// space complexity:
// It has at most N leaf trie nodes, which implies O(N) total trie nodes
// each node has next pointer, with size P=unique component count,
// say len of component name is L,
// final space complexity for adding trie is O(N * P * *L)
// if N=1000, P=10, L=20, total memory added is 200K, would fit in L2 cache
// It should be much faster than the old regex matching
void read_stdin2()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	EndPointDispatch dispatch;
	bool bConfig = true;
	for (string line; getline(cin, line); ) {
		if (line.empty())
			continue;
		if (line.front() == '#') {
			bConfig = false;
			continue;
		}
		if (bConfig)
			dispatch.add_config(move(line));
		else {
			cout << dispatch.match(move(line)) << "\n";
		}
	}
}

// assume input is case sensitive, X is wildcard for one component, all input start with / as root
// assume each component is separated by /, space separate request pattern and endpoint method.
// assume there is no other space
TEST_CASE("four square interview 2 Jan 12", "[NEW]")
{
	EndPointDispatch dispatch;
	dispatch.add_config("/user userRootEndpoint");
	dispatch.add_config("/user/friends userFriendsEndpoint");
	dispatch.add_config("/user/lists userListsEndpoint");
	dispatch.add_config("/user/X userEndpoint");
	dispatch.add_config("/user/X/friends userFriendsEndpoint");
	dispatch.add_config("/user/X/lists userListsEndpoint");
	dispatch.add_config("/user/X/lists/X userListsIdEndpoint");
	dispatch.add_config("/X/X/lists/X ListsIdEndpoint");
	dispatch.add_config("/ RootEndpoint");
	dispatch.add_config("/X/friends userFriendsEndpoint");
	dispatch.add_config("/X/lists userListsEndpoint");
	dispatch.add_config("/settings settingsEndpoint");
	CHECK(dispatch.match("/user/123") == "userEndpoint");
	CHECK(dispatch.match("/") == "RootEndpoint");
	CHECK(dispatch.match("/user") == "userRootEndpoint");
	CHECK(dispatch.match("/user/friends") == "userFriendsEndpoint");
	CHECK(dispatch.match("/user/123/friends") == "userFriendsEndpoint");
	CHECK(dispatch.match("/user/123/friends/zzz") == "404");
	CHECK(dispatch.match("/user/friends/friends") == "userFriendsEndpoint");
	CHECK(dispatch.match("/user/friends/lists/friends") == "userListsIdEndpoint");
	CHECK(dispatch.match("/settings") == "settingsEndpoint");
	CHECK(dispatch.match("/settings/friends/lists/123") == "ListsIdEndpoint");
	CHECK(dispatch.match("/abc/lists") == "userListsEndpoint");
	CHECK(dispatch.match("/abc/def") == "404");
	CHECK(dispatch.match("/abc/def/lists") == "404");

	CHECK(dispatch.match("/abc/def/lists/123") == "ListsIdEndpoint");

	dispatch.add_config("/X/X/X/X/default defaultEndpoint");
	CHECK(dispatch.match("/user/friends/lists/friends/default") == "defaultEndpoint");
	CHECK(dispatch.match("/abc/def/lists/123/default") == "defaultEndpoint");
	CHECK(dispatch.match("/user/friends/lists/friends/X") == "404");
	CHECK(dispatch.match("/user/friends/friends/friends") == "404");
	CHECK(dispatch.match("/user/friends/friends") == "userFriendsEndpoint");
}