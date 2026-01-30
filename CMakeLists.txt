cmake_minimum_required(VERSION 3.15)
project(RandomWalkVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System)

add_executable(random_walk_visual
    src/main.cpp
    src/Walker.cpp
    src/RandomWalk.cpp
    src/Statistics.cpp
    src/Renderer.cpp
)

target_include_directories(random_walk_visual PRIVATE include)

target_link_libraries(random_walk_visual
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)
