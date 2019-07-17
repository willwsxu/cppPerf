cmake_minimum_required(VERSION 3.12.1)
project(libcombo_dll VERSION "1.0" LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

IF(MSVC)
#	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
ENDIF()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(libcombo_dll SHARED dummy.cpp)

#add heirarchy to header file, maintain consistency with installed folder structure
FILE(COPY ${CMAKE_CURRENT_LIST_DIR}/dummy.h
	DESTINATION ${CMAKE_BINARY_DIR}/include/dummy
	FILES_MATCHING PATTERN *.h )

target_include_directories(libcombo_dll
    PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/${INSTALL_INCLUDEDIR}>  #contain all necessary headers
    INTERFACE $<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>  #include for installed package
)
target_link_libraries(libcombo_dll 
	PRIVATE liba
	PRIVATE libb
	)
target_compile_definitions(libcombo_dll PUBLIC CMAKE_DLL) #CMAKE_DLL is used for none CMAKE build
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

set(EXPORT_HEADER ${CMAKE_BINARY_DIR}/${INSTALL_INCLUDEDIR}/dummy/libcombo_dll_export.h)
message(STATUS "EXPORT_HEADER=${EXPORT_HEADER}")
set_target_properties(libcombo_dll PROPERTIES 
	POSITION_INDEPENDENT_CODE 1
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
	DEBUG_POSTFIX "-d"
    OUTPUT_NAME "libcombo"
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
	PUBLIC_HEADER "dummy.h;${EXPORT_HEADER}"  #header files to install, destination is specified in install target below
)

#export header file is generated at CMAKE time, only needed for for dll target
include(GenerateExportHeader)
generate_export_header(libcombo_dll
    BASE_NAME "libcombo"
    EXPORT_MACRO_NAME "DLL_IMPORT_EXPORT"
    EXPORT_FILE_NAME "${EXPORT_HEADER}"
	STATIC_DEFINE "DUMMY_STATIC_DEFINE"  #use this to indicate building static lib
)


add_library(libcombo_static STATIC dummy.cpp)
target_compile_definitions(libcombo_static PUBLIC DUMMY_STATIC_DEFINE)

target_include_directories(libcombo_static
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>  # copied all necessary headers here
    INTERFACE $<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>  #include for installed package
)
target_link_libraries(libcombo_static 
	PRIVATE liba
	PRIVATE libb
	)
set_target_properties(libcombo_static PROPERTIES 
	POSITION_INDEPENDENT_CODE 1
	DEBUG_POSTFIX "-sd"
	RELEASE_POSTFIX "-s"
    ARCHIVE_OUTPUT_NAME "libcombo_dummy"
)

#SUMMARY
#export header is generated at cmake config time
#use FILE copy to structure header file heirarchy
#all commands above are for building targets, except INSTALL_INTERFACE and PUBLIC_HEADER setting
#below is installed commands
#install lib and headers. depend on PUBLIC_HEADER target property
# libcombo_static depdends on liba and libb. libcombo_dll can be installed alone as it is linked with liba and libb
install(TARGETS  liba libb libcombo_static libcombo_dll
	EXPORT DummyTargets  #referred later
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}  #relative to CMAKE_INSTALL_PREFIX
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDEDIR}/dummy
)

install(EXPORT DummyTargets
	NAMESPACE DUMMY_NS::
	DESTINATION ${INSTALL_CMAKEDIR}
)
message(STATUS "PROJECT_VERSION=${PROJECT_VERSION}")
#generate cmake files to find package and version
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/dummyConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
	${CMAKE_SOURCE_DIR}/cmake/DummyConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/DummyConfig.cmake
	INSTALL_DESTINATION ${INSTALL_CMAKEDIR}
)
install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/DummyConfig.cmake
		${CMAKE_CURRENT_BINARY_DIR}/dummyConfigVersion.cmake
	DESTINATION
		${INSTALL_CMAKEDIR}
)

#PUBLIC_HEADER can only specifify one dest dir, not suited for heirarchy
install(
	DIRECTORY
		${CMAKE_BINARY_DIR}/include/liba
		${CMAKE_BINARY_DIR}/include/libb
	DESTINATION
		${INSTALL_INCLUDEDIR}
)