cmake_minimum_required(VERSION 3.12.1)
project(profiling VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(profiling_source_files
			main.cpp
)
set(app_name profile_mem)
add_executable(${app_name} ${profiling_source_files})
target_include_directories(${app_name} 
	PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/cpp11/threading_model/>
	)
target_link_libraries(${app_name} 
		PRIVATE catch_test
		)