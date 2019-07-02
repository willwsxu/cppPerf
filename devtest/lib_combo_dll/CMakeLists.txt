cmake_minimum_required(VERSION 3.12.1)
project(libcombo_dll LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

IF(MSVC)
#	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
ENDIF()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(libcombo_dll SHARED dummy.cpp)

target_include_directories(libcombo_dll
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>  # copied headers from liba libb
    PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/${INSTALL_INCLUDEDIR}>
)
target_link_libraries(libcombo_dll 
	PRIVATE liba
	PRIVATE libb
	)
target_compile_definitions(libcombo_dll PUBLIC CMAKE_DLL)
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

set(EXPORT_HEADER ${CMAKE_BINARY_DIR}/${INSTALL_INCLUDEDIR}/libcombo_dll_export.h)
message(STATUS "EXPORT_HEADER=${EXPORT_HEADER}")
set_target_properties(libcombo_dll PROPERTIES 
	POSITION_INDEPENDENT_CODE 1
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
	DEBUG_POSTFIX "-d"
    OUTPUT_NAME "libcombo"
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
	PUBLIC_HEADER "dummy.h;${EXPORT_HEADER}"
)

include(GenerateExportHeader)
generate_export_header(libcombo_dll
    BASE_NAME "libcombo"
    EXPORT_MACRO_NAME "DLL_IMPORT_EXPORT"
    EXPORT_FILE_NAME "${EXPORT_HEADER}"
)


add_library(libcombo_static STATIC dummy.cpp)

target_include_directories(libcombo_static
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>  # copied headers from liba libb
)
target_link_libraries(libcombo_static 
	PRIVATE liba
	PRIVATE libb
	)
set_target_properties(libcombo_static PROPERTIES 
	POSITION_INDEPENDENT_CODE 1
	DEBUG_POSTFIX "-sd"
	RELEASE_POSTFIX "-s"
    ARCHIVE_OUTPUT_NAME "libcombo"
)
