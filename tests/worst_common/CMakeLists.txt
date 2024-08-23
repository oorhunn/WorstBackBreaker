include_guard()

project(worst_common_test)

add_executable(${PROJECT_NAME}
        binary_io_test.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${SRC_PATH}/common
)

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
