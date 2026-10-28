cmake_minimum_required(VERSION 3.20)

project("Dynamic Team Orieentering Problem Solver")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    dtop-solver
    src/main.cc
    src/dtop_instance.cc
    src/dtop_parser.cc
    src/algorithm/solomon_regret_algorithm.cc
)