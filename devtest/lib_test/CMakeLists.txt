cmake_minimum_required(VERSION 3.12.1)
project(libtest LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(generated_lib "${CMAKE_BINARY_DIR}")
set(lib_combo_static ${generated_lib}/lib_combo_static.lib)
set(all_lib_objs 
	"${CMAKE_BINARY_DIR}/devtest/liba/CMakeFiles/liba.dir/a.cpp.obj"
	"${CMAKE_BINARY_DIR}/devtest/libb/CMakeFiles/libb.dir/b.cpp.obj"
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
add_library(libc STATIC IMPORTED GLOBAL)
#add_dependencies(libc libc_gen)
set_target_properties(libc
	PROPERTIES IMPORTED_LOCATION ${lib_combo_static}
)
#add_dependencies(libtest libc)

add_executable(libtest main.cpp)
target_link_libraries(libtest libc)
target_include_directories(libtest 
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../liba/include>
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../libb/include>
	)
#target_link_directories(libtest PRIVATE ${generated_lib})