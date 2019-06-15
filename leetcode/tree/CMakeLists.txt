cmake_minimum_required(VERSION 3.12.1)
#set (CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/../CompilerOptions.cmake")
project(leetcode VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(leetcode_source_files
			BinaryTree.cpp
			binary_tree.cpp
)
add_executable(leetcode ${leetcode_source_files})
#must apprear after target is created such as add_executable
#PRE_BUILD did not work as expected: not done before build, not triggered by build
add_custom_command(TARGET leetcode 
	PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_LIST_DIR}/../pre_built_command.cmake"
)
add_custom_target(pre_build_target 
	COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_LIST_DIR}/../pre_built_command2.cmake"
)
add_dependencies(leetcode pre_build_target)

target_include_directories(leetcode
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}
							)
target_link_libraries(leetcode 
		PRIVATE catch_test
		PRIVATE leetcode_common)
install(TARGETS leetcode DESTINATION bin)