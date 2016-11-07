cmake_minimum_required(VERSION 2.6)

project(ASSIGNMENT2)

set(CMAKE_CXX_FLAGS "-g -Wall")

add_executable(fileformat-test fileformat-test.cpp fileformat-testdata.cpp textfileformat.cpp binaryfileformat.cpp FileContent.cpp Value.cpp)