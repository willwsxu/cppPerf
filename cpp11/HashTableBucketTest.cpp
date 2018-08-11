#include "..\catch.hpp"

#include "string"
#include "ConsoleLogger.h"
#include "HashTableBucketHashing.h"
#include "HashTable.h"

using namespace std;

using HashTableMock = HASH_TABLE_NEW<Console, string, HASH_TABLE_BUCKET, FileNone>;

TEST_CASE("Backet Hashing table Test", "[NEW]")
{
	HashTableMock t(10);
	t.insert("test");
	string *t2 = new string{ "test2" };
	t.insert(t2);
	CHECK(t.size() == 2);
	auto f = t.find("test");
	CHECK(f != nullptr);
	t.erase(*t2);
	CHECK(t.size() == 1);
}