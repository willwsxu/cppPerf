#include <string>
#include <set>
#include <vector>

using namespace std;
class LongestDupSubStr
{
    const static int MOD = 1000000007;
    vector<long long> hash_prime;
    // finding dup string of length L, using hash
    int longestDupSubstring_helper(const string& s, int L)
    {
        set<long long> substr_hash;
        long long window_hash = 0;
        for (int i = 0; i < s.size(); i++) {
            if (i >= L) {  // subtract hash of previous char out of the window
                window_hash -= hash_prime[L - 1] * (s[i - L] - 'a');
                window_hash = (MOD + window_hash) % MOD;
            }
            window_hash = (31 * window_hash + s[i]-'a')%MOD;
            if (i >= L - 1) {
                if (substr_hash.count(window_hash))
                    return i;
                substr_hash.insert(window_hash);
            }
        }
        return -1;
    }
public:
    LongestDupSubStr() {
        const int max_size = 100001;
        hash_prime.reserve(max_size);
        long long hash = 1;
        for (int i = 0; i < max_size; i++) {
            hash_prime.push_back(hash);
            hash = (hash * 31) % MOD;
        }
    }
    string longestDupSubstring(string S) {
        int low = 0, hi = S.size() - 1;
        int dup_pos = -1;
        while (low < hi) {
            int mid = (low + hi) / 2;
            int dup = longestDupSubstring_helper(S, mid);
            if (dup < 0)  // no dup string at length mid
                hi = mid - 1;
            else {
                low = mid;
                dup_pos = dup;
            }
        }
        return dup_pos < 0 ? "" : S.substr(dup_pos-low+1, low);
    }
};

#include <catch.hpp>
TEST_CASE("1044. Longest Duplicate Substring", "[BS]")
{
    CHECK(LongestDupSubStr().longestDupSubstring("banana") == "ana");
}