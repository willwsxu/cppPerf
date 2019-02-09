
#include "..\catch.hpp"

// large malloc
// why virtual memory is slow
// difference of process vs thread
TEST_CASE("Jump memory test", "[TEST]")
{
#ifdef _WIN64
	size_t s = 32000000000LL;
	auto *large = malloc(s);
	CHECK(large == 0);
#endif
}