cmake_minimum_required(VERSION 3.12.1)
project(cpp11 VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
set(cpp11_source_files
	FactoryTest.cpp
	FunctorTest.cpp
	HashMockTest.cpp
	HashTableBaseFileTest.cpp
	HashTableBaseTest.cpp
	HashTableBucketTest.cpp
    main.cpp
	MessageTest.cpp
	meta_test.cpp
	SeanParentTest.cpp
	testLargetInt.cpp
	TestslistRingBuffer.cpp
)


add_executable(cpp11 ${cpp11_source_files})
target_include_directories(cpp11
	PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/cpp11>
	PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/library>
	)
target_link_libraries(cpp11
		PRIVATE catch_test
		PRIVATE Boost::boost
		)