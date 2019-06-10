cmake_minimum_required(VERSION 3.12.1)
set (CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/../CompilerOptions.cmake")
project(leetcode VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(leetcode_source_files
			${CMAKE_CURRENT_LIST_DIR}/../../catch_main.cpp
			binary_tree.cpp
			BinaryTree.cpp
)
add_executable(leetcode ${leetcode_source_files})
if (MSVC)
    # remove default debug info from CMAKE_CXX_FLAGS_INIT
    string (REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS_INIT ${CMAKE_CXX_FLAGS_INIT})
    string (REGEX REPLACE "/Zi" "" CMAKE_CXX_FLAGS_DEBUG_INIT ${CMAKE_CXX_FLAGS_DEBUG_INIT})

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} /W4")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG_INIT} /ZI")
endif()
Message(STATUS "CMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}")
Message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
#target_compile_options(leetcode PRIVATE $<$<CONFIG:Debug>:/ZI> $<$<CONFIG:Release>:/Zi>)  // did not replace Zi with ZI
target_link_options(leetcode PRIVATE $<$<CONFIG:Release>:/DEBUG /OPT:REF /OPT:ICF>)
target_include_directories(leetcode
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../library
							)
install(TARGETS leetcode DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)