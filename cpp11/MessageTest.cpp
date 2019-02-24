#include "MessageConcept.h"

#include "..\catch.hpp"

TEST_CASE("message concept", "[MSG]")
{
	{
		request<string_body> req;
		ostringstream oss;
		write(oss, req);
		CHECK(oss.str() == "");
	}
	{
		response<vector_body<char>> res;
		ostringstream oss;
		write(oss, res);
		CHECK(oss.str() == "");
	}
	{
		response<list_body<string>> res;
		ostringstream oss;
		write(oss, res);
		CHECK(oss.str() == "");
	}
	{
		response<empty_body> res;
		ostringstream oss;
		write(oss, res);
		CHECK(oss.str() == "");
	}
}
