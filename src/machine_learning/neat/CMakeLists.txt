include_guard()

project(Neat)

message(STATUS "Neat is Included.!")

find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)
set(SFML_LIBS sfml-system sfml-window sfml-graphics sfml-audio)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_PATH}/data_structures/graphs)

target_link_libraries(${PROJECT_NAME} PRIVATE ${SFML_LIBS})
