cmake_minimum_required(VERSION 3.12.1)
project(cmake_demo VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(socket_client)
add_subdirectory(socket_server)
add_subdirectory(proto_msg)
#add_subdirectory(proto3_msg)   #protobuf generated header has an error


add_subdirectory(liba)
add_subdirectory(libb)
add_subdirectory(lib_combo_dll)
add_subdirectory(lib_test_dll)
#add_subdirectory(lib_combo_static2)
add_subdirectory(lib_test)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
include(GNUInstallDirs) #needed for CMAKE_INSTALL_XXX def
message(STATUS "install_lib=${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

install(TARGETS liba libb EXPORT lib_ab
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}  #relative to CMAKE_INSTALL_PREFIX
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY libb/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES liba/liba.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT lib_ab DESTINATION share/lib_ab/cmake)
export(TARGETS liba libb FILE lib_ab.cmake)