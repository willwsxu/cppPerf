cmake_minimum_required(VERSION 3.12.1)
project(lib_combo_static2 LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Boost REQUIRED COMPONENTS date_time system)

#must be actual obj file name, not *.obj
set(all_lib_objs2 
	"${CMAKE_CURRENT_BINARY_DIR}/../liba/CMakeFiles/liba.dir/a.cpp.obj"
	"${CMAKE_CURRENT_BINARY_DIR}/../libb/CMakeFiles/libb.dir/b.cpp.obj"
	)
SET_SOURCE_FILES_PROPERTIES(
  ${all_lib_objs2}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)
add_library(lib_combo_static2 STATIC ${all_lib_objs2})
# below is required to link obj files into lib
set_target_properties(lib_combo_static2 PROPERTIES LINKER_LANGUAGE CXX)
add_dependencies(lib_combo_static2 liba)
add_dependencies(lib_combo_static2 libb)
target_include_directories(lib_combo_static2 
	INTERFACE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
	)
target_link_libraries(lib_combo_static2
	PUBLIC Boost::boost 
	PUBLIC Boost::date_time
)