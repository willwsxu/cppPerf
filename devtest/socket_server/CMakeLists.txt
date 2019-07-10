cmake_minimum_required(VERSION 3.12.1)
project(socket_server)
add_executable(socket_server socket_server.cpp)
#install(TARGETS socket_server DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)