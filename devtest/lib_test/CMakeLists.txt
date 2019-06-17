cmake_minimum_required(VERSION 3.12.1)
project(libtest LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(libtest main.cpp)
target_link_libraries(libtest liba libb)