#include "..\catch.hpp"

#include "string"
#include "ConsoleLogger.h"
#include "HashTable.h"
#include "HashTableBase.h"
#include "HashTableFileRead.h" // hash table reader based on c style tokenizer
#include "FileReadMock.h"

using namespace std;

using HashTableMock = HASH_TABLE_NEW<Console, string, FH_HASH_TABLE_BASE, FileReadMock>;