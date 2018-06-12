#include "..\catch.hpp"

#include "string"
#include "ConsoleLogger.h"
#include "HashTableMockImpl.h"
#include "HashTableFileRead.h" //"HashTableFileReadSimple.h"
#include "FileReadMock.h"
#include "HashTable.h"

using namespace std;

using HashTableMock = HASH_TABLE_NEW<Console, string, HASH_TABLE_Mock, FileReadMock>;


TEST_CASE("Hash table Mock Test", "[MOCK]")
{
	HashTableMock t(10, "testHash.txt");

	CHECK(t.size() == 6);
	CHECK(t.insert(string("test3")) == false);  // already in the file
	CHECK(t.insert(string("test1")) == true);
	CHECK(t.find("test2") == nullptr);
	CHECK(t.insert(string("test2")) == true);
	CHECK(t.find("test2") != nullptr);
	CHECK(t.insert("test2") == false);
	CHECK(t.size() == 8);
	t.WriteHashTable();
}