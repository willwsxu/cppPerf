project(socket_client)
cmake_minimum_required(VERSION 3.12.1)
set(CMAKE_VERBOSE_MAKEFILE on)

#add_compile_options("/showIncludes")
add_executable(socket_client socket_client.cpp)

target_link_libraries(socket_client proto_msg)