cmake_minimum_required(VERSION 3.12.1)
#include("cmake/toolchain_find.cmake")
project(cpptest VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)

#find_toolchain()  #needed on vc2017
Message(STATUS "CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}. set it in env or we will try to find it")

# for these settings to apply to all projects, must place them here, before targets are created
include(GNUInstallDirs) #needed for CMAKE_INSTALL_XXX def
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
message(STATUS "make output=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}, ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

add_subdirectory(singleton)
