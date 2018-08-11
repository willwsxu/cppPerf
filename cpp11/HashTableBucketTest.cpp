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
	auto ins1=t.insert("test");
	CHECK(ins1.second);
	CHECK(*ins1.first=="test");
	ins1 = t.insert("test");
	CHECK(ins1.second==false);
	CHECK(*ins1.first == "test");

	string *t2 = new string{ "test2" };
	auto ins2=t.insert(t2);
	CHECK(*ins2.first == "test2");
	CHECK(t.size() == 2);
	auto f = t.find("test");
	CHECK(f != t.end());
	CHECK(*f == "test");
	t.erase(*t2);
	CHECK(t.size() == 1);

	// test iterators
	auto first = t.begin();
	auto last = t.end();
	CHECK(*first == "test");
	CHECK(last != first);
	CHECK(last == ++first);
	first = t.begin();
	CHECK(last != first++);
	CHECK(last == first);
}