include_guard()

project(data_structures_test)

add_executable(${PROJECT_NAME}
        dag_test.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${SRC_PATH}/data_structures
        ${SRC_PATH}/data_structures/graphs
)

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
