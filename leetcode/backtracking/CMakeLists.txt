cmake_minimum_required(VERSION 3.12.1)
project(leetcode_back VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(leetcode_source_files
			backtracking.cpp
)
add_executable(leetcode_back ${leetcode_source_files})

target_include_directories(leetcode_back
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}
							)
target_link_libraries(leetcode_back 
		PRIVATE catch_test
		PRIVATE leetcode_common)
install(TARGETS leetcode_back DESTINATION bin)