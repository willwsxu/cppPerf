cmake_minimum_required(VERSION 3.12.1)
project(proto_msg LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

IF(MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
ENDIF()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(libcombo_dll SHARED dummy.cpp)
include(GenerateExportHeader)
generate_export_header(libcombo_dll)

target_link_libraries(libcombo_dll 
	PRIVATE liba
	PRIVATE libb
	)
if (WIN32)
	set(DLL_LINK_FLAGS "/WHOLEARCHIVE")
elseif(APPLE)
	set(DLL_LINK_FLAGS "-wl,-all_load")
else()
	set(DLL_LINK_FLAGS "-wl,--whole-archive")
endif()
set_target_properties(libcombo_dll PROPERTIES LINK_FLAGS ${DLL_LINK_FLAGS})
if(WIN32)
set_target_properties(libcombo_dll PROPERTIES LINK_FLAGS "/FORCE:MULTIPLE")
endif()
target_include_directories(libcombo_dll INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)