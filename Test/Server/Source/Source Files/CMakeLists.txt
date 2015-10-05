find_package(Boost COMPONENTS system program_options thread REQUIRED)
find_package(Threads REQUIRED)
find_package(Poco COMPONENTS Util REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${Poco_INCLUDE_DIRS})

set(SOURCE main.cpp Client.cpp Server.cpp ServiceApplication.cpp Operation.cpp OperationContext.cpp ReadDataOperation.cpp ReadHeaderOperation.cpp WriteDataOperation.cpp ServiceTask.cpp TaskExecutor.cpp Helper.cpp)

add_executable(${PROJ_NAME} ${SOURCE})
target_link_libraries(${PROJ_NAME} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${Poco_LIBRARIES})
