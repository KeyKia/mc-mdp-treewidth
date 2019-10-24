cmake_minimum_required(VERSION 3.14)
project(rmc-treewidth-code C)

set(CMAKE_C_STANDARD 11)

#MATH(EXPR stack_size "100 * 1024 * 1024") # 16 Mb
#set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")

add_executable(mc-prob mc/mc_hitprob/mc_hitptob_main.c utils/vector.h utils/vector.c mc/mc_treedec.c mc/mc_treedec.h mc/mc_hitprob/mc_hitprob_solve.h)
add_executable(sle utils/vector.h utils/vector.c sle/sle_main.c sle/sle_treedec.c sle/sle_treedec.h sle/derive_independent_eqs.h sle/gaussian_elimination.h)
add_executable(gaus-mcprob-test gaussian_elimination/gaus_mcprob_test.c gaussian_elimination/gaussian_elim.h)
add_executable(mdp-prob mdp/mdp_hitprob/hitprob_policy_iter.c utils/vector.h utils/vector.c mc/mc_treedec.c mc/mc_treedec.h mc/mc_hitprob/mc_hitprob_solve.h)
add_executable(mc-discsum mc/mc_discsum/mc_discsum_main.c utils/vector.h utils/vector.c mc/mc_treedec.c mc/mc_treedec.h mc/mc_discsum/mc_discsum_solve.h)
add_executable(mdp-discsum mdp/mdp_discsum/discsum_policy_iter.c utils/vector.h utils/vector.c mc/mc_treedec.c mc/mc_treedec.h mc/mc_discsum/mc_discsum_solve.h gaussian_elimination/gaussian_elim.h)
