cmake_minimum_required(VERSION 3.12.1)
include("devtest/cmake/toolchain_find.cmake")
find_toolchain(0)

set (CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/leetcode/CompilerOptions.cmake")
project(all_cpp_projetcs VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(catch_source_files
			${CMAKE_CURRENT_LIST_DIR}/catch_main.cpp
)
add_library(catch_test STATIC ${catch_source_files})

target_include_directories(catch_test
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}
							)
install(TARGETS catch_test DESTINATION bin)

add_subdirectory(leetcode)
add_subdirectory(library)
#add_subdirectory(devtest)  make it a stand alone cmake project to test install, lib merging, and code generation script