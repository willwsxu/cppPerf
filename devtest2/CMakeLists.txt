cmake_minimum_required(VERSION 3.12.1)
include("../devtest/cmake/toolchain_find.cmake")
project(cmake_demo2 VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_toolchain()  #needed on vc2017

add_subdirectory(test_installed)