cmake_minimum_required(VERSION 3.14)
project(rmc-treewidth-code C)

set(CMAKE_C_STANDARD 11)

add_executable(mc-prob mc/mc_hitprob/mc_hitptob_main.c utils/vector.h utils/vector.c mc/mc_treedec.c mc/mc_treedec.h mc/mc_hitprob/mc_hitprob_solve.h)
add_executable(sle utils/vector.h utils/vector.c sle/sle_main.c sle/sle_treedec.c sle/sle_treedec.h sle/derive_independent_eqs.h sle/gaussian_elimination.h)
add_executable(gaus-mcprob-test experiments/gaus/gaus_mcprob_test.c experiments/gaus/gaussian_elim.h)
add_executable(mdp-prob mdp/policy_iter.c utils/vector.h utils/vector.c mc/mc_treedec.c mc/mc_treedec.h mc/mc_hitprob/mc_hitprob_solve.h)
add_executable(mc-expsum mc/mc_expsum/mc_expsum_main.c utils/vector.h utils/vector.c mc/mc_treedec.c mc/mc_treedec.h mc/mc_expsum/mc_expsum_solve.h)
