cmake_minimum_required(VERSION 3.12.1)
project(jam VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(jam_source_files
			${CMAKE_CURRENT_LIST_DIR}/../../catch_main.cpp
			codejam1B3.cpp
			codejam1C1.cpp
)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
add_executable(jam ${jam_source_files})
target_include_directories(jam
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../
							)
install(TARGETS jam DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)