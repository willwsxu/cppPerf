cmake_minimum_required(VERSION 3.12.1)
project(leetcode_bfs VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(leetcode_source_files
			grid_path.cpp
)
add_executable(leetcode_bfs ${leetcode_source_files})
target_link_libraries(leetcode_bfs 
		PRIVATE catch_test
#		PRIVATE leetcode_common
		)