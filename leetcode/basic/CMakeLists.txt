cmake_minimum_required(VERSION 3.12.1)
project(leetcode_math VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(leetcode_source_files
			math.cpp
			associative.cpp
)
add_executable(leetcode_math ${leetcode_source_files})
target_link_libraries(leetcode_math 
		PRIVATE catch_test
#		PRIVATE leetcode_common
		)