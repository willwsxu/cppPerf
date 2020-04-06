cmake_minimum_required(VERSION 3.12.1)
include("devtest/cmake/toolchain_find.cmake")
find_toolchain(0)

#use export to set compiler
#export CC=/usr/bin/gcc
#export CXX=/usr/bin/g++
add_compile_definitions($<$<CONFIG:Debug>:_DEBUG>)

include(GNUInstallDirs) #needed for CMAKE_INSTALL_XXX def
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "you may change installation include dir here")
message(STATUS "INSTALL_INCLUDEDIR=${INSTALL_INCLUDEDIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

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
add_subdirectory(cppcon)
add_subdirectory(cpp11)
add_subdirectory(cpp11/test)
add_subdirectory(library)
add_subdirectory(cpp11/threading_model)
add_subdirectory(profiling)

#add_subdirectory(devtest)  make it a stand alone cmake project to test install, lib merging, and code generation script