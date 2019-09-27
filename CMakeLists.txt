cmake_minimum_required(VERSION 3.14)
project(rmc-treewidth-code C)

set(CMAKE_C_STANDARD 11)

add_executable(rmc-treewidth-code-mc Main-MC/main.c Main-MC/vector.h Main-MC/vector.c Main-MC/tree_decomposition.c Main-MC/tree_decomposition.h)
add_executable(rmc-treewidth-code-sle Main-SLE/main.c Main-SLE/vector.h Main-SLE/vector.c Main-SLE/tree_decomposition.c Main-SLE/tree_decomposition.h Main-SLE/gram_schmidt.h Main-SLE/derive_independent_eqs.h Main-SLE/gaussian_elimination.h)
