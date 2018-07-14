#include "stdafx.h"
#include "..\catch.hpp"  // don't put this file in stdafx.h

#include "myalgo.h"
using namespace std;

//#include <complex>
class String {
public:
	// 791. Custom Sort String, sort t same as letter order in s
	string customSortString(string S, string T) {  // beat 100%
		size_t order[128];  // map letter to index for ordering
		int i = 0;
		for_each(begin(S), end(S), [&order, &i](char c) { order[c] = i++; }); // no raw loop
		sort(begin(T), end(T), [&order](char a, char b) { return order[a] < order[b]; });
		return T;
	}

	// 537. Complex Number Multiplication, in form of a+bi, no extra blank
	// Input: "1+-1i", "1+-1i"
	// Output: "0+-2i"
	vector<int> parseComplex(const string& s) {
		vector<string> ord= tokenizer(s.cbegin(), s.cend(), "[\\+i]+");
		return{ atoi(ord[0].c_str()),atoi(ord[1].c_str()) };
	}
	string complexNumberMultiply(string a, string b) {  // beat 59%
		//auto c1 = parseComplex(a);
		//auto c2 = parseComplex(b);
		stringstream sa(a), sb(b), oss;
		int c1[2], c2[2];
		char buff;
		sa >> c1[0] >> buff >> c1[1];  // much faster than parseComplex, 0 ms vs 4 ms, beat 100%
		sb >> c2[0] >> buff >> c2[1];
		auto c = multiplyComplex(c1[0], c1[1], c2[0],c2[1]);
		oss << c.first << '+' << c.second << 'i';
		return oss.str();// to_string(r).append(1, '+').append(to_string(i)).append(1, 'i');
	}

	// 553. Optimal Division, maximize result, add / and parenthesis only, no redundant parenthesis
	// size [1,10], nums val [2,1000]
	// simple algorithm to maximize division of integers:
	// use first int as dividend, rest as divisor. divide in sequential order to minimize divisor
	string optimalDivision(vector<int>& nums) { // tricky question, but easy. beat 100%
		if (nums.empty())
			return "";
		string str;
		str.reserve(50);
		str.append(to_string(nums[0]));
		int n = nums.size();
		if (n == 1)
			return str;
		str.append(1, '/');
		if (n > 2)
			str.append(1, '(');
		for (int i = 1; i < n; i++) {
			str.append(to_string(nums[i]));
			if (i < n - 1)
				str.append(1, '/');
		}
		if (n > 2)
			str.append(1, ')');
		return str;
	}

	// 856. Score of Parentheses
	//() 1, ()() 1+1=2, (()) 2*1=2
	size_t pos=0;
	int scoreOfParenthesesHelp(const string& S) {
		int res = 0;
		while (pos < S.size()) {
			char c = S[pos++];
			if (c == '(') {
				if (S[pos] == ')') {
					res++;
					pos++;
				}
				else
					res += 2 * scoreOfParenthesesHelp(S);
			}
			else
				return res;
		}
		return res;
	}

	int scoreOfParentheses(const string& S) {  // borrow idea, stack or recursion
		pos = 0;
		return scoreOfParenthesesHelp(S);
	}

