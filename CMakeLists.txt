cmake_minimum_required(VERSION 3.12.1)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if (DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    else()
        if(WIN32)
            set(root_dir "C:")
        else()
            set(root_dir "$ENV{HOME}")
        endif()
        file(GLOB VCPKG_SEARCH_PATH ${root_dir}/*/vcpkg)
        Message(STATUS "VCPKG_SEARCH_PATH=${VCPKG_SEARCH_PATH}")
        unset(vcpkg_dir CACHE)
        find_path(vcpkg_dir .vcpkg-root PATHS ${VCPKG_SEARCH_PATH})
        set(CMAKE_TOOLCHAIN_FILE "${vcpkg_dir}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    endif()
endif()
Message(STATUS "CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}. set it in env or we will try to find it")

set (CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/leetcode/CompilerOptions.cmake")
project(all_cpp_projetcs VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(catch_source_files
			${CMAKE_CURRENT_LIST_DIR}/catch_main.cpp
)
add_library(catch_test STATIC ${catch_source_files})

target_include_directories(catch_test
							PUBLIC ${CMAKE_CURRENT_LIST_DIR}
							)
install(TARGETS catch_test DESTINATION bin)

add_subdirectory(leetcode)
add_subdirectory(library)
add_subdirectory(devtest)