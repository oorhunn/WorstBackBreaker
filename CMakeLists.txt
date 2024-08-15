cmake_minimum_required(VERSION 3.27)
project(WorstBackBreaker)

set(CMAKE_CXX_STANDARD 20)

set(SRC_PATH ${CMAKE_CURRENT_LIST_DIR}/src)

include_directories(${SRC_PATH}/common)

add_executable(${PROJECT_NAME} main.cpp)


add_subdirectory(src/machine_learning/neat)
add_subdirectory(src/data_structures)

