cmake_minimum_required(VERSION 3.10)

project(Multithreaded-Web-Server-Tests VERSION 1.0)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(include)

add_executable(tests src/test_main.cpp)

# Link against Google Test libraries
target_link_libraries(tests gtest gtest_main)

# Enable testing
enable_testing()

# Add the test executable
add_test(NAME tests COMMAND tests)