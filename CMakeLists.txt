cmake_minimum_required(VERSION 3.12.1)
set(CMAKE_TOOLCHAIN_FILE "c:\\dev\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" CACHE STRING "")
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
add_subdirectory(devtest)