	// 609. Find Duplicate File in System, output files with duplicate content
	vector<vector<string>> findDuplicate(vector<string>& paths) { // beat 99% easy
		unordered_map<string, vector<string>> contents;
		for (const auto& p : paths) {
			vector<string> files;
			istringstream iss(p);
			copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(files)); // tokenize path
			for (size_t i = 1; i < files.size(); i++) { // first is file dir, rest are file name and content
				auto c = files[i].find_first_of('(');
				if (c != string::npos) { // map file content to file path
					contents[files[i].substr(c + 1, files[i].size() - c - 2)].push_back(files[0]+"/"+files[i].substr(0, c));
				}
			}
		}
		vector<vector<string>> ans;
		transform_if(begin(contents), end(contents), back_inserter(ans), [](auto&p) {return move(p.second); }, [](const auto&p) {return p.second.size() > 1; });
		return ans;
	}


	// 833. Find And Replace in String,
	// if source match S at index, replace with target. guarantee no overlap
	string findReplaceString(string S, vector<int>& indexes, vector<string>& sources, vector<string>& targets) {// beat 99.8%
		using triple = tuple<int, string, string>;
		vector<triple> triplets;  // bundle and sort by index
		int n = indexes.size();
		triplets.reserve(n);
		for (int i = 0; i < n; i++)
			triplets.emplace_back(indexes[i], sources[i], targets[i]);
		sort(begin(triplets), end(triplets), [](auto&a, auto&b) {return get<0>(a) > get<0>(b); }); // sort reverse order
		for_each(begin(triplets), end(triplets), [&S](const auto& tr) {
			const auto&src = get<1>(tr);
			auto loc = cbegin(S) + get<0>(tr);
			if (equal(cbegin(src), cend(src), loc)) {  // if source match S, replace, from right to left
				const auto& dst = get<2>(tr);
				S.replace(loc, loc + src.size(), dst);
			}
		});
		return S;
	}

	int minDifference(vector<int>& minutes)
	{
		bucket_sort(begin(minutes), end(minutes), 24 * 60);
		int ans = INT_MAX;
		int n = minutes.size();
		for (int i = 1; i < n; i++)
			ans = min(ans, minutes[i] - minutes[i - 1]);
		return min(ans, 24 * 60 + minutes[0] - minutes[n - 1]);
	}

	// 539. Minimum Time Difference in minutes. The input time is legal and ranges from 00:00 to 23:59, points [2,20000]
	int findMinDifference(vector<string>& timePoints) {  // beat 73% using sort, beat 97% with bucket sort
		vector<int> minutes;
		minutes.reserve(timePoints.size());
		auto convert = [](const string& s) {
			istringstream iss(s);
			int h, m;
			iss >> h;
			iss.ignore(1);
			iss >> m;
			return h * 60 + m;
		};
		transform(begin(timePoints), end(timePoints), back_inserter(minutes), convert);
		return minDifference(minutes);
	}

	// 831. Masking Personal Information, string can be either email or phone
	// email in form name1@name2.name3, mask name 1 with first and last letter and 5 * in between
	// phone # use [+-() ], mask local # as ***-***-1111, mask international # as +***-***-***-1111
	string maskPII(string S) {  // beat 100%, 0ms vs avg 4 ms
		if (isalpha(S[0])) {  // email
			transform(begin(S), end(S), begin(S), [](unsigned char c) { return tolower(c); });
			size_t at = S.find_first_of('@');
			if (at != string::npos) {
				S.replace(1, at - 2, 5, '*');
			}
			return S;
		}
		else {
			int digits = count_if(begin(S), end(S), [](unsigned char c) { return isdigit(c); });
			string masking;
			masking.reserve(digits + 4);
			if (digits > 10) {  // add county code mask
				masking.append(1, '+');
				masking.append(digits - 10, '*');
				masking.append(1, '-');
			}
			masking.append("***-***-");  // local mask
			int len = masking.size();
			int count = 0;
			for (auto ch = rbegin(S); ch != rend(S) && count<4; ch++) {
				if (isdigit(*ch)) {
					masking.append(1, *ch);
					count++;
				}
			}
			reverse(begin(masking) + len, end(masking));  // reverse order of last 4 digits
			return masking;
		}
	}

	// 49. Group Anagrams
	vector<vector<string>> groupAnagrams(vector<string>& strs) { // beat 96%
		map<string, vector<string>> group;
		for (const auto& s : strs) {
			string key = s;
			bucket_sort(begin(key), end(key), 128); // sort string as key
			group[key].emplace_back(s); // group sring by sorted key
		}
		//vector<vector<string>> ans;
		//ans.reserve(group.size());
		//transform(begin(group), end(group), back_inserter(ans), [](auto&p) {return move(p.second); });  // move groups from map to vector
		return map2vec(group);
	}
};


TEST_CASE("Complex multiply", "[COMPLEX]")
{
	String s;
	CHECK(s.complexNumberMultiply("1+1i", "1+1i") == "0+2i");
}


TEST_CASE("parenthesis score", "[paren]")
{
	String s;
	CHECK(s.scoreOfParentheses("((()())())") == 10);
	CHECK(s.scoreOfParentheses("()") == 1);
	CHECK(s.scoreOfParentheses("()()") == 2);
	CHECK(s.scoreOfParentheses("(())") == 2);
	CHECK(s.scoreOfParentheses("(()(()))") == 6);
}


