cmake_minimum_required(VERSION 3.12.1)
project(proto3_msg LANGUAGES CXX)
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

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
# protobuf_generate_cpp did not produce source files

file(GLOB_RECURSE all_messages_PROTOS "${CMAKE_CURRENT_LIST_DIR}/*.proto")

foreach(PROTO_FILE ${all_messages_PROTOS})
	string(REGEX REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" proto_name ${PROTO_FILE})
	string(REGEX REPLACE ".proto" ".pb.cc" proto_source_file ${proto_name})
	set(proto_source_file ${PROJECT_BINARY_DIR}/generated/${proto_source_file})
	set(PROROC_COMMAND_ARGS
		"--cpp_out=${PROJECT_BINARY_DIR}/generated"
		"--proto_path=${CMAKE_CURRENT_LIST_DIR}"
		"-I${Protobuf_INCLUDE_DIRS}"
		${PROTO_FILE}
	)
	add_custom_command (
		OUTPUT ${proto_source_file}
		DEPENDS ${PROTO_FILE}
		COMMAND ${Protobuf_PROTOC_EXECUTABLE} ARGS ${PROROC_COMMAND_ARGS}
	)
	set(PROTO_SOURCE_FILES ${PROTO_SOURCE_FILES} ${proto_source_file})
endforeach()

add_custom_target(proto_msg3_gen ALL
	DEPENDS ${PROTO_SOURCE_FILES}
)                         

include_directories(proto3_msg
                           PUBLIC $<BUILD_INTERFACE:${PROTOBUF_INCLUDE_DIRS}>
                           PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/generated>
						   )
					  
add_library(proto3_msg ${PROTO_SOURCE_FILES}
				)
target_link_libraries(proto3_msg PUBLIC ${Protobuf_LIBRARIES})
add_dependencies(proto3_msg proto_msg3_gen)

foreach(exe_name add_person list_people)
	set(${exe_name}_SRCS "${exe_name}.cc")
	add_executable(${exe_name} "")
	target_sources(${exe_name}
		PRIVATE ${${exe_name}_SRCS}
	)
	
	target_link_libraries(${exe_name} 
		PRIVATE ${Protobuf_LIBRARIES}
		PRIVATE proto3_msg
	)
endforeach()