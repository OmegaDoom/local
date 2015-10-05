cmake_minimum_required(VERSION 3.0)
set(PROJ_NAME CalcServer)
project(${PROJ_NAME})
enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")
if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

include_directories("Header Files")
add_subdirectory("Source Files")
add_subdirectory("UnitTests")

