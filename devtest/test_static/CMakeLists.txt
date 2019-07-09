cmake_minimum_required(VERSION 3.12.1)
project(libtest LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Boost REQUIRED COMPONENTS date_time system)

add_executable(libtest main.cpp)
target_link_libraries(libtest
	PRIVATE lib_combo_s
)
#target_include_directories(libtest PRIVATE ${CMAKE_BINARY_DIR}/include)

add_executable(libtest2 main.cpp)
target_link_libraries(libtest2 
	PRIVATE lib_combo_static2
	PRIVATE Boost::boost
	PRIVATE Boost::date_time
)
