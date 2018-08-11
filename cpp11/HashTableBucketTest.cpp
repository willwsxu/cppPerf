#include "..\catch.hpp"

#include "string"
#include "ConsoleLogger.h"
#include "HashTableBucketHashing.h"
#include "HashTable.h"

using namespace std;

using HashTableMock = HASH_TABLE_NEW<Console, string, HASH_TABLE_BUCKET, FileNone>;

TEST_CASE("Hash table Mock Test", "[MOCK]")
{
	HashTableMock t(10);
	t.insert("test");
	string *t2 = new string{ "test2" };
	t.insert(t2);
	t.find("test");
}