cmake_minimum_required(VERSION 3.12.1)
project(cpp11Examples VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
set(cpp11_source_files
    ../InterviewBook.cpp
    ../practice.cpp
)


add_executable(cpp11Examples ${cpp11_source_files})
target_include_directories(cpp11Examples 
	PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/cpp11>
	PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/library>
	)
target_link_libraries(cpp11Examples 
		PRIVATE catch_test
		PRIVATE Boost::boost
		)