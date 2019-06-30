cmake_minimum_required(VERSION 3.12.1)
project(leetcode_dp VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(leetcode_source_files
			triangulation.cpp
			longest_chain.cpp
			knapsack.cpp
			edit_distance.cpp
			coin_change.cpp
			prefix-suffix.cpp
)
add_executable(leetcode_dp ${leetcode_source_files})
target_link_libraries(leetcode_dp 
		PRIVATE catch_test
#		PRIVATE leetcode_common
		)
install(TARGETS leetcode_dp DESTINATION bin)