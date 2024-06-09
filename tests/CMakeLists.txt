cmake_minimum_required(VERSION 3.10)

project(Multithreaded-Web-Server-Tests VERSION 1.0)

include_directories(include)

# Add test files
set(TEST_SOURCES src/test_main.cpp)

# Add source code files needed for testing
set(SOURCE_FILES_FOR_TEST 
    ${CMAKE_SOURCE_DIR}/src/Server.cpp
    ${CMAKE_SOURCE_DIR}/src/ThreadPool.cpp)

add_executable(tests ${TEST_SOURCES} ${SOURCE_FILES_FOR_TEST})

# Link against Google Test libraries
target_link_libraries(tests gtest gtest_main)