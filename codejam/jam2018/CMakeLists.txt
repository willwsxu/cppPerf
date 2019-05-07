cmake_minimum_required(VERSION 3.12.1)
set(CMAKE_VERBOSE_MAKEFILE on)
project(jam)
Message("${CMAKE_CURRENT_LIST_DIR}")
set(jam_source_files
			${CMAKE_CURRENT_LIST_DIR}/../../catch_main.cpp
			jam1B.cpp
			wafflechoppercpp.cpp
)
add_executable(jam ${jam_source_files})
target_include_directories(jam
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../..
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../)
install(TARGETS jam DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)
