cmake_minimum_required(VERSION 3.12.1)
project(proto_msg LANGUAGES CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Protobuf_USE_STATIC_LIBS ON)
find_package(Protobuf REQUIRED)

message("   --> PROTOBUF LIB: ${Protobuf_LIBRARY}")
message("   --> PROTOBUF INCLUDE: ${Protobuf_INCLUDE_DIRS}")
message("   --> PROTOBUF Found: ${Protobuf_FOUND}")

message(STATUS "Discovered Protobuf v${Protobuf_VERSION}")
message(STATUS "Using protoc ${Protobuf_PROTOC_EXECUTABLE}")

set(PROTO_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/source)
file(GLOB_RECURSE all_messages_PROTOS "${PROTO_SOURCE_DIR}/*.proto")

foreach(PROTO_FILE ${all_messages_PROTOS})
	add_custom_command (
		OUTPUT ${PROJECT_BINARY_DIR}/generated/addressbook.pb.cc
		DEPENDS ${PROTO_FILE}
		COMMAND ${Protobuf_PROTOC_EXECUTABLE} 
		ARGS "--cpp_out=${PROJECT_BINARY_DIR}/generated" "--proto_path=${PROTO_SOURCE_DIR}" ${PROTO_FILE}
	)
	set(PROTO_SOURCE_FILES ${PROTO_SOURCE_FILES} ${PROJECT_BINARY_DIR}/generated/addressbook.pb.cc)
endforeach()

add_custom_target(proto_msg_gen ALL
	DEPENDS ${PROTO_SOURCE_FILES}
)

include_directories(proto_msg
                           PUBLIC $<BUILD_INTERFACE:${PROTOBUF_INCLUDE_DIRS}>
                           PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/generated>
						   )
					  
add_library(proto_msg ${PROTO_SOURCE_FILES}
				)
target_link_libraries(proto_msg ${Protobuf_LIBRARIES})

add_dependencies(proto_msg proto_msg_gen)