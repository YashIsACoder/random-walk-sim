cmake_minimum_required(VERSION 3.12)
project(RandomWalkSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(random_walk
    src/main.cpp
    src/Walker.cpp
    src/RandomWalk.cpp
    src/Statistics.cpp
)

target_include_directories(random_walk PRIVATE include)
