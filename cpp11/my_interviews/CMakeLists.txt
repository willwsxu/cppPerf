cmake_minimum_required(VERSION 3.12.1)
project(cpp11Interview VERSION 0.1 LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
set(cpp11_source_files
    ../arbitrate.cpp
    ../atomic_set.cpp
    ../BrickByBrick.cpp
    ../DataFeed.cpp
    ../DockingSchedule.cpp
    ../gaussian.cpp
    ../Iterator.cpp
    ../jumpTrading.cpp
    ../KnowledgeBase.cpp
    ../PhoneRegistry.cpp
    ../requestCounter.cpp
    ../ReservationSystem.cpp
    ../SharedPtr.cpp
    ../smallVec.cpp
)


add_executable(cpp11Interview ${cpp11_source_files})
target_include_directories(cpp11Interview 
	PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/cpp11>
	)
target_link_libraries(cpp11Interview 
		PRIVATE catch_test
		PRIVATE Boost::boost
		)