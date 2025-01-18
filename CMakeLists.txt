cmake_minimum_required(VERSION 3.5.0)

project(
        A-DS
        VERSION 1.0
        DESCRIPTION "Algorithms and data structures"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(bin)
add_subdirectory(DataStructures)
add_subdirectory(Algorithms)