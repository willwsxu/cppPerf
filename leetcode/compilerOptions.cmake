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