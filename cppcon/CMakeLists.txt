cmake_minimum_required(VERSION 3.12.1)
project(cppcon VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(cppcon_source_files
			philosophers.cpp
			TaskWaiting.cpp
)
add_executable(concurrency ${cppcon_source_files})
target_include_directories(concurrency 
	PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/cpp11/threading_model/>
	)
target_link_libraries(concurrency 
		PRIVATE catch_test
		)