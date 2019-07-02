cmake_minimum_required(VERSION 3.12.1)
project(test_dll LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS date_time system)
add_executable(test_dll main.cpp)
target_link_libraries(test_dll 
	PRIVATE libcombo_dll
	PRIVATE Boost::boost 
	PRIVATE Boost::date_time
)
# libcombo_dll took care of all include dependecy
#target_include_directories(test_dll PRIVATE ${CMAKE_BINARY_DIR}/include)

# OUTPUT_DIRECTORY in root folder should work if there are placed ahead of adding this target
set_target_properties(test_dll
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_executable(test_dummy_s main.cpp)
target_link_libraries(test_dummy_s 
	PRIVATE libcombo_static
	PRIVATE Boost::boost 
	PRIVATE Boost::date_time
)