cmake_minimum_required(VERSION 3.12.1)
project(libtest LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(libtest main.cpp)
add_custom_target(build_lib ALL
	DEPENDS liba libb
)
add_dependencies(libtest build_lib)

set(static_lib_projs liba libb)
foreach(lib_proj ${static_lib_projs})
	set(out_obj "${CMAKE_BINARY_DIR}/devtest/${lib_proj}/CMakeFiles/${lib_proj}.dir/*.obj")
	add_custom_command(
		OUTPUT  ${out_obj}
		COMMAND ar -x $<TARGET_FILE:${lib_proj}>
		DEPENDS ${lib_proj}
	)
	file(GLOB _lib_objs ${out_obj})
	message(STATUS "_lib_objs=${_lib_objs} from ${out_obj}")
	set(all_lib_objs ${all_lib_objs} ${_lib_objs})
endforeach()

set(lib_combo_static ${CMAKE_BINARY_DIR}/lib_combo_static.lib)
message(STATUS "all_lib_objs=${all_lib_objs}")
add_custom_command(
	OUTPUT ${lib_combo_static}
	DEPENDS ${all_lib_objs}
	COMMAND ar -qcs ${lib_combo_static} ${all_lib_objs}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_custom_target(libc_gen
	DEPENDS ${lib_combo_static}
)
add_library(libc STATIC IMPORTED GLOBAL)
add_dependencies(libc libc_gen)
set_target_properties(libc
	PROPERTIES IMPORTED_LOCATION ${lib_combo_static}
)
add_dependencies(libtest libc)
message(STATUS "lib_combo_static=${lib_combo_static}")
target_link_libraries(libtest liba libb)
target_include_directories(libtest 
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../liba/include>
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../libb/include>
	)