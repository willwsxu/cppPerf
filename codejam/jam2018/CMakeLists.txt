cmake_minimum_required(VERSION 3.12.1)
project(jam)
Message("${CMAKE_CURRENT_LIST_DIR}")
set(jam_source_files
			jam1B.cpp
			wafflechoppercpp.cpp
)
add_executable(jam ${jam_source_files})
target_include_directories(jam
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../)