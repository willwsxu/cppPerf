#include <string>
#include <unordered_set>
#include <vector>
#include <iostream>

using namespace std;
class LongestDupSubStr
{
    const static long MOD = 1000000007; // llimited so multiple 2 numbers does not overflow
    vector<long long> pow31;
    vector<long long> poly_sum;  // pre-compute polynomial sum, e.g. 26 chars: a*31^25+b*31^24+...+y*31+z

    // finding dup string of length L, using hash
    int longestDupSubstring_helper(const string& s, int L);
public:
    class string_window_view
    {
        const string& data;  // reference won't compile without copy constructor
        int left, right;     // [left,right]
        const long hash_val;
        bool data_equal(const string_window_view& rhs) const {
            for (int i = left, j = rhs.left; i <= right; i++, j++) {
                if (data[i] != rhs.data[j])
                    return false;
            }
            return true;
        }
    public:
        string_window_view(const string& d, int l, int r, const vector<long long>& pow31, const vector<long long>& poly_sum) :data(d), left(l), right(r),
            hash_val ( (poly_sum[right + 1] - poly_sum[left] * pow31[right - left + 1] % MOD + MOD) % MOD ) { }

        friend bool operator==(const string_window_view& lhs, const string_window_view& rhs) noexcept{
            if (lhs.hash() != rhs.hash())
                return false;
            return lhs.data_equal(rhs);  // resolve hash collision
        }
        // member operator overload won't compile
        //bool operator==(const string_window_view& rhs) noexcept {
        //    if (hash() != rhs.hash())
        //        return false;
        //    return data_equal(rhs);  // resolve hash collision
        //}
        long hash() const {
            return hash_val;
        }
    };

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
        while (low < hi) {
            int mid = (low + hi) / 2;
            if (mid == low)  // don't want to be stuck on low!!
                mid = low+1;
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
namespace std {
    template<> struct hash<LongestDupSubStr::string_window_view>
    {
        typedef LongestDupSubStr::string_window_view argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept
        {
            return s.hash();
        }
    };
}

int LongestDupSubStr::longestDupSubstring_helper(const string& s, int L)
{
    if (L == 0)
        return -1;
    unordered_set<LongestDupSubStr::string_window_view> substr_hash;
    substr_hash.reserve((s.size() - L) * 2);
    for (int right = L - 1; right < s.size(); right++) {  // string s window [left, right] 
        int left_char = right - L + 1;  // correspond to poly_sum window [left+1, right+1]
        LongestDupSubStr::string_window_view wv(s, left_char, right, pow31, poly_sum);
        //long long left_sum = poly_sum[left_char] * pow31[L] % MOD;
        //long long window_hash = (poly_sum[right + 1] - left_sum + MOD) % MOD;
        if (substr_hash.count(wv))
            return right;
        substr_hash.insert(wv);
    }
    return -1;
}

#include <catch.hpp>
TEST_CASE("1044. Longest Duplicate Substring", "[BS]")
{
    CHECK(LongestDupSubStr().longestDupSubstring("banana") == "ana");
    string long_str = "moplvidmaagmsiyyrkchbyhivlqwqsjcgtumqscmxrxrvwsnjjvygrelcbjgbpounhuyealllginkitfaiviraqcycjmskrozcdqylbuejrgfnquercvghppljmojfvylcxakyjxnampmakyjbqgwbyokaybcuklkaqzawageypfqhhasetugatdaxpvtevrigynxbqodiyioapgxqkndujeranxgebnpgsukybyowbxhgpkwjfdywfkpufcxzzqiuglkakibbkobonunnzwbjktykebfcbobxdflnyzngheatpcvnhdwkkhnlwnjdnrmjaevqopvinnzgacjkbhvsdsvuuwwhwesgtdzuctshytyfugdqswvxisyxcxoihfgzxnidnfadphwumtgdfmhjkaryjxvfquucltmuoosamjwqqzeleaiplwcbbxjxxvgsnonoivbnmiwbnijkzgoenohqncjqnckxbhpvreasdyvffrolobxzrmrbvwkpdbfvbwwyibydhndmpvqyfmqjwosclwxhgxmwjiksjvsnwupraojuatksjfqkvvfroqxsraskbdbgtppjrnzpfzabmcczlwynwomebvrihxugvjmtrkzdwuafozjcfqacenabmmxzcueyqwvbtslhjeiopgbrbvfbnpmvlnyexopoahgmwplwxnxqzhucdieyvbgtkfmdeocamzenecqlbhqmdfrvpsqyxvkkyfrbyolzvcpcbkdprttijkzcrgciidavsmrczbollxbkytqjwbiupvsorvkorfriajdtsowenhpmdtvamkoqacwwlkqfdzorjtepwlemunyrghwlvjgaxbzawmikfhtaniwviqiaeinbsqidetfsdbgsydkxgwoqyztaqmyeefaihmgrbxzyheoegawthcsyyrpyvnhysynoaikwtvmwathsomddhltxpeuxettpbeftmmyrqclnzwljlpxazrzzdosemwmthcvgwtxtinffopqxbufjwsvhqamxpydcnpekqhsovvqugqhbgweaiheeicmkdtxltkalexbeftuxvwnxmqqjeyourvbdfikqnzdipmmmiltjapovlhkpunxljeutwhenrxyfeufmzipqvergdkwptkilwzdxlydxbjoxjzxwcfmznfqgoaemrrxuwpfkftwejubxkgjlizljoynvidqwxnvhngqakmmehtvykbjwrrrjvwnrteeoxmtygiiygynedvfzwkvmffghuduspyyrnftyvsvjstfohwwyxhmlfmwguxxzgwdzwlnnltpjvnzswhmbzgdwzhvbgkiddhirgljbflgvyksxgnsvztcywpvutqryzdeerlildbzmtsgnebvsjetdnfgikrbsktbrdamfccvcptfaaklmcaqmglneebpdxkvcwwpndrjqnpqgbgihsfeotgggkdbvcdwfjanvafvxsvvhzyncwlmqqsmledzfnxxfyvcmhtjreykqlrfiqlsqzraqgtmocijejneeezqxbtomkwugapwesrinfiaxwxradnuvbyssqkznwwpsbgatlsxfhpcidfgzrc";
    CHECK(LongestDupSubStr().longestDupSubstring(long_str) == "akyj");
}