cmake_minimum_required(VERSION 3.10)

project(Multithreaded-Web-Server VERSION 1.0)

include_directories(include)

add_executable(tests src/test_main.cpp)

# Link against Google Test libraries
target_link_libraries(tests gtest gtest_main)