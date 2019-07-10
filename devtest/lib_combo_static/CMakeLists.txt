cmake_minimum_required(VERSION 3.12.1)
project(lib_combo_s VERSION "1.0" LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Boost REQUIRED COMPONENTS date_time system)

set(generated_lib "${CMAKE_BINARY_DIR}")
set(lib_combo_static ${generated_lib}/lib_combo_static${CMAKE_STATIC_LIBRARY_SUFFIX})
set(all_lib_objs 
	"${CMAKE_CURRENT_BINARY_DIR}/../liba/CMakeFiles/liba.dir/*${CMAKE_CXX_OUTPUT_EXTENSION}"
	"${CMAKE_CURRENT_BINARY_DIR}/../libb/CMakeFiles/libb.dir/*${CMAKE_CXX_OUTPUT_EXTENSION}"
	)
add_custom_command(
	OUTPUT ${lib_combo_static}
	depends liba libb
	COMMAND rm ARGS -f ${all_lib_objs}
	COMMAND ar -x $<TARGET_FILE:liba>
	COMMAND ar -x $<TARGET_FILE:libb>
	COMMAND ar -qcs ${lib_combo_static} ${all_lib_objs}
	COMMAND ar ARGS -r test.lib ${all_lib_objs}  # -r works as well as -q
	COMMAND rm ARGS -f ${all_lib_objs}
	WORKING_DIRECTORY ${generated_lib}
	COMMENT "archive ${all_lib_objs}"
	VERBATIM
)
add_custom_target(libc_gen ALL
	DEPENDS ${lib_combo_static}
)

add_library(lib_combo_s STATIC IMPORTED GLOBAL)
add_dependencies(lib_combo_s libc_gen)
set_target_properties(lib_combo_s
	PROPERTIES IMPORTED_LOCATION ${lib_combo_static}
)
target_include_directories(lib_combo_s 
	INTERFACE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
	)

#or use target_link_libraries
set_target_properties(lib_combo_s
	PROPERTIES
	INTERFACE_LINK_LIBRARIES Boost::boost 
	INTERFACE_LINK_LIBRARIES Boost::date_time
)

#camke does not support install imported lib lib_combo_s, try out using interface
add_library(lib_combo_s_interface INTERFACE)
add_dependencies(lib_combo_s_interface libc_gen)
set_target_properties(lib_combo_s_interface
	PROPERTIES
#	INTERFACE_LINK_LIBRARIES Boost::boost 
#	INTERFACE_LINK_LIBRARIES Boost::date_time
#	INTERFACE_INCLUDE_DIRECTORIES $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
	INTERFACE_INCLUDE_DIRECTORIES $<BUILD_INTERFACE:${Boost_INCLUDE_DIR}>
	INTERFACE_LINK_LIBRARIES lib_combo_static${CMAKE_STATIC_LIBRARY_SUFFIX}
	INTERFACE_LINK_DIRECTORIES $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/lib>
    INTERFACE_LINK_DIRECTORIES $<INSTALL_INTERFACE:${INSTALL_LIBDIR}>  #for installed package
)
#target_link_directories(lib_combo_s_interface   does not work in vc2017
#	INTERFACE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/lib>
#    INTERFACE $<INSTALL_INTERFACE:${INSTALL_LIBDIR}>  #for installed package
#)
target_include_directories(lib_combo_s_interface 
	INTERFACE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    INTERFACE $<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>  #include for installed package
)
target_link_libraries(lib_combo_s_interface 
	INTERFACE Boost::boost
	INTERFACE Boost::date_time
)
install(TARGETS  lib_combo_s_interface
	EXPORT ImplibTargets  #referred later
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}  #relative to CMAKE_INSTALL_PREFIX
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
	FILES
		${lib_combo_static}
	DESTINATION
		${INSTALL_LIBDIR}
)

install(EXPORT ImplibTargets
	NAMESPACE Implib_NS::
	DESTINATION ${INSTALL_CMAKEDIR}
)
message(STATUS "PROJECT_VERSION=${PROJECT_VERSION}")
#generate cmake files to find package and version
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/ImplibConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
	${CMAKE_SOURCE_DIR}/cmake/ImplibConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/ImplibConfig.cmake
	INSTALL_DESTINATION ${INSTALL_CMAKEDIR}
)
install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/ImplibConfig.cmake
		${CMAKE_CURRENT_BINARY_DIR}/ImplibConfigVersion.cmake
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