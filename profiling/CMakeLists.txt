cmake_minimum_required(VERSION 3.12.1)
project(profiling VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(profiling_source_files
			main.cpp
)
set(app_name profile_mem)
add_executable(${app_name} ${profiling_source_files})
target_compile_features(${app_name} PUBLIC cxx_std_17)
target_compile_definitions(${app_name} PUBLIC UNIT_TEST)
#-pg for gprof
#-lprofiler cpu profile
#-ltcmalloc mem profile, linked last
#sudo apt-get install google-perftools-dev
#    sudo yum install google-perftools-devel
#HEAPPROFILE=/tmp/test.hprof HEAPPROFILESIGNAL=12 ./bin/profile_mem
#    killall -12 ./bin/profile_mem   ( to start or stop profiling)
#sudo apt install google-perftools
#    sudo yum install google-perftools
#google-pprof --text ./bin/profile_mem /tmp/test.hprof.0001.heap
#google-pprof --text --base=/tmp/test.hprof.0001.heap ./bin/profile_mem /tmp/test.hprof.0005.heap

target_compile_options(${app_name} PUBLIC $<$<PLATFORM_ID:Linux>:-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free -fno-omit-frame-pointer>)
target_include_directories(${app_name}
	PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/cpp11/threading_model/>
)

target_link_libraries(${app_name} 
		PRIVATE catch_test
        PRIVATE $<$<PLATFORM_ID:Linux>:-rdynamic>
        PRIVATE $<$<PLATFORM_ID:Linux>:-lpthread>
        PRIVATE $<$<PLATFORM_ID:Linux>:-lprofiler>
        PRIVATE $<$<PLATFORM_ID:Linux>:-ltcmalloc>
)