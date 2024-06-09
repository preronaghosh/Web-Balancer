cmake_minimum_required(VERSION 3.10)

project(Multithreaded-Web-Server VERSION 1.0)

# Add the directory containing header files to the include directories
include_directories(include)

# Add Google Test as a subdirectory
add_subdirectory(lib/googletest)

add_subdirectory(tests)

set(SOURCE_FILES 
        src/main.cpp 
        src/LoadBalancer.cpp 
        src/Server.cpp 
        src/ThreadPool.cpp)

# Add the source files to the executable target
add_executable(main ${SOURCE_FILES})

