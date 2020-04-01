cmake_minimum_required(VERSION 3.12.1)
project(cpp11test VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
set(cpp11_source_files
    simplified_alloc.cpp
    thread_loccal.cpp
    test_variant.cpp
    member_access.cpp
    test_macro.cpp
)


add_executable(cpp11test ${cpp11_source_files})
target_include_directories(cpp11test 
	PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/cpp11>
	)
target_link_libraries(cpp11test 
		PRIVATE catch_test
		PRIVATE Boost::boost
		)