TEST_CASE("find duplicate files", "[NEW]")
{
	String s;
	CHECK(s.findDuplicate(vector<string>{ "root/a 1.txt(abcd) 2.txt(efgh)", "root/c 3.txt(abcd)", "root/c/d 4.txt(efgh)", "root 4.txt(efgh)" }) 
		== vector<vector<string>>{ {"root/a/2.txt", "root/c/d/4.txt", "root/4.txt"}, {"root/a/1.txt", "root/c/3.txt"}});
}


TEST_CASE("string replacements", "[NEW]")
{
	String s;
	CHECK(s.findReplaceString("abcd", vector<int>{0, 2}, vector<string>{"ab", "ec"}, vector<string>{"eee", "ffff"}) == "eeecd");
}


class StringX {
public:
	struct validNumber
	{
		const string& str;
		int first;
		int last;
		bool lead0;  // ends none zero
		validNumber(const string& s, int f, int l) :str(s), first(f), last(l) {
			lead0 = (str[first] == '0');
		}

		string generate1()
		{
			string res;
			res.append(1, str[first]).append(1, '.').append(begin(str) + first + 1, begin(str) + last);
			return res;
		}
		vector<string> generate12()
		{
			int len = last - first;
			vector<string> ans;
			if (len == 1 || !lead0) {  // add whole
				ans.push_back(str.substr(first, len));
			}
			if (len < 2 || str[last - 1] == '0')
				return ans;
			auto b = begin(str) + first;
			auto e = b + len;
			int loop = lead0 ? 2 : len;
			for (int dp = 1; dp < loop; dp++) {
				string res;
				res.reserve(len + 1);
				res.append(b, b + dp).append(1, '.').append(b + dp, e);
				ans.push_back(res);
			}
			return ans;
		}
		template<typename BackIns>
		void generate(BackIns back, string second) {
			int len = last - first;
			int totalLen = len + second.size() + 5;
			auto b = begin(str) + first;
			auto e = b + len;
			if (len == 1 || !lead0) {  // add whole
				string res;
				res.reserve(totalLen);
				res.append(1, '(').append(b, e).append(", ").append(second).append(1, ')');
				*back++ = res;
			}
			if (len < 2 || str[last - 1] == '0')
				return;
			int loop = lead0 ? 2 : len;
			for (int dp = 1; dp < loop; dp++) {
				string res;
				res.reserve(totalLen);
				res.append(1, '(').append(b, b + dp).append(1, '.').append(b + dp, e).append(", ").append(second).append(1, ')');
				*back++ = res;
			}
		}

