include_guard()

project(thread_pool_test)

add_executable(${PROJECT_NAME}
        thread_pool_test.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${SRC_PATH}/thread_pool
)

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
