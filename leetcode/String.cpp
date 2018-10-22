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


TEST_CASE("find duplicate files", "[STR]")
{
	String s;
	CHECK(s.findDuplicate(vector<string>{ "root/a 1.txt(abcd) 2.txt(efgh)", "root/c 3.txt(abcd)", "root/c/d 4.txt(efgh)", "root 4.txt(efgh)" }) 
		== vector<vector<string>>{ {"root/a/2.txt", "root/c/d/4.txt", "root/4.txt"}, {"root/a/1.txt", "root/c/3.txt"}});
}


TEST_CASE("string replacements", "[STR]")
{
	String s;
	CHECK(s.findReplaceString("abcd", vector<int>{0, 2}, vector<string>{"ab", "ec"}, vector<string>{"eee", "ffff"}) == "eeecd");
}


class Coordinates {
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
	Coordinates s;
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
				if (cur->cnt != count && (cur->cnt<count || cur->cnt < 3))  // requirement for extended definition is not clear
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

TEST_CASE("longest uncommon sequence", "[SEQ]")
{
	MoreString s;
	CHECK(s.findLUSlength("aba", "cdc") == 3);
	CHECK(s.findLUSlength(vector<string>{ "aabbcc", "aabbcc", "cb", "abc" }) == 2);
	CHECK(s.findLUSlength(vector<string>{ "bb", "ab", "bba", "bba", "bbb", "bbbb", "bbbb" }) == 2);
}


class StringMulptiply
{
	static string addImpl(string& s1, string& s2)  // s1 longer, add two string of numbers
	{
		auto dest = rbegin(s1);  // pick longer string as destination
		int carry = 0;
		for (auto src = rbegin(s2); src != rend(s2); ++src, ++dest) {
			int sum = (*src - '0') + (*dest - '0') + carry;
			*dest = sum % 10 + '0';
			carry = sum / 10;
		}
		if (carry > 0) {
			for (; dest != rend(s1); dest++) {
				carry += (*dest - '0');
				*dest = carry % 10 + '0';
				carry /= 10;
			}
		}
		if (carry)
			s1.insert(0, 1, carry + '0');
		return s1;
	}
public:
	static string add(string s1, string s2, int offset) // s2 shift to left before add
	{
		// use reverse iterator to get to position, then call base to get regular iterator so string is not reversed
		string s1r((s1.rbegin() + offset).base(), s1.end()); // same as shift S1 to right, save the offset part
		s1.erase((s1.rbegin() + offset).base(), s1.end());  // trim s1 to align with s2
		if (s1.size() < s2.size())
			return addImpl(s2, s1).append(s1r);  // append back s1 right portion
		return addImpl(s1, s2).append(s1r);
	}
	static string multiply(string num1, int i)
	{
		auto x = accumulate(rbegin(num1), rend(num1), 0, [i](int carry, char& d) {
			int m = i*(d-'0')+carry;
			d = (m % 10) + '0';  // convert product to digit
			return m / 10; // return carry after multiplication
		});
		return x>0?num1.insert(0,1,x+'0'):num1;
	}
	static string multiply(string num1, char i);  // declared to prevent bad callling with char

	static string multiplyImpl(string& num1, string& num2) {  // num2 is shorter
		string ans;
		int offset = 0;
		for (auto c2 = rbegin(num2); c2 != rend(num2); ++c2, offset++) {
			string prod = multiply(num1, *c2-'0');  // multiple one char at a time
			ans = add(ans, prod, offset);  // add to previous result, need to offset for alignment
		}
		return ans;
	}
	string multiply(string num1, string num2) { // beat 94%
		if (num1 == "0" || num2 == "0")
			return "0";
		if (num1.size() < num2.size())
			return multiplyImpl(num2, num1);
		return multiplyImpl(num1, num2);
	}
};


TEST_CASE("string multiply", "[MULT]")
{
	CHECK(StringMulptiply().multiply("0", "9193") == "0");
	CHECK(StringMulptiply().multiply("999", "999") == "998001");
	CHECK(StringMulptiply::multiply("323", 4) == "1292");
	CHECK(StringMulptiply::add("999", "2", 1) == "1019");
	CHECK(StringMulptiply::add("2", "999", 0) == "1001");
	CHECK(StringMulptiply::add("2", "999", 1) == "9992");
	CHECK(StringMulptiply().multiply("123", "456") == "56088");
}

class ValidIP
{
	template<typename Pred>
	bool validIP(const string&IP, const char *sep, int groups, Pred&& invalid) {
		char cSep = sep[strlen(sep) - 1];
		if (IP.front() == cSep || IP.back() == cSep)  // check if there . or : ar begin or end
			return false;
		vector<string> tokens = tokenizer(IP.begin(), IP.end(), sep);
		if (tokens.size() != groups)  // must be 4 or 8 groups
			return false;
		if (any_of(tokens.begin(), tokens.end(), invalid))  // find any invalid group
			return false;
		return true;
	}

public:
	// 468. Validate IP Address
	string validIPAddress(string IP) {  // beat 100% 0ms
		if (IP.find('.') != string::npos) {
			auto invalid = [](const string& s) {
				if (s.size() > 3 || s.empty())  // 1 to 3 digits
					return true;
				if (s[0] == '0' && s.size() > 1)  // leading 0 not allowed
					return true;
				if (any_of(s.begin(), s.end(), [](char c) { return !isdigit(c); }))
					return true; // not digit
				return atoi(s.c_str()) > 255;  // value out of range
			};
			if (validIP(IP, "\\.", 4, invalid))
				return "IPv4";
		}
		else if (IP.find(':') != string::npos) {
			auto invalid = [](const string& s) {
				if (s.size() > 4 || s.empty())  // 1 to 4 digits
					return true;
				if (!all_of(s.begin(), s.end(), [](char c) { return isxdigit(c); }))
					return true; // not all of hex digit
				return false;
			};
			if (validIP(IP, ":", 8, invalid))
				return "IPv6";
		}
		return "Neither";
	}
};


TEST_CASE("valid IP", "[STR]")
{
	CHECK(ValidIP().validIPAddress("172.16.254.1")=="IPv4");
	CHECK(ValidIP().validIPAddress("172.16.254.1.") == "Neither");
	CHECK(ValidIP().validIPAddress(".172.16.254.1") == "Neither");
	CHECK(ValidIP().validIPAddress("2001:0db8:85a3:0:0:8A2E:0370:7334") == "IPv6");
	CHECK(ValidIP().validIPAddress("2001:0db8:85a3:0:0:8A2E:0370:7334:") == "Neither");
	CHECK(ValidIP().validIPAddress("256.256.256.256") == "Neither");
}

//165. Compare Version Numbers
class StringParsing
{
	bool any_none_zero(stringstream& iss) {
		int val;
		while (!iss.eof()) {
			if (iss.peek() == '.')
				iss.ignore();
			iss >> val;
			if (val)
				return true;
		}
		return false;
	}
public:
	int compareVersion(string version1, string version2) {  // simple, beat 100%
		stringstream v1(version1), v2(version2);
		int val1, val2;
		while (!v1.eof() && !v2.eof()) {
			if (v1.peek() == '.')
				v1.ignore();
			if (v2.peek() == '.')
				v2.ignore();
			v1 >> val1;
			v2 >> val2;
			if (val1 < val2)
				return -1;
			if (val1 > val2)
				return 1;
		}
		if (v1.eof() && v2.eof())
			return 0;
		if (v1.eof())
			return any_none_zero(v2) ? -1 : 0;  // special case, 1.0 is same as 1
		return any_none_zero(v1) ? 1 : 0;
	}

	// 71. Simplify Path (Unix-style)
	// remove any redundant  /
	// interpret . and ..
	string simplifyPath(string path) {  // beat 55%
		auto tokens = tokenizer(path.cbegin(), path.cend(), "/");
		deque<string> pk;
		for (const string& t : tokens) {
			if (t.empty() || t == ".")
				continue;
			if (t == "..") {
				if (!pk.empty())
					pk.pop_back();
			}
			else
				pk.push_back(t);
		}
		if (pk.empty())
			return "/";
		stringstream oss;
		for (const string& t : pk) {
			oss << '/' << t;
		}
		return oss.str();
	}
};


TEST_CASE("version compare", "[VER]")
{
	StringParsing v;
	CHECK(v.compareVersion("1.1", "0.1") == 1);
	CHECK(v.compareVersion("1.0.1.1", "1.0.1") == 1);
	CHECK(v.compareVersion("1.0", "1") == 0);
	CHECK(v.compareVersion("1.0", "1.0.0") == 0);
	CHECK(v.compareVersion("1.0", "1.0") == 0);
	CHECK(v.compareVersion("1.0", "1.0.1") == -1);
	CHECK(v.compareVersion("1.0", "1.1.1") == -1);
}

TEST_CASE("simplify path", "[STR]")
{
	StringParsing v;
	CHECK(v.simplifyPath("/home//foo/") == "/home/foo");
	CHECK(v.simplifyPath("/../") == "/");
	CHECK(v.simplifyPath("/a/./b/../../c/") == "/c");
}

class StringWords {
public:
	void reverseWords(string &s) {  // easy beat 99%
		reverse(s.begin(), s.end());  // reverse the whole string
		istringstream iss(s);
		auto first = s.begin();
		auto last = s.end();
		for_each(istream_iterator<string>(iss), istream_iterator<string>(), [&first, last](const string& word) {
			for (auto x = word.rbegin(); x != word.rend(); ++x)  // reverse back the word
				*first++ = *x;
			if (first != last)
				*first++ = ' ';
		});
		if (*(first - 1) == ' ')  // strip last space if exist
			--first;
		s.erase(first, last);
	}
};

TEST_CASE("reverse words in a string", "[STR]")
{
	string str{ "the sky is blue" };
	StringWords().reverseWords(str);
	CHECK(str == "blue is sky the");
	string str2{ " the sky is  blue " };
	StringWords().reverseWords(str2);
	CHECK(str2 == "blue is sky the");
}


class MatchLetters
{
public:
	// 748. Shortest Completing Word
	// find shortest word contain all letters in target
	// target can have digits, space, upper or lower case letters, len 1 to 7
	// word dictionary all lower cases, word len [1,15], dictionary size [10, 1000]
	// if there is multiple matches, return first one
	string shortestCompletingWord(string licensePlate, vector<string>& words) { // beta 100%
		int target[26] = { 0 }; // store char count of target
		int count = 0;
		for (char c : licensePlate) {
			if (isalpha(c)) {
				target[tolower(c) - 'a']++;
				count++;
			}
		}
		const string *ans = nullptr;
		for (string const & w : words) {
			vector<int> t(begin(target), end(target)); // make a copy of target
			int match = count;
			for (char c : w) {
				if (t[c - 'a'] > 0) {  // decrement char count if there is a match
					t[c - 'a']--;
					if (match-- == 1)
						break;
				}
			}
			if (match == 0) {  // complete match
				if (ans == nullptr || ans->size()>w.size())  // only update anser if len is smalller
					ans = &w;
			}
		}
		return ans ? *ans : "";
	}
};


TEST_CASE("Match letters", "[NEW]")
{
	CHECK(MatchLetters().shortestCompletingWord("1s3 PSt", vector<string>{"step", "steps", "stripe", "stepple"}) == "steps");
}

class StringStuff
{
public:
	string toLowerCase(string str) {
		transform(begin(str), end(str), begin(str), [](char c) {
			if (c >= 'A' && c <= 'Z')
				c = 'a' + (c - 'A');
			return c;
		});
		return str;
	}
	// 804. Unique Morse Code Words
	int uniqueMorseRepresentations(vector<string>& words) {  // beat 79%
		static const vector<string> morse_code = { ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." };
		set<string> uniq;
		for (const auto& w : words) {
			string code = accumulate(begin(w), end(w), string(), [](string&init, char c) { return init.append(morse_code[c - 'a']); });
			uniq.insert(code);
		}
		return uniq.size();
	}
	// 541. Reverse String II
	string reverseStr(string s, int k) { // beat 25%
		auto first = s.begin();
		auto last = s.end();
		int step = 2 * k;
		while (first != s.end()) {
			int remain = distance(first, last);
			if (remain <= k)
				reverse_string(first, last);
			else
				reverse_string(first, first + k);
			if (remain <= step)
				break;
			first += step;
		}
		return s;
	}
	// 557. Reverse Words in a String III
	string reverseWords(string s) {  // beat 96%
		auto first = s.begin();
		do {
			auto space = find(first, end(s), ' ');
			reverse_string(first, space);
			first = space;
			if (space == end(s))
				break;
		} while (++first != end(s));
		return s;
	}
	//657. Robot Return to Origin
	bool judgeCircle(string moves) {  // beat 97%
		int count[26] = { 0 };
		for (char m : moves)
			count[m - 'A']++;
		return count['U' - 'A'] == count['D' - 'A'] && count['L' - 'A'] == count['R' - 'A'];
	}
	// 859. Buddy Strings
	// Given two strings A and B of lowercase letters, return true if and only if we can swap two letters in A so that the result equals B
	bool buddyStrings(string A, string B) {// beat 98%, watch out for special case
		if (A.size() != B.size())
			return false;
		auto mismatch1 = mismatch(begin(A), end(A), begin(B), end(B));
		if (mismatch1.first == A.end()) {  // all matching, special case
			int count[26] = { 0 };  // looking for if there are chars same
			for (char c : A) {
				if (count[c - 'a']++ == 1)  // find same char in A
					return true;
			}
			return false;
		}
		auto mismatch2 = mismatch(mismatch1.first + 1, end(A), mismatch1.second + 1, end(B));
		if (mismatch2.first == A.end())
			return false;
		auto mismatch3 = mismatch(mismatch2.first + 1, end(A), mismatch2.second + 1, end(B));
		if (mismatch3.first != A.end())
			return false;
		return *mismatch1.first == *mismatch2.second && *mismatch1.second == *mismatch2.first;
	}
	// 387. First Unique Character in a String, only lowercase letters
	// Given a string, find the first non-repeating character in it and return it's index. If it doesn't exist, return -1.
	int firstUniqChar(string s) {
		int count[26] = { 0 };
		for (char c : s)
			count[c - 'a']++;
		auto found = find_if(s.begin(), s.end(), [&count](char c) { return count[c - 'a'] == 1; });
		if (found == s.end())
			return -1;
		return distance(s.begin(), found);
	}
	int firstUniqChar_fast(string s) {  // beat 97%
		const int MAX_POS = s.size();
		// array can not be used to initialize like vector
		vector<int> position(26, MAX_POS);  // keep track of first position of each letter, if more than once, mark as INT32_MAX
		for (int i = 0; i < MAX_POS; i++) {
			int idx = s[i] - 'a';
			if (position[idx] == MAX_POS)  // first appearance
				position[idx] = i;
			else if (position[idx] != INT32_MAX)
				position[idx] = INT32_MAX;
		}
		auto ans = min_element(begin(position), end(position));
		return *ans >= MAX_POS ? -1 : *ans;  // more error prone than first approach, due to states
	}
	// 893. Groups of Special-Equivalent Strings, only lowercase letters, All A[i] have the same length
	// Two strings S and T are special-equivalent if S==T after any number of moves, swapping char at odd or even position
	int numSpecialEquivGroups(vector<string>& A) {  // beat 100%
		auto compose_key = [](const string& s) {
			string key;
			key.reserve(s.size());
			int pos = 0;
			copy_if(begin(s), end(s), back_inserter(key), [&pos](char c) { return pos++ % 2 == 0; });  // even position
			sort(begin(key), end(key));  // sort even chars
			size_t old_len = key.size();
			pos = 0;
			copy_if(begin(s), end(s), back_inserter(key), [&pos](char c) { return pos++ % 2 == 1; });  // odd position
			sort(begin(key) + old_len, end(key));  // sort chars at odd pos
			return key;
		};
		set<string> group;
		for (const string&s : A) {
			group.insert(compose_key(s));
		}
		return group.size();
	}
	// 824. Goat Latin, upper and lower cases words in a sentence
	// for each words, if starting vowel, append ma, else move letter to end of word, append ma
	// for ith word, append i 'a', i from 1 to n
	string toGoatLatin(string S) {  // beat 100%
		const static set<char> vowels{ 'a','e','i','o','u','A','E','I','O','U' };
		int i = 0;
		auto word = S.begin();
		string ans;
		ans.reserve(S.size() * 2);
		while (word != S.end()) {
			auto space = find(word, S.end(), ' ');
			if (vowels.count(*word)) {
				ans.append(word, space);
			}
			else {
				ans.append(word + 1, space).append(1, *word);
			}
			ans.append("ma").append(++i, 'a');
			if (space == S.end())
				break;
			ans.append(1, ' ');
			word = space + 1;
		}
		return ans;
	}
	// 520. Detect Capital
	bool detectCapitalUse(string word) {  // beat 100%
		int countUp = 0;
		int countLow = 0;
		for (char c : word) {
			if (isupper(c)) {
				if (countLow)
					return false;  // upper case after lower case
				countUp++;
			}
			else {
				countLow++;
			}
		}
		return countLow == 0 || countUp <= 1;  // either all upper case, or first is upper, or all lower
	}
	// 696. Count Binary Substrings, of same number of 0's and 1's
	int countBinarySubstrings(string s) {  // beat 56%
		int group1 = 0;
		int group2 = 0;
		char last = 0;
		int ans = 0;
		for (char c : s) {
			if (c != last) {
				ans += min(group1, group2); // count of substring of group1 and group 2
				group1 = group2;  // shift to 2
				group2 = 1;  // next group
				last = c;
			}
			else
				group2++;
		}
		ans += min(group1, group2);
		return ans;
	}
	// 38. Count and Say, start with 1
	// count same digit and say last string, generate a new string as count+digit1+count+digit2
	string countAndSay(int n) {  // beat 100%
		string last = "1";
		while (--n) {
			int count = 0;
			char last_char = last[0];
			string new_str;
			for (char c : last) {
				if (c != last_char) {
					new_str.append(to_string(count)).append(1, last_char);
					count = 1;
					last_char = c;
				}
				else
					count++;
			}
			new_str.append(to_string(count)).append(1, last_char);
			last = move(new_str);
		}
		return last;
	}
	//13. Roman to Integer
	int romanToInt(string s) {  // beat 97%
		static const int roman[26] = { 0,0,100,500,0,0,0,0,1,0,0,50,1000,0,0,0,0,0,0,0,0,5,0,10 };
		int ans = 0;
		int size = s.size();
		for (int i = 0; i < size - 1; i++) {
			int cur = roman[s[i] - 'A'];
			switch (s[i]) {
			case 'C':
			case 'X':
			case 'I':
			{
				if (cur < roman[s[i + 1] - 'A']) // CM, CD // XC, XL // IX, IV
					ans -= cur;
				else
					ans += cur;
				break;
			}
			default:
				ans += cur;
				break;
			}
		}
		return ans + roman[s[size - 1] - 'A'];  // add last one
	}
	// 383. Ransom Note, lowe case only
	// check if note can written from lettters in magazine
	bool canConstruct(string ransomNote, string magazine) {  // beat 81%, easy
		int count[26] = { 0 };
		for (char c : magazine)
			count[c - 'a']++;
		for (char c : ransomNote) {
			if (count[c - 'a'] == 0)
				return false;
			count[c - 'a']--;
		}
		return true;
	}
	// 819. Most Common Word, not in banned list
	// word is always separated by a space, return only lower case letter
	string mostCommonWord(string paragraph, vector<string>& banned) {  // beat 98%
		map<string, int> count;
		int maxCount = 0;
		for (const string&s : banned)  // add banned words with negative count
			count[s] = INT32_MIN;
		auto word = paragraph.begin();
		string result;
		while (word != paragraph.end()) {
			auto space = find(word, paragraph.end(), ' ');
			auto end = space; // save
			if (!isalpha(*(space - 1)))  // strip last none letter char
				--space;
			string key;
			transform(word, space, back_inserter(key), [](char c) {return tolower(c); });  // convert to lower case
			auto& c = count[key];
			if (++c > maxCount) {  // count and keep track of max
				maxCount = c;
				result = key;
			}
			if (end == paragraph.end())
				break;
			word = end + 1;
		}
		return result;
	}
	// 551. Student Attendance Record I, no more than 1 Ansent, no mor than 2 continuous Late
	bool checkRecord(string s) {  // beat 50%, very easy
		int absent = 0;
		int lateCont = 0;
		for (char c : s) {
			switch (c) {
			case 'A':
				if (++absent > 1)
					return false;
				lateCont = 0;
				break;
			case 'L':
				if (++lateCont > 2)
					return false;
				break;
			default:
				lateCont = 0;
			}
		}
		return true;
	}
	// 459. Repeated Substring Pattern
	bool repeatedSubstringPattern(string s) {
		//return (s + s).substr(1, 2*s.size() - 2).find(s) != string::npos;  // borrowed idea, beat 95%
		// more straight forward method below also beat 95%
		int size = s.size();
		for (int step = size / 2; step > 0; step--) {  // iterate all possible substring length
			if (size%step == 0) {
				bool success = true;
				for (int j = step; j < size; j += step) {
					if (!equal(begin(s), begin(s) + step, begin(s) + j, begin(s) + j + step)) {
						success = false;
						break;
					}
				}
				if (success)
					return true;
			}
		}
		return false;
	}
	// 28. Implement strStr()
	int strStr(string haystack, string needle) { // brute force, beat 98%
		for (int i = 0; i <= (int)(haystack.size() - needle.size()); i++) {
			if (equal(begin(haystack) + i, begin(haystack) + i + needle.size(), begin(needle), end(needle)))
				return i;
		}
		return -1;
	}
	// 686. Repeated String Match
	// find the minimum number of times A has to be repeated such that B is a substring of it
	int repeatedStringMatch(string A, string B) { // brute force, beat 53%
		int a_size = A.size();
		A.reserve(B.size() + A.size());
		int ans = 1;
		while (A.size() < B.size()) {  // make A as long as B
			A.append(A.begin(), A.begin() + a_size);
			ans++;
		}
		if (A.find(B) != string::npos)
			return ans;
		A.append(A.begin(), A.begin() + a_size);  // try again repeat one more time
		if (A.find(B) != string::npos)
			return ans + 1;
		return -1;
	}
	// 434. Number of Segments in a String
	int countSegments(string s) {  // beat 100%
		int count = 0;
		int word = 0; // letter count
		for (char c : s) {
			if (isspace(c)) {
				if (word) {
					word = 0;
					count++;
				}
			}
			else
				word++;
		}
		return word ? count + 1 : count;
	}
	// 443. String Compression, in place
	int compress(vector<char>& chars) {  // beat 25%
		int count = chars.empty() ? 0 : 1;
		auto write_count = [&chars](int to, int count) {
			int write = to;
			while (count > 0) {
				chars[write++] = (count % 10 + '0');
				count /= 10;
			}
			if (write - to > 1)
				reverse(chars.begin() + to, chars.begin() + write);
			return write - 1; // position for next char
		};
		int to = 0; // current char
		for (size_t from = 1; from < chars.size(); from++) {
			if (chars[from] != chars[to]) {
				if (count > 1)
					to = write_count(to + 1, count);
				count = 1;
				chars[++to] = chars[from]; // move to postion storing next char
			}
			else
				count++;
		}
		if (count > 1)
			to = write_count(to + 1, count);
		return to + 1;
	}
	// 20. Valid Parentheses
	bool isValid(string s) {  // beat 100%
		deque<char> paren;
		for (char c : s) {
			switch (c) {
			case '(':
			case '[':
			case '{':
				paren.push_back(c);
				break;
			case ')':
				if (!paren.empty() && paren.back() == '(')
					paren.pop_back();
				else
					return false;
				break;
			case ']':
				if (!paren.empty() && paren.back() == '[')
					paren.pop_back();
				else
					return false;
				break;
			case '}':
				if (!paren.empty() && paren.back() == '{')
					paren.pop_back();
				else
					return false;
				break;
			}
		}
		return paren.empty();
	}
	// 58. Length of Last Word
	int lengthOfLastWord(string s) { // beat 100%
		auto rfind = s.rbegin();
		while (rfind != s.rend() && *rfind == ' ')  // skip space from end
			++rfind;
		if (rfind == s.rend())
			return 0;
		auto end = find(rfind, s.rend(), ' ');
		return distance(rfind, end);
	}
	// 14. Longest Common Prefix, lowercase letters
	string longestCommonPrefix(vector<string>& strs) {  // beat 98%
		if (strs.empty())
			return "";
		if (strs.size() == 1)
			return strs[0];
		const string& first = strs[0];
		size_t match = first.size();
		for (size_t i = 1; i < strs.size(); i++) {
			match = min(match, strs[i].size());
			size_t j = 0;
			for (; j < match; j++) {
				if (first[j] != strs[i][j])
					break;
			}
			if (j == 0)
				return "";
			match = j;
		}
		return first.substr(0, match);
	}
	// 796. Rotate String
	bool rotateString(string A, string B) {  // brute force, try all rotation, beat 100%
		if (A.size() != B.size())
			return false;
		for (size_t i = 0; i < A.size() - 1; i++) {
			rotate(A.begin(), A.begin() + 1, A.end()); // rotate left once
			if (A == B)
				return true;
		}
		return false;
	}
	bool rotateString_clever(string A, string B) {  // beat 100%
		if (A.size() == B.size() && A.append(A).find(B) != string::npos)
			return true;
		return false;
	}
	// 717. 1-bit and 2-bit Characters
	// valid bit: 0, 10, 11
	bool isOneBitCharacter(vector<int>& bits) {  // beat 98%
		for (size_t i = 0; i < bits.size(); i++) {
			if (i == bits.size() - 1)
				return  bits[i] == 0;
			if (bits[i] == 1) {  // 2 bits
				i++;  // skip next bit
			}
		}
		return false;
	}
	// 482. License Key Formatting, divide into N+1 group of K length, except the first group
	string licenseKeyFormatting(string S, int K) {  // beat 26%
		S.erase(remove(begin(S), end(S), '-'), end(S));
		transform(begin(S), end(S), begin(S), [](char c) {return toupper(c); });
		for (int i = S.size() - K; i > 0; i -= K)
			S.insert(i, 1, '-');
		return S;
	}
	// 820. Short Encoding of Words
	// use # for end of words, index array for start of a word
	bool startWith(const string&prefix, const string&target)
	{
		if (prefix.size() > target.size())
			return false;
		return target.compare(0, prefix.size(), prefix) == 0;// compare prefix of target to prefix string
	}
public:
	int minimumLengthEncoding(vector<string>& words) {  // beat 81%, use sorting
		if (words.empty())
			return 0;
		for (string& s : words)
			reverse(begin(s), end(s));
		sort(begin(words), end(words));
		int len = 0;
		auto last = begin(words);
		for (auto word = begin(words) + 1; word != end(words); ++word) {
			if (!startWith(*(word - 1), *word)) {  // find longest word ends with same suffix
				len += (word - 1)->size();
				len++;
			}
		}
		len += words.back().size();
		return len + 1;
	}
	// 925. typed name could repeat same letter when pressed too long
	bool isLongPressedName(string name, string typed) { // easy, beat 100%
		if (name.empty() || typed.empty() || name[0] != typed[0])
			return false;
		int idx = 0;
		for (size_t i = 0; i < name.size(); i++) {
			if (typed[idx] == name[i])
				idx++;
			else {
				if (name[i] == name[i - 1])  // same letter as last one, typed does not have enough
					return false;
				while (typed[idx] == typed[idx - 1])  // skip repeating letter
					idx++;
				if (typed[idx] != name[i])
					return false;
				idx++;
			}
		}
		return true;
	}
	// 926, flip to increasing, only 0 or 1
	int minFlipsMonoIncr(string S) {  // beat 100%, similar to 122?
		vector<int> count0back(S.size()+1, 0);
		for (int backward = S.size() - 1; backward >=0; backward--) {
			count0back[backward] = count0back[backward + 1];
			if (S[backward] == '0')
				count0back[backward]++;
		}
		int ans = S.size();
		int ones = 0;  // forward couting
		for (size_t i = 0; i < S.size(); i++) {
			if (S[i] == '1')
				ones++;
			ans = min(ans, ones + count0back[i] - 1);  // at each position, how many 1 before and zero after
		}  // don't doubt count at current position
		return ans;
	}
};

TEST_CASE("926, flip to increasing", "[NEW]")
{
	CHECK(StringStuff().minFlipsMonoIncr("00110") == 1);
	CHECK(StringStuff().minFlipsMonoIncr("00100110") == 2);
	CHECK(StringStuff().minFlipsMonoIncr("0011000") == 2);
}

TEST_CASE("820. Short Encoding of Words", "[NEW]")
{
	CHECK(StringStuff().minimumLengthEncoding(vector<string>{"time", "me", "bell"}) == 10);
}

TEST_CASE("20. Valid Parentheses", "[NEW]")
{
	CHECK(StringStuff().isValid("[") == false);
	CHECK(StringStuff().isValid("[](") == false);
}
TEST_CASE("443. String Compression", "[NEW]")
{
	CHECK(StringStuff().compress(vector<char>{ 'a' }) == 1);
	CHECK(StringStuff().compress(vector<char>{ 'a', 'b' }) == 2);
	CHECK(StringStuff().compress(vector<char>{ 'a', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b' }) == 4);
	CHECK(StringStuff().compress(vector<char>{ 'a', 'a', 'b', 'b', 'c', 'c', 'c' }) == 6);
}
TEST_CASE("686. Repeated String Match", "[NEW]")
{
	CHECK(StringStuff().repeatedStringMatch("abcd", "cdabcdab") == 3);
}
TEST_CASE("459. Repeated Substring Pattern", "[NEW]")
{
	CHECK(StringStuff().repeatedSubstringPattern("Aba") == false);
}

TEST_CASE("819. Most Common Word", "[NEW]")
{
	CHECK(StringStuff().mostCommonWord("a, a, a, a, b,b,b,c, c", vector<string>{ "a" }) == "b,b,b,c");
}

TEST_CASE("387. First Unique Character in a String", "[NEW]")
{
	CHECK(StringStuff().firstUniqChar_fast("aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz") == -1);
	CHECK(StringStuff().firstUniqChar_fast("") == -1);
	CHECK(StringStuff().firstUniqChar_fast("loveleetcode") == 2);
}
TEST_CASE("859. Buddy Strings", "[NEW]")
{
	CHECK(StringStuff().buddyStrings("aa", "aa") == true);
	CHECK(StringStuff().buddyStrings("ab", "ab") == false);
}
/*
	int strStr_KMP(string haystack, string needle) {
		for (int i = 0; i <= (int)(haystack.size() - needle.size()); i++) {
		}
		return -1;
	}
*/