cmake_minimum_required(VERSION 3.10)

project("Puzzle Block AI" VERSION 1.0.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)


add_subdirectory(src)