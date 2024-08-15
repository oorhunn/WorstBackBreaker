cmake_minimum_required(VERSION 3.27)

project(tests)

set(CMAKE_CXX_STANDARD 20)

set(SRC_PATH ${CMAKE_CURRENT_LIST_DIR}/../src/)



find_package(GTest REQUIRED)
enable_testing()
include(GoogleTest)


add_subdirectory(thread_pool)
add_subdirectory(data_structures)
add_subdirectory(neat)