		template<typename BackIns>
		void generate3(BackIns back, string str1st) {
			int len = last - first;
			int totalLen = len + str1st.size() + 5;
			auto b = begin(str) + first;
			auto e = b + len;
			if (len == 1 || !lead0) {  // add whole
				string res;
				res.reserve(totalLen);
				res.append(1, '(').append(str1st).append(", ").append(b, e).append(1, ')');
				*back++ = res;
			}
			if (len < 2 || str[last - 1] == '0')
				return;
			int loop = lead0 ? 2 : len;
			for (int dp = 1; dp < loop; dp++) {
				string res;
				res.reserve(totalLen);
				res.append(1, '(').append(str1st).append(", ").append(b, b + dp).append(1, '.').append(b + dp, e).append(1, ')');
				*back++ = res;
			}
		}
		template<typename BackIns, typename RandI>
		void generate2(BackIns back, RandI s1, RandI s2) {
			int len = last - first;
			auto b = begin(str) + first;
			auto e = b + len;
			string res;
			res.reserve(str.size());
			if (len == 1 || !lead0) {  // add whole
				for (auto s = s1; s != s2; s++) {
					res.append(1, '(').append(b, e).append(", ").append(*s).append(1, ')');
					*back++ = res;
					res.clear();
				}
			}
			if (len < 2 || str[last - 1] == '0')
				return;
			int loop = lead0 ? 2 : len;
			for (int dp = 1; dp < loop; dp++) {
				for (auto s = s1; s != s2; s++) {
					res.append(1, '(').append(b, b + dp).append(1, '.').append(b + dp, e).append(", ").append(*s).append(1, ')');
					*back++ = res;
					res.clear();
				}
			}
		}
	};
	// 816. Ambiguous Coordinates
	// rule 1: a number cannot have 0 at start and end
	// rule 2: if a number start with 0, it must contain ., total 1 choices
	// rule 3: if a number ends with 0, it must not contain ., i.e. 1 choice whole number
	// rule 4: if a number does not start or end with 0, choices=n, whole, plus dp in n-1 places
	// rules for string S, (###)
	// rule 5: if S starts and ends with 0, split string at consecutive none zero only
	// rule 6: if S does not starts or ends 0, split at any places
	// rule 7: if S starts 0 but not end 0, split must skip until none zero
	// rule 8: if S starts none 0 but ends 0, split must find none zero as second part
	vector<string> ambiguousCoordinates(string S) {  // beat 100%
		vector<string> ans;
		int start = 1;
		int end = S.size() - 1;   // exclusive
		int len = S.size() - 2;
		if (len < 2)  // cannot be shorter than 4, (12)
			return ans;
		if (len == 2) {
			ans.push_back({ '(',S[start],',',' ',S[end - 1],')' });
			return ans;
		}
		bool start0 = (S[start] == '0');
		bool end0 = (S[end - 1] == '0');
		if (start0 && end0) {
			// two consecutive 1, special case at begining and end
			bool prev = true; // so first 0 is chosen
			for (int d = start + 1; d < end; d++) {
				if (S[d] == '0') {
					if (prev && d == end - 1) {  // last case
						validNumber num(S, start, d);
						num.generate(back_inserter(ans), S.substr(d, end - d));
					}
					prev = false;
				}
				else {
					if (prev) {
						validNumber num(S, start, d);
						num.generate(back_inserter(ans), S.substr(d, end - d));
					}
					else
						prev = true;
				}
			}
		}
		else if (start0) {
			validNumber num(S, start + 1, end);
			num.generate3(back_inserter(ans), "0");
			for (int d = start + 1; d < end - 1; d++) {  // d belong to first part
				if (S[d] != '0') {
					validNumber num1(S, start, d + 1);
					validNumber num2(S, d + 1, end);
					num2.generate3(back_inserter(ans), num1.generate1());
				}
			}
		}
		else if (end0) {
			for (int d = start + 1; d < end - 1; d++) {  // d belong to second part
				if (S[d] != '0') {
					validNumber num1(S, start, d);
					num1.generate(back_inserter(ans), S.substr(d, end - d));
				}
			}
			validNumber num1(S, start, end - 1);
			num1.generate(back_inserter(ans), "0");
		}
		else {
			for (int d = start + 1; d < end; d++) {
				validNumber num1(S, start, d);
				validNumber num2(S, d, end);
				auto second = num2.generate12();
				num1.generate2(back_inserter(ans), begin(second), second.end());
			}
		}
		return ans;
	}
};


TEST_CASE("Minimum time difference", "[DIFF]")
{
	String s;
	CHECK(s.findMinDifference(vector<string>{ "23:58", "00:00", "23:57" }) == 1);
}


TEST_CASE("Mask info", "[MASK]")
{
	String s;
	CHECK(s.maskPII("LeetCode@LeetCode.com") == "l*****e@leetcode.com");
	CHECK(s.maskPII("AB@qq.com") == "a*****b@qq.com");
	CHECK(s.maskPII("1(234)567-890") == "***-***-7890");
	CHECK(s.maskPII("86-(10)12345678") == "+**-***-***-5678");
}

TEST_CASE("coordinates valid", "[COOR]")
{
	StringX s;
	CHECK(s.ambiguousCoordinates("(1001)") == vector<string>{"(1, 0.01)", "(10, 0.1)", "(100, 1)"});
	CHECK(s.ambiguousCoordinates("(00)") == vector<string>{"(0, 0)"});
	CHECK(s.ambiguousCoordinates("(01230)") == vector<string>{"(0, 1230)", "(0.1, 230)", "(0.12, 30)", "(0.123, 0)"});
	CHECK(s.ambiguousCoordinates("(010234)") == vector<string>{"(0, 10234)", "(0, 1.0234)", "(0, 10.234)", "(0, 102.34)", "(0, 1023.4)", "(0.1, 0.234)", "(0.102, 34)", "(0.102, 3.4)", "(0.1023, 4)"});
	CHECK(s.ambiguousCoordinates("(123040)") == vector<string>{"(1, 23040)", "(12, 3040)", "(1.2, 3040)", "(1230, 40)", "(12304, 0)", "(1.2304, 0)", "(12.304, 0)", "(123.04, 0)", "(1230.4, 0)"});
	CHECK(s.ambiguousCoordinates("(12304)") == vector<string>{"(1, 2304)", "(1, 2.304)", "(1, 23.04)", "(1, 230.4)", "(12, 304)", "(12, 3.04)", "(12, 30.4)", "(1.2, 304)", "(1.2, 3.04)", "(1.2, 30.4)", "(123, 0.4)", "(1.23, 0.4)", "(12.3, 0.4)", "(1230, 4)"});
}


