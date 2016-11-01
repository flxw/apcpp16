cmake_minimum_required(VERSION 2.6)

project(GREETWORLD)

set(CMAKE_CXX_FLAGS "-g -Wall")

add_executable(greet_world greet_world.cpp)
set_target_properties(greet_world PROPERTIES COMPILE_FLAGS "-save-temps")

add_library(provider SHARED provider.cpp)
add_executable(providerlibtest providerlibtest.cpp)
target_link_libraries(providerlibtest provider)

add_executable(validate_emails validate_emails.cpp)

add_executable(count_words count_words.cpp)