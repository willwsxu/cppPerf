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
set(PROTO_GENERATED_DIR "${PROJECT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${PROTO_GENERATED_DIR})

set(all_messages_PROTOS "" PARENT_SCOPE)
file(GLOB_RECURSE all_messages_PROTOS "${PROTO_SOURCE_DIR}/*.proto")

add_custom_target(proto_glob ALL 
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/proto_files_glob.cmake
	COMMAND ${CMAKE_COMMAND} -D PROTO_DIR=${PROTO_SOURCE_DIR} -D all_messages_PROTOS=""  -P ${CMAKE_CURRENT_SOURCE_DIR}/proto_files_glob.cmake
	WORKING_DIRECTORY
		${CMAKE_CURRENT_SOURCE_DIR}
)
foreach(PROTO_FILE ${all_messages_PROTOS})
	string(REGEX REPLACE "${PROTO_SOURCE_DIR}" "" proto_name ${PROTO_FILE})
	string(REGEX REPLACE ".proto" ".pb.cc" proto_source_file ${proto_name})
	set(proto_source_file ${PROJECT_BINARY_DIR}/generated/${proto_source_file})
	set(PROROC_COMMAND_ARGS
		"--cpp_out=${PROJECT_BINARY_DIR}/generated"
		"--proto_path=${PROTO_SOURCE_DIR}"
		"-I${Protobuf_INCLUDE_DIRS}"
		${PROTO_FILE}
	)
	add_custom_command ( # files are generated at build time, not make time
		OUTPUT ${proto_source_file}
		DEPENDS proto_glob
		COMMAND ${Protobuf_PROTOC_EXECUTABLE} ARGS ${PROROC_COMMAND_ARGS}
		COMMENT("custom command ${proto_source_file}")
	)
	set(PROTO_SOURCE_FILES ${PROTO_SOURCE_FILES} ${proto_source_file})
	message(STATUS "add proto source ${proto_source_file}") # make time
endforeach()
message(STATUS ${PROTO_SOURCE_FILES})
add_custom_target(proto_msg_gen ALL 
	DEPENDS ${PROTO_SOURCE_FILES} 
	COMMENT "${PROTO_SOURCE_FILES}"
)
include_directories(proto_msg
                           PUBLIC $<BUILD_INTERFACE:${PROTOBUF_INCLUDE_DIRS}>
                           PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/generated>
						   )
					  
add_library(proto_msg "")
target_sources(proto_msg
	PRIVATE ${PROTO_SOURCE_FILES}
)
target_link_libraries(proto_msg ${Protobuf_LIBRARIES})

add_dependencies(proto_msg proto_msg_gen)