cmake_minimum_required(VERSION 3.12.1)
project(libtest LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Boost REQUIRED COMPONENTS date_time system)

set(generated_lib "${CMAKE_BINARY_DIR}")
set(lib_combo_static ${generated_lib}/lib_combo_static.lib)
set(all_lib_objs 
	"${CMAKE_CURRENT_BINARY_DIR}/../liba/CMakeFiles/liba.dir/*.obj"
	"${CMAKE_CURRENT_BINARY_DIR}/../libb/CMakeFiles/libb.dir/*.obj"
	)
add_custom_command(
	OUTPUT ${lib_combo_static}
	depends liba libb
	COMMAND ar -x $<TARGET_FILE:liba>
	COMMAND ar -x $<TARGET_FILE:libb>
	COMMAND ar -qcs ${lib_combo_static} ${all_lib_objs}
	WORKING_DIRECTORY ${generated_lib}
	COMMENT "archive ${all_lib_objs}"
	VERBATIM
)
add_custom_target(libc_gen ALL
	DEPENDS ${lib_combo_static}
)
add_library(lib_combo_s STATIC IMPORTED GLOBAL)
#add_dependencies(lib_combo_s libc_gen)
set_target_properties(lib_combo_s
	PROPERTIES IMPORTED_LOCATION ${lib_combo_static}
)
target_include_directories(lib_combo_s 
	INTERFACE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
	)
set_target_properties(lib_combo_s
	PROPERTIES
	INTERFACE_LINK_LIBRARIES Boost::boost 
	INTERFACE_LINK_LIBRARIES Boost::date_time
)
add_executable(libtest main.cpp)
target_link_libraries(libtest lib_combo_s)
#target_link_directories(libtest PRIVATE ${generated_lib})