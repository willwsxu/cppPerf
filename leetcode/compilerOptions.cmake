#https://stackoverflow.com/questions/45995784/how-to-set-compiler-options-with-cmake-in-visual-studio-2017
# log all *_INIT variables
get_cmake_property(_varNames VARIABLES)
list (REMOVE_DUPLICATES _varNames)
list (SORT _varNames)
foreach (_varName ${_varNames})
    if (_varName MATCHES "_INIT$")
        message(STATUS "${_varName}=${${_varName}}")
    endif()
endforeach()

function(update_CXX_flag proj_name warning_level debug_info)

if (MSVC)
    # remove default debug info from CMAKE_CXX_FLAGS_INIT
    string (REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS_INIT ${CMAKE_CXX_FLAGS_INIT})
    string (REGEX REPLACE "/Zi" "" CMAKE_CXX_FLAGS_DEBUG_INIT ${CMAKE_CXX_FLAGS_DEBUG_INIT})

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} ${warning_level}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG_INIT} ${debug_info}")
endif()
Message(STATUS "CMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}")
Message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
#target_compile_options(leetcode PRIVATE $<$<CONFIG:Debug>:/ZI> $<$<CONFIG:Release>:/Zi>)  // did not replace Zi with ZI
target_link_options(${proj_name} PRIVATE $<$<CONFIG:Release>:/DEBUG /OPT:REF /OPT:ICF>)
endfunction()