struct CharCount
{
	const char ch;
	const int  cnt;
	CharCount(char c, int i) :ch(c), cnt(i) {}
};
class MoreString {
public:
	// 809. Expressive Words, letters are extended to express feeling. 
	// A group is extended if that group is length 3 or more
	// For some given string S, a query word is stretchy if it can be made to be equal to S by extending some groups
	int expressiveWords(string S, vector<string>& words) { // beat 100%
		if (S.empty())
			return 0;
		vector<CharCount> chc;  // count letters
		char last = S[0];
		int count = 0;
		for (char c : S) {
			if (c == last)
				count++;
			else {
				chc.emplace_back(last, count);
				last = c;
				count = 1;
			}
		}
		chc.emplace_back(last, count);  // add last one

		auto first = begin(chc);
		auto endi = end(chc);
		auto match = [first, endi](const string& w) { // match word by letter count
			if (w.empty())
				return false;
			auto cur = first;
			char last = w[0];
			int count = 0;
			auto equal = [&cur, &last, &count]() { // either same count, or count is extended to at last 3
				if (cur->ch != last)
					return false;
				if (cur->cnt != count && (cur->cnt<count || cur->cnt < 3))  // requirement so extended definition is not clear
					return false;
				return true;
			};
			for (char c : w) {
				if (c == last)
					count++;
				else {
					if (!equal())  // compare each letter count
						return false;
					last = c;
					count = 1;
					++cur;
				}
			}
			if (!equal())
				return false;
			return ++cur == endi;
		};
		return count_if(begin(words), end(words), match);
	}

	//848. Shifting Letters
	// shift letter n times, in alphabeta order, wrap around from z to a
	// each shift count applies to all letters up tot his point
	string shiftingLetters(string S, vector<int>& shifts) {  // beat 99%, tricky overflow
		int count = 0;
		int n = shifts.size();
		for (int i = n - 1; i >= 0; i--) {  // shift from right
			count += shifts[i];
			count %= 26;		// avoid int overflow
			S[i] += count;
			if ((unsigned char)S[i] > 'z')   // watch out for char overflow, negative
				S[i] = (unsigned char)S[i] - 'z' + 'a' - 1;  // wrap around fron 'z' to 'a'
		}
		return S;
	}

	int longest(const string& s, bool present[])  // long substring of letters not preset in other tring
	{
		int max = -1;
		int start = 0;
		int next = 0;
		for (char c : s) {
			if (present[c]) {  // reset
				if (next - start > max)
					max = next - start;
				next = 0;
				start = 0;
			}
			else {
				next++;
			}
		}
		if (next - start > max)
			max = next - start;
		return max;
	}
	// 521. Longest Uncommon Subsequence I, the longest subsequence of one of these strings should not be any subsequence of the other strings
	// idea: if one string is longer, pick it. cases when two string of same len, check if string are iddentical
	int findLUSlength(string a, string b) { // easy once understood the confusing question, beat 100%
		if (a.size() == b.size())
			return a == b ? -1 : a.size();
		return max(a.size(), b.size());
	}
	// 522. Longest Uncommon Subsequence II
	int findLUSlength(vector<string>& strs) {
		auto comp = [](const string& a, const string& b) { return a.size() > b.size(); }; // sort string by len from long to short
		sort(strs.begin(), strs.end(), comp);
		auto start = begin(strs);
		while (start != end(strs)) {
			int ans = start->size();
			auto range = equal_range(start, end(strs), *start, comp); // find range of string of same length
			for (auto it = range.first; it != range.second; ++it) {   // examine each string  in this range
				if (count_if(strs.begin(), range.second, // search from begining to end of this range
					[it](const string&a) { return subsequence(a.begin(), a.end(), it->begin(), it->end()); }) == 1)
					return ans;  // find a string that is not subsequence of any string of equal or longer length, ==1 means find just itself
			}
			start = range.second; // try a shorter string if no solution found yet
		}
		return -1;
	}
};


