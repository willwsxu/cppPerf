cmake_minimum_required(VERSION 3.12.1)
project(threading_model VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(threading_source_files
			main.cpp
)
add_executable(threading_test ${threading_source_files})
target_link_libraries(threading_test 
		PRIVATE catch_test
		)