find_package(Boost COMPONENTS system program_options REQUIRED)
find_package(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE main.cpp Client.cpp InputReader.cpp ConsoleWriter.cpp Operation.cpp OperationContext.cpp ReadDataOperation.cpp ReadHeaderOperation.cpp WriteDataOperation.cpp WritePingOperation.cpp Session.cpp)

add_executable(${PROJ_NAME} ${SOURCE})
target_link_libraries(${PROJ_NAME} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}) 