TEST_CASE("match expressive words", "[EXPR]")
{
	MoreString s;
	string S = "nnnnsssuuuvvvwwwwdddddettttttaaaaaatttttnnnuuullllllqqqqoooooojggggggbbbbsssiiiiffffffwwwwwbkkk";
	auto v = vector<string>{ "nnssuvvwwdettatnuullqoojgbbssiifwbbkk", "nssuvvwdetaattnulqqoojjgbssiffwbkk", "nssuvvwwddeettaattnnulqojgbssiifwwbk", "nnssuuvvwwdeettaattnuulqoojjgbbssiiffwbbk", "nssuuvvwwdettaattnnullqqojgbsiifwbbk", "nnssuvvwdeettaatnullqqojjggbssiifwwbk", "nnssuvvwwdeettatnnuulqoojjgbssiiffwbkk", "nssuuvwwddeettattnnullqqoojggbbssifwwbbkk", "nnsuuvvwdeetattnuulqojjgbbssiiffwbkk", "nssuuvvwddeetaattnnulqqoojgbbssiffwbbk", "nnsuuvwwddettaatnuulqojgbbssifwbbkk", "nssuuvvwwddeettattnnulqoojjggbbsiiffwbkk", "nssuvvwwdettatnullqojggbbssifwbbk", "nnsuuvwddettattnuullqoojggbsifwwbkk", "nsuvwddetaatnuullqoojgbssiifwwbkk", "nsuvwdetatnulqojjgbbsiffwwbk", "nssuvvwddeettaattnnulqqojjgbbssiffwwbk", "nnssuvvwwddeetaatnnuulqojgbbssiffwbbkk", "nssuvvwddeettatnullqqoojgbssiffwbbkk", "nnssuvvwwdeettattnnulqqoojgbssifwbkk", "nsuuvvwwdettatnuulqqoojggbssifwbbk", "nssuvwwddeettattnulqojjggbbssifwwbkk", "nnssuuvvwdeetaattnnulqqoojjggbbsifwbk", "nsuvwdeetaatnulqoojggbssiffwwbbk", "nnsuvvwdeettattnullqoojjgbbsiffwbbkk", "nnssuvwdetattnnuullqqoojgbbsiiffwwbkk", "nnssuvwwdetattnnullqqoojjgbbssiifwwbk", "nnsuvvwwddeetattnnulqqojjggbsiifwwbbk", "nssuvwdetattnnuullqoojjggbsiiffwwbbk", "nnsuvvwwdetatnuulqoojjgbssifwbkk", "nsuuvvwwddeettaatnullqojgbbsiifwwbkk", "nsuuvwwddeettaattnuullqqoojgbbsifwbbk", "nsuvwdettaattnnuulqqojgbbssiifwwbk", "nnsuvwwdettaattnnulqqoojjgbsifwwbbk", "nsuvvwwddettaatnulqoojggbbsifwwbbkk", "nsuuvwwddeettaattnulqojgbbsiffwbkk", "nnsuvwwdeettaatnullqqoojgbsiiffwbbkk", "nssuvwwddettaattnulqoojgbsifwbkk", "nssuvwddeettaatnnullqojggbbssifwwbbk", "nnssuvvwwddeetatnulqoojjggbbsiiffwbk", "nnsuuvwddetattnnulqojjgbsiiffwwbk", "nssuuvvwwddeetaattnnulqqoojjgbbssifwwbbkk", "nsuvwdeettatnnullqqojggbbsiiffwwbkk", "nssuuvvwddeetaattnullqoojjggbbsifwbk", "nnsuvwwddettaatnnulqoojggbbssiiffwwbk", "nnsuuvwdettatnnuullqqojjgbsiffwwbbkk", "nsuvwdeettaattnnulqoojjgbssifwwbkk", "nnssuuvwddeettatnnullqqoojjgbbsiiffwbk", "nsuuvvwwdeetatnnuulqojggbssiiffwbk", "nnssuuvwwddeettaatnuullqojggbbssiffwbk", "nnsuuvvwwdeettaatnnuulqojjggbsiffwbk", "nnssuuvvwwddeetaattnuulqqojgbsiffwbk", "nssuvwddetaattnnuulqqojggbsiffwwbbkk", "nnsuuvvwwdetatnnullqqojggbssiifwbk", "nnsuuvwwddeetaattnullqojgbssiffwbbkk", "nssuvvwddetatnnulqojjggbbssiiffwwbbk", "nnsuuvvwwddeetatnullqojgbbssifwwbbkk", "nnssuvvwwdetaatnnulqqojggbbssifwwbkk", "nnssuvwdeettaatnnuulqojggbsiifwwbbk", "nssuvvwwddetatnuullqojjgbbssiffwbk", "nssuuvvwddeettatnuullqoojjgbbsiifwbbkk", "nssuuvvwwdettaatnuullqqoojjgbbssiifwbbk", "nnssuvvwwdeetaatnnullqqoojggbsiiffwbk", "nnssuuvwdettaatnuullqqoojggbbsiifwwbbkk", "nssuvvwddettattnulqojjgbsiffwbkk", "nnssuvwwdeetaatnullqojgbssifwwbkk", "nnssuvwwddettattnnullqqoojgbssiffwbk", "nsuuvwdeettaatnuulqoojjgbsiffwwbbk", "nssuvwdeetattnnulqojggbbsiiffwwbbk", "nnsuuvvwwdeettatnnuullqoojjggbssiiffwbbkk", "nnssuuvvwddeettatnulqojggbssiifwwbkk", "nnsuuvwdettaattnuulqoojjgbbssiifwwbkk", "nnsuvvwddeettaatnnulqqoojggbbsiiffwwbbk", "nsuuvwddeettaattnnuulqojjggbssifwwbbkk", "nnssuuvwwdettatnullqqojggbbssifwwbbk", "nnssuuvvwdetatnuulqojgbbsifwbbkk", "nnssuuvwddeetattnullqqoojjgbssiffwwbbk", "nssuvvwwdettattnnuulqqojgbbsifwwbbk", "nnssuvwwdettaattnnullqojgbssiifwwbkk", "nsuuvvwwdeettatnnulqqoojjggbbsiffwbbkk", "nssuvvwdettatnuulqqoojgbssiifwwbbkk", "nnssuvwddeettaattnuulqojggbbssiifwbk", "nnssuvwwddeettaattnullqqojggbsiifwwbbk", "nssuvwddeettaatnulqqoojgbsifwbbkk", "nnsuuvvwwdeetaattnnulqqoojjgbbssiiffwwbbkk", "nssuvwdeetatnulqqoojgbsiifwwbbk", "nssuvvwdeettaattnnuulqojggbbssifwbbkk", "nnssuvvwwddetattnuullqqoojgbssiiffwbbkk", "nnssuuvwdettatnuullqqoojggbbssiifwwbbkk", "nnssuvvwwddetatnuulqoojgbbssifwwbk", "nssuvvwwdeetaatnulqqojjgbsiiffwwbkk", "nnssuvwwdeettattnuulqojjggbssifwbk", "nnssuvvwwddetattnnullqoojgbbssiifwwbbk", "nnsuuvvwwdeettaatnulqqoojjggbsiiffwwbkk", "nssuuvvwddettattnnuullqoojjgbbssifwwbkk", "nssuvwwddeetaatnuulqqojjgbssiifwwbkk", "nnsuvvwwdettatnnuulqqoojggbbssiifwbkk", "nnssuuvwwdetatnuulqqojjggbbssiifwwbkk", "nsuuvwddettattnuulqojggbssiffwwbbk", "nssuuvwddeetattnnulqqoojgbssiiffwbbk" };
	CHECK(s.expressiveWords(S, v) == 12);
	CHECK(s.expressiveWords("heeelllooo", vector<string>{"hello", "hi", "helo"}) == 2);
	CHECK(s.expressiveWords("abcd", vector<string>{"abc"}) == 0);
}


TEST_CASE("shift letters", "[SHIFT]")
{
	MoreString s;
	CHECK(s.shiftingLetters("ruu", vector<int>{26, 9, 17}) == "rul");
}

TEST_CASE("longest uncommon sequence", "[NEW]")
{
	MoreString s;
	CHECK(s.findLUSlength("aba", "cdc") == 3);
	CHECK(s.findLUSlength(vector<string>{ "aabbcc", "aabbcc", "cb", "abc" }) == 2);
	CHECK(s.findLUSlength(vector<string>{ "bb", "ab", "bba", "bba", "bbb", "bbbb", "bbbb" }) == 2);
}