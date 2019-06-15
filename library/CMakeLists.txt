cmake_minimum_required(VERSION 3.12.1)

project(library VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(library_include_DIR ${CMAKE_CURRENT_LIST_DIR})
add_library(library INTERFACE)
target_compile_features(library INTERFACE cxx_std_11)
target_include_directories(library
							INTERFACE $<BUILD_INTERFACE:${library_include_DIR}>
							)
