#include "MessageConcept.h"

#include "..\catch.hpp"

TEST_CASE("message concept", "NEW")
{
	{
		request<string_body> req;
		write(cout, req);
	}
	{
		response<vector_body<char>> res;
		write(cout, res);
	}
	{
		response<list_body<string>> res;
		write(cout, res);
	}
	{
		response<empty_body> res;
		write(cout, res);
	}
}
