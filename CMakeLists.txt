cmake_minimum_required(VERSION 3.14)
project(test C)

set(CMAKE_C_STANDARD 11)

add_library(vector.h vector.c)
add_executable(rmc-treewidth-code main.c)