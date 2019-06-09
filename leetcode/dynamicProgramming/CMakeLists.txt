cmake_minimum_required(VERSION 3.12.1)
project(leetcode VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(leetcode_source_files
			${CMAKE_CURRENT_LIST_DIR}/../../catch_main.cpp
			triangulation.cpp
			longest_chain.cpp
			lastStone.cpp
)
add_executable(leetcode ${leetcode_source_files})
target_compile_options(leetcode PRIVATE $<$<CONFIG:Debug>:/ZI> $<$<CONFIG:Release>:/Zi>)
target_link_options(leetcode PRIVATE $<$<CONFIG:Release>:/DEBUG /OPT:REF /OPT:ICF>)
target_include_directories(leetcode
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../
							)
install(TARGETS leetcode DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)