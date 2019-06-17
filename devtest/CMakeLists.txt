cmake_minimum_required(VERSION 3.12.1)
project(hello_socket)
add_subdirectory(socket_client)
add_subdirectory(socket_server)
add_subdirectory(proto_msg)


add_subdirectory(liba)
add_subdirectory(libb)
add_subdirectory(lib_test)