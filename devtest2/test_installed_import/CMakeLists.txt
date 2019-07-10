cmake_minimum_required(VERSION 3.12.1)
project(test_installed_import LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CPP_PERF_DIR ${CMAKE_SOURCE_DIR}/..)
set(TEST_SOURCE_FILES ${CPP_PERF_DIR}/devtest/test_static/main.cpp)

message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
#instruct find_package where to find Dummy, Capitalize D is necessary. Or set CMAKE_PREFIX_PATH
set(Implib_DIR "C:\\dev\\cppPerf\\devtest\\build\\x64-Debug\\installed\\cmake")
find_package(Implib REQUIRED)  #package name is case sensitive
find_package(Boost REQUIRED COMPONENTS date_time system)
add_executable(test_installed_import ${TEST_SOURCE_FILES})
target_link_libraries(test_installed_import 
	PRIVATE Implib_NS::lib_combo_s_interface
	PRIVATE Boost::boost 
	PRIVATE Boost::date_time
)
