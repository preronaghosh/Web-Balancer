cmake_minimum_required(VERSION 3.10)

project(MyProject VERSION 1.0)

# Add the directory containing header files to the include directories
include_directories(include)

# Add the source files to the executable target
add_executable(main src/main.cpp src/LoadBalancer.cpp src/Server.cpp src/ThreadPool.cpp)
