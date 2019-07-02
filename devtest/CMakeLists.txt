cmake_minimum_required(VERSION 3.12.1)
include("cmake/toolchain_find.cmake")
project(cmake_demo VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_toolchain()  #needed on vc2017
Message(STATUS "CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}. set it in env or we will try to find it")

# for these settings to apply to all projects, must place them here, before targets are created
include(GNUInstallDirs) #needed for CMAKE_INSTALL_XXX def
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
message(STATUS "make output=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}, ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

set(INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR} CACHE PATH "you may change installation lib dir here")
set(INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR} CACHE PATH "you may change installation bin dir here")
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "you may change installation include dir here")
if(WIN32)
    set(DEF_INSTALL_CMAKEDIR CMake)
else()
    set(DEF_INSTALL_CMAKEDIR share/cmake/${PROJECT_NAME})
endif()
set(INSTALL_CMAKEDIR ${DEF_INSTALL_CMAKEDIR} CACHE PATH "you may change installation cmake dir here")
message(STATUS "INSTALL_LIBDIR=${INSTALL_LIBDIR}")
message(STATUS "INSTALL_BINDIR=${INSTALL_BINDIR}")
message(STATUS "INSTALL_INCLUDEDIR=${INSTALL_INCLUDEDIR}")
message(STATUS "INSTALL_CMAKEDIR=${INSTALL_CMAKEDIR}")

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/installed)  #better pass in as CMAKE varibale
#report where components will be installed
foreach(p LIB BIN INCLUDE CMAKE)
	file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_${p}DIR} _path)
	message(STATUS "Installing ${p} to ${_path}")
	unset(_path)
endforeach()

add_subdirectory(socket_client)
add_subdirectory(socket_server)
add_subdirectory(proto_msg)
#add_subdirectory(proto3_msg)   #protobuf generated header has an error


add_subdirectory(liba)
add_subdirectory(libb)
add_subdirectory(lib_combo_dll)
add_subdirectory(test_dll)
add_subdirectory(lib_combo_static2)
add_subdirectory(test_static)


#install(TARGETS liba libb libcombo_dll EXPORT lib_ab
#	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}  #relative to CMAKE_INSTALL_PREFIX
#	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#	PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDEDIR}
#)
#install(DIRECTORY libb/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#install(FILES liba/liba.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#install(FILES lib_combo_dll/dummy.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#install(EXPORT lib_ab DESTINATION share/lib_ab/cmake)
#export(TARGETS liba libb libcombo_dll FILE lib_ab.cmake)