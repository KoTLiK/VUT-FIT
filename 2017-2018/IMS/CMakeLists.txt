cmake_minimum_required(VERSION 3.7.2)

project(IMS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O2 -pedantic -Wall")

set(SOURCE_FILES src/main.cpp src/common/config.h src/processes/Failure.cpp src/processes/Failure.h src/processes/Toilet.cpp src/processes/Toilet.h src/processes/Post.cpp src/processes/Post.h src/processes/Lunch.cpp src/processes/Lunch.h src/processes/Shift.cpp src/processes/Shift.h)
add_executable(simulation ${SOURCE_FILES})

target_link_libraries(simulation simlib)
