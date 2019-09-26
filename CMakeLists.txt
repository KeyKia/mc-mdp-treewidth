cmake_minimum_required(VERSION 3.14)
project(rmc-treewidth-code C)

set(CMAKE_C_STANDARD 11)

add_executable(rmc-treewidth-code Main-MC/main.c Main-MC/vector.h Main-MC/vector.c Main-MC/tree_decomposition.c Main-MC/tree_decomposition.h)