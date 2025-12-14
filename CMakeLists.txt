cmake_minimum_required(VERSION 3.12)
project(RandomWalkSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(randomwalk_lib
    src/Walker.cpp
    src/RandomWalk.cpp
    src/Statistics.cpp
)

target_include_directories(randomwalk_lib PUBLIC include)

add_executable(random_walk src/main.cpp)
target_link_libraries(random_walk PRIVATE randomwalk_lib)

add_executable(run_tests tests/test_stats.cpp)
target_link_libraries(run_tests PRIVATE randomwalk_lib)
