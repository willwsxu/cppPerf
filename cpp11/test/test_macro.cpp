
#define TEST1_1 1000
#define TEST2_1 2000

#define MACRO1(p1, p2) \
    p1 << TEST1_##p2

#define MACRO2(p1, p2) \
    p1 << TEST2_##p2

#define MACRO_COND(choose1, p1, p2) \
    ((choose1) ? MACRO1(p1,p2) : MACRO2(p1,p2))

#define CONCAT(X, Y)  X##Y
#define MACRO_HELP(p1, tag, p2) \
    p1 << CONCAT(tag, p2)

#define MACRO_COND2(choose1, p1, p2) \
    MACRO_HELP(p1, ((choose1) ? TEST1_ : TEST2_), p2)

// https://gcc.gnu.org/onlinedocs/gcc-3.4.3/cpp/Stringification.html
#define TEST_STRINGIFY(ostr, STR) \
    ostr << #STR << "=" << STR

#include <sstream>
#include <iostream>
bool choose(bool a) {
    return a;
}
void test_macro()
{
    std::ostringstream ostr;
    MACRO_COND(choose(true), ostr, 1) << " test 1";
    std::cout << ostr.str() << "\n";

    std::ostringstream ostr2;
    MACRO_COND(choose(false), ostr2, 1) << " test 2";
    std::cout << ostr2.str() << "\n";

    std::ostringstream ostr3;
    //INVALID
    //MACRO_COND2(true, ostr3, 1) << " TEST 3";
    TEST_STRINGIFY(ostr3, TEST1_1);
    std::cout << ostr3.str() << "\n";
}

typedef enum{
    LOG_MIN_LEVEL=0,
    LOG_INFO=1,
    LOG_INFO_LOCAL = 2,
    LOG_INFO_SEARCH = 3,
    LOG_WARN=4,
    LOG_MAX_LEVEL = 5,
} LOG_LEVEL;
struct Logger {
    std::ostringstream& Stream() { return ostr; }
    Logger(LOG_LEVEL level, int id, const char *fn, int ln) : m_level(level_str(level)), m_id(id), m_file(fn), m_line(ln) {}
    ~Logger()
    {
        std::cout << "(" << m_level << "," << m_id << ")" << ostr.str() << " at " << m_file << ":" << m_line << "\n";
    }
private:
    const char* level_str(LOG_LEVEL l) {
        static const char* str[] = {"", "INFO", "LOCAL", "SEARCH", "WARN"};
        return str[l];
    }
    const char * m_level;
    int m_id;
    int m_line;
    const char* m_file;
    std::ostringstream ostr;
};

// A little trick from Google's log implementation which changes the return
// type of Logger::Stream() to void. The '&' operator is used because it has
// lower precedence than <<.
class VoidifyStream
{
public:
    void operator&(std::ostream&) const {}
};

#define TTLOG_S_BASE(_level, _id, _src) \
    ( (LOG_##_level) <= LOG_MIN_LEVEL || \
      (LOG_##_level) >= LOG_MAX_LEVEL ) ? (void)0 : \
    VoidifyStream() & \
    Logger((LOG_##_level), (_id), __FILE__, __LINE__).Stream()

#define TTLOG(_level, _id) \
    TTLOG_S_BASE(_level, _id, _id)

#define TTLOG_BLACKLIST_ON(_id)    TTLOG(INFO_LOCAL, _id)
#define TTLOG_BLACKLIST_OFF(_id)    TTLOG(INFO_SEARCH, _id)

// Ternary op ?: requires same type of both expression, that's why we need VoidifyStream in line 79
// require () over the whole expression, ( ? e1 : e2 ), to make e1 and e2 work, see line 12 and 95
// when on=false, << works, 
// when on=true, << does work
// We cannot add () to line 94 because expression would evaluate to void due to VoidifyStream
// VoidifyStream() & Logger(..).Stream() << "WORKS"
// (VoidifyStream() & Logger(..).Stream()) << "DOES NOT WORK"
#define TTLOG_BLACKLIST(on, _id) \
    (on) ? TTLOG_BLACKLIST_ON(_id) : TTLOG_BLACKLIST_OFF(_id)

#define TTLOG_BLACKLIST2(on, _id) \
    ((on) ?  \
    Logger(LOG_INFO_LOCAL, (_id), __FILE__, __LINE__).Stream() : \
    Logger(LOG_INFO_SEARCH, (_id), __FILE__, __LINE__).Stream())

bool is_blacklist_on()
{
    return true;
}

#define special_case(f1,f2) \
    f1##f2
#include "..\catch.hpp"
TEST_CASE("test_macro", "[MACRO]")
{
    test_macro();

    TTLOG_S_BASE(INFO_SEARCH, 9999999, 0) << " base test";
    TTLOG_BLACKLIST((!is_blacklist_on()), 9999999) << " BLACKLIST OFF";
    TTLOG_BLACKLIST((is_blacklist_on()), 9999999) << " BLACKLIST ON";

    TTLOG_BLACKLIST2((is_blacklist_on()), 999999) << " BLACKLIST2 ON";
    TTLOG_BLACKLIST2((!is_blacklist_on()), 999999) << " BLACKLIST2 OFF";

    std::string x("test MACRO special char and empty param");
    CHECK(special_case(x, .c_str()) == x);
    CHECK(special_case(x, ) == x);
}