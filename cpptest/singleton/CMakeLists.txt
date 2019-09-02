cmake_minimum_required(VERSION 3.12.1)

project(singleton_test VERSION "1.0" LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(singleton_dll SHARED singleton.cpp)
target_include_directories(singleton_dll
    PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/${INSTALL_INCLUDEDIR}/export>
)

set(EXPORT_HEADER ${CMAKE_BINARY_DIR}/${INSTALL_INCLUDEDIR}/export/singleton_dll_export.h)
message(STATUS "EXPORT_HEADER=${EXPORT_HEADER}")
set_target_properties(singleton_dll PROPERTIES 
	POSITION_INDEPENDENT_CODE 1
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
	DEBUG_POSTFIX "-d"
    OUTPUT_NAME "singleton"
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
	PUBLIC_HEADER "singleton.h;${EXPORT_HEADER}"  #header files to install, destination is specified in install target below
)

#export header file is generated at CMAKE time, only needed for for dll target
include(GenerateExportHeader)
generate_export_header(singleton_dll
    BASE_NAME "singleton"
    EXPORT_MACRO_NAME "DLL_IMPORT_EXPORT"
    EXPORT_FILE_NAME "${EXPORT_HEADER}"
	STATIC_DEFINE "SINGLETON_STATIC_DEFINE"  #use this to indicate building static lib
)

add_library(singleton_static STATIC static_lib.cpp)
target_include_directories(singleton_static
    PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/${INSTALL_INCLUDEDIR}/export>
)
add_executable(singleton_test main.cpp)
target_link_libraries(singleton_test 
	PRIVATE singleton_static
	PRIVATE singleton_dll
	)