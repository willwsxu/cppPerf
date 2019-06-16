cmake_minimum_required(VERSION 3.12.1)

project(leetcode_common VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_library(leetcode_common INTERFACE)

target_include_directories(leetcode_common
							INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
							)

target_link_libraries(leetcode_common 
		INTERFACE library)

add_subdirectory(tree)
add_subdirectory(dynamicProgramming)
add_subdirectory(stack)
add_subdirectory(backtracking)
add_subdirectory(contest)
add_subdirectory(bfs)
add_subdirectory(dfs)
add_subdirectory(basic)