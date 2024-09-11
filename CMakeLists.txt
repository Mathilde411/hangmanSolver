cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
project(untitled)

add_executable(untitled main.cpp
        Solver.cpp
        Solver.h)
