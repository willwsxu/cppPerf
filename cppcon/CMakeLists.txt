cmake_minimum_required(VERSION 3.12.1)
project(cppcon VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(cppcon_source_files
			TaskWaiting.cpp
)
add_executable(concurrency ${cppcon_source_files})
target_link_libraries(concurrency 
		PRIVATE catch_test
		)