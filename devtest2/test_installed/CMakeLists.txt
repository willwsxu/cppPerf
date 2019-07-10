cmake_minimum_required(VERSION 3.12.1)
project(test_installed LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CPP_PERF_DIR ${CMAKE_SOURCE_DIR}/..)
set(TEST_SOURCE_FILES ${CPP_PERF_DIR}/devtest/test_dll/main.cpp)

message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
#instruct find_package where to find Dummy, Capitalize D is necessary. Or set CMAKE_PREFIX_PATH
set(Dummy_DIR "C:\\dev\\cppPerf\\devtest\\build\\x64-Debug\\installed\\cmake")
find_package(Dummy 1 REQUIRED)  #package name is case sensitive
find_package(Boost REQUIRED COMPONENTS date_time system)
add_executable(test_installed ${TEST_SOURCE_FILES})
target_link_libraries(test_installed 
	PRIVATE DUMMY_NS::libcombo_dll
	PRIVATE Boost::boost 
	PRIVATE Boost::date_time
)

add_executable(test_installed_s ${TEST_SOURCE_FILES})
target_link_libraries(test_installed_s 
	PRIVATE DUMMY_NS::libcombo_static
	PRIVATE Boost::boost 
	PRIVATE Boost::date_time
)