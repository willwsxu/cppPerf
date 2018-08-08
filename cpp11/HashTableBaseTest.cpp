#include "..\catch.hpp"

#include "string"
#include "ConsoleLogger.h"
#include "HashTableBase.h"
#include "HashTable.h"

using namespace std;

using HashTableMock = HASH_TABLE_NEW<Console, string, FH_HASH_TABLE_BASE, FileNone>;