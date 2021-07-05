cmake_minimum_required(VERSION 3.12.1)
project(jam2020 VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
set(jam2020_source_files
    qualify2020.cpp
)


add_executable(jam2020 ${jam2020_source_files})
target_include_directories(jam2020 
	PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/jam2020>
	)
target_link_libraries(jam2020 
		PRIVATE catch_test
		PRIVATE Boost::boost
		)