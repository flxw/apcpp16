cmake_minimum_required(VERSION 2.6)

project(ASSIGNMENT2)

set(CMAKE_CXX_FLAGS "-g -Wall")

add_library(fileformat STATIC textfileformat.cpp binaryfileformat.cpp FileContent.cpp Value.cpp)
add_executable(fileformat-test fileformat-test.cpp fileformat-testdata.cpp)
target_link_libraries(fileformat-test fileformat)