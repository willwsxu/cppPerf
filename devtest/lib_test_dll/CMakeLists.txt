cmake_minimum_required(VERSION 3.12.1)
project(libtest_dll LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS date_time system)
add_executable(libtest_dll main.cpp)
target_link_libraries(libtest_dll 
	INTERFACE libcombo_dll  #cannot use PRIVATE
	PRIVATE Boost::boost 
	PRIVATE Boost::date_time
)
target_include_directories(libtest_dll PRIVATE ${CMAKE_BINARY_DIR}/include)