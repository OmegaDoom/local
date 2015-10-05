cmake_minimum_required(VERSION 3.0)
set(PROJ_NAME Client)
project(${PROJ_NAME})

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

include_directories("Header Files")
add_subdirectory("Source Files")

