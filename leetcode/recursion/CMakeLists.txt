cmake_minimum_required(VERSION 3.12.1)
project(leetcode_recur VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(leetcode_source_files
			brace_expression.cpp
)
add_executable(leetcode_recur ${leetcode_source_files})
target_link_libraries(leetcode_recur 
		PRIVATE catch_test
#		PRIVATE leetcode_common
		)