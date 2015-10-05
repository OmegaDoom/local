find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

set(UNIT_TEST_NAME FactorizationTest)
set(SOURCE FactorizationUnitTest.cpp)

add_executable(${UNIT_TEST_NAME} ${SOURCE})
target_link_libraries(${UNIT_TEST_NAME} ${Boost_LIBRARIES})
add_test(${UNIT_TEST_NAME} ${UNIT_TEST_NAME})
