cmake_minimum_required(VERSION 3.14)
project(rmc-treewidth-code C)

set(CMAKE_C_STANDARD 11)

add_executable(mc-prob mc/mc_main.c utils/vector.h utils/vector.c mc/mc_treedec.c mc/mc_treedec.h mc/mc_solve.h)
add_executable(sle utils/vector.h utils/vector.c sle/sle_main.c sle/sle_treedec.c sle/sle_treedec.h sle/derive_independent_eqs.h sle/gaussian_elimination.h)
add_executable(gaus-mcprob-test experiments/gaus/gaus_mcprob_test.c experiments/gaus/gaussian_elim.h)
