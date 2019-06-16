cmake_minimum_required(VERSION 3.12.1)
project(leetcode_stack VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(leetcode_source_files
			stack.cpp
			stack_easy.cpp
)
add_executable(leetcode_stack ${leetcode_source_files})

target_include_directories(leetcode_stack
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}
							)
target_link_libraries(leetcode_stack 
		PRIVATE catch_test
		PRIVATE leetcode_common)
#install(TARGETS leetcode_stack DESTINATION bin)