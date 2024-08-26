cmake_minimum_required(VERSION 3.26)
project(project1)

set(CMAKE_CXX_STANDARD 17)

add_executable(project1
        cyclic_double_queue.hpp
        queue_app.cpp
        maze.hpp
        maze.cpp
        maze_app.cpp)
