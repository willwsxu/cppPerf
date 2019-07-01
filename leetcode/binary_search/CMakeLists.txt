cmake_minimum_required(VERSION 3.12.1)
project(leetcode_bs VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(leetcode_source_files
			MountainArray.cpp
			BinarySearch.cpp
			sliding_window.cpp
)
add_executable(leetcode_bs ${leetcode_source_files})
target_link_libraries(leetcode_bs 
		PRIVATE catch_test
		PRIVATE leetcode_common
		)