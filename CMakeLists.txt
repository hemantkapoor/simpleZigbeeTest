cmake_minimum_required(VERSION 3.0)

project(simpleZigbeeTest)

file(GLOB_RECURSE sources      src/*.cpp src/*.h)
add_library(SimpleSerial "")
add_subdirectory(src/simpleSerial)
add_library(SimpleZigbee "")
add_subdirectory(src/simpleZigbee)


# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_executable(simpleZigbeeTest ${sources})
target_compile_options(simpleZigbeeTest PUBLIC -std=c++1y -Wall -Wfloat-conversion)
TARGET_LINK_LIBRARIES(simpleZigbeeTest PUBLIC -lpthread)

