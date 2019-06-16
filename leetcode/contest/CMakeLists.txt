cmake_minimum_required(VERSION 3.12.1)
project(contest VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(leetcode_source_files
			contest.cpp
)
add_executable(contest ${leetcode_source_files})
target_link_libraries(contest 
		PRIVATE catch_test
#		PRIVATE leetcode_common
		)