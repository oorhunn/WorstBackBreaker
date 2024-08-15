include_guard()

project(neat_tests)

add_executable(${PROJECT_NAME}
    network_test.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRC_PATH}/machine_learning/neat
)

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})