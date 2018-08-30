#include "..\catch.hpp"

#include "string"
#include "ConsoleLogger.h"
#include "HashTable.h"
#include "HashTableBucketHashing.h"
#include "HashTableProbing.h"

using namespace std;

using HashTableMock = HASH_TABLE_NEW<Console, string, HASH_TABLE_BUCKET, FileNone>;

TEST_CASE("Bucket Hashing table Test", "[NEW]")
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
	CHECK(t.erase(*t2)==1);
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


using HashTableProbe = HASH_TABLE_NEW<Console, string, HASH_TABLE_PROBING, FileNone>;

TEST_CASE("probing Hashing table Test", "[NEW]")
{
	HashTableProbe t(10);
	auto ins1 = t.insert("test");  // test insert
	CHECK(ins1.second);
	CHECK(*ins1.first == "test");
	ins1 = t.insert("test");
	CHECK(ins1.second == false);
	CHECK(*ins1.first == "test");

	auto found = t.find("test2");  // test find
	CHECK(found == t.end());
	found = t.find("test");
	CHECK(*found == "test");

	CHECK(t.erase("test2") == 0);  // test erase
	CHECK(t.erase("test") == 1);

	CHECK(t.size() == 0);
	ins1 = t.insert("test");
	ins1 = t.insert("test2");
	ins1 = t.insert("test3");
	ins1 = t.insert("test3");
	ins1 = t.insert("test4");
	ins1 = t.insert("test5");
	ins1 = t.insert("test6");
	ins1 = t.insert("test7");
	ins1 = t.insert("test8");
	ins1 = t.insert("test9");
	CHECK(t.size() == 9);
	CHECK(*t.find("test9") == "test9");
}