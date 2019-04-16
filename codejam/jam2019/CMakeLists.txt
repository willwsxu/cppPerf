cmake_minimum_required(VERSION 3.12.1)
project(jam)
set(jam_source_files
			qualify2019.cpp
			codejam1A.cpp
)
add_executable(jam ${jam_source_files})
target_include_directories(jam
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../)