#include <string>
#include <unordered_set>
#include <vector>
#include <iostream>

using namespace std;
class LongestDupSubStr
{
    const static int MOD = 1000000007;
    vector<long long> pow31;
    vector<long long> poly_sum;  // pre-compute polynomial sum, e.g. 26 chars: a*31^25+b*31^24+...+y*31+z
    // finding dup string of length L, using hash
    int longestDupSubstring_helper(const string& s, int L)
    {
        if (L == 0)
            return -1;
        unordered_set<long> substr_hash;
        substr_hash.reserve((s.size() - L) * 2);
        for (int right = L-1; right < s.size(); right++) {  // string s window [left, right] 
            int left_char = right - L + 1;  // correspond to poly_sum window [left+1, right+1]
            long long left_sum = poly_sum[left_char]*pow31[L]%MOD;
            long long window_hash = (poly_sum[right+1]- left_sum+MOD)%MOD;
            if (substr_hash.count(window_hash))
                return right;
            substr_hash.insert(window_hash);
        }
        return -1;
    }
public:
    LongestDupSubStr() {
        const int max_size = 100001;
        pow31.reserve(max_size);
        long long hash = 1;
        for (int i = 0; i < max_size; i++) {
            pow31.push_back(hash);
            hash = (hash * 31) % MOD;
        }
    }
    string longestDupSubstring(string S) {
        poly_sum.resize(S.size() + 1);
        for (int i = 0; i < S.size(); i++) {
            poly_sum[i + 1] = (poly_sum[i] * 31 + S[i] - 'a') % MOD;
        }
        int low = 0, hi = S.size() - 1;
        int dup_pos = -1;
        int count = 0;
        while (low < hi) {
            int mid = (low + hi) / 2;
            int dup = longestDupSubstring_helper(S, mid);
            if (dup < 0)  // no dup string at length mid
                hi = mid - 1;
            else {
                low = mid;
                dup_pos = dup;
            }
            if (++count == 100) {
                cout << "lo=" << low << " hi " << hi << endl;
            }
        }
        return dup_pos < 0 ? "" : S.substr(dup_pos-low+1, low);
    }
};

#include <catch.hpp>
TEST_CASE("1044. Longest Duplicate Substring", "[BS]")
{
    CHECK(LongestDupSubStr().longestDupSubstring("banana") == "ana");
    string long_str = "moplvidmaagmsiyyrkchbyhivlqwqsjcgtumqscmxrxrvwsnjjvygrelcbjgbpounhuyealllginkitfaiviraqcycjmskrozcdqylbuejrgfnquercvghppljmojfvylcxakyjxnampmakyjbqgwbyokaybcuklkaqzawageypfqhhasetugatdaxpvtevrigynxbqodiyioapgxqkndujeranxgebnpgsukybyowbxhgpkwjfdywfkpufcxzzqiuglkakibbkobonunnzwbjktykebfcbobxdflnyzngheatpcvnhdwkkhnlwnjdnrmjaevqopvinnzgacjkbhvsdsvuuwwhwesgtdzuctshytyfugdqswvxisyxcxoihfgzxnidnfadphwumtgdfmhjkaryjxvfquucltmuoosamjwqqzeleaiplwcbbxjxxvgsnonoivbnmiwbnijkzgoenohqncjqnckxbhpvreasdyvffrolobxzrmrbvwkpdbfvbwwyibydhndmpvqyfmqjwosclwxhgxmwjiksjvsnwupraojuatksjfqkvvfroqxsraskbdbgtppjrnzpfzabmcczlwynwomebvrihxugvjmtrkzdwuafozjcfqacenabmmxzcueyqwvbtslhjeiopgbrbvfbnpmvlnyexopoahgmwplwxnxqzhucdieyvbgtkfmdeocamzenecqlbhqmdfrvpsqyxvkkyfrbyolzvcpcbkdprttijkzcrgciidavsmrczbollxbkytqjwbiupvsorvkorfriajdtsowenhpmdtvamkoqacwwlkqfdzorjtepwlemunyrghwlvjgaxbzawmikfhtaniwviqiaeinbsqidetfsdbgsydkxgwoqyztaqmyeefaihmgrbxzyheoegawthcsyyrpyvnhysynoaikwtvmwathsomddhltxpeuxettpbeftmmyrqclnzwljlpxazrzzdosemwmthcvgwtxtinffopqxbufjwsvhqamxpydcnpekqhsovvqugqhbgweaiheeicmkdtxltkalexbeftuxvwnxmqqjeyourvbdfikqnzdipmmmiltjapovlhkpunxljeutwhenrxyfeufmzipqvergdkwptkilwzdxlydxbjoxjzxwcfmznfqgoaemrrxuwpfkftwejubxkgjlizljoynvidqwxnvhngqakmmehtvykbjwrrrjvwnrteeoxmtygiiygynedvfzwkvmffghuduspyyrnftyvsvjstfohwwyxhmlfmwguxxzgwdzwlnnltpjvnzswhmbzgdwzhvbgkiddhirgljbflgvyksxgnsvztcywpvutqryzdeerlildbzmtsgnebvsjetdnfgikrbsktbrdamfccvcptfaaklmcaqmglneebpdxkvcwwpndrjqnpqgbgihsfeotgggkdbvcdwfjanvafvxsvvhzyncwlmqqsmledzfnxxfyvcmhtjreykqlrfiqlsqzraqgtmocijejneeezqxbtomkwugapwesrinfiaxwxradnuvbyssqkznwwpsbgatlsxfhpcidfgzrc";
    CHECK(LongestDupSubStr().longestDupSubstring(long_str) == "ana");
}