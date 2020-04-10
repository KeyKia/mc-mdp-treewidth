cmake_minimum_required(VERSION 3.0.2)
project(rmc-treewidth-code C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-O2")

add_executable(mc-hitprob mc_hitprob/ours/mc_hitptob_main.c commons/vector.h commons/vector.c commons/mc_treedec.c commons/mc_treedec.h mc_hitprob/ours/mc_hitprob_solve.h)
add_executable(mc-discsum mc_discsum/ours/mc_discsum_main.c commons/vector.h commons/vector.c commons/mc_treedec.c commons/mc_treedec.h mc_discsum/ours/mc_discsum_solve.h)

add_executable(mdp-hitprob mdp_hitprob/ours/hitprob_policy_iter.c commons/vector.h commons/vector.c commons/mc_treedec.c commons/mc_treedec.h mc_hitprob/ours/mc_hitprob_solve.h)
add_executable(mdp-discsum mdp_discsum/ours/discsum_policy_iter.c commons/vector.h commons/vector.c commons/mc_treedec.c commons/mc_treedec.h mc_discsum/ours/mc_discsum_solve.h commons/gaussian_elim.h)

add_executable(gauss-mc-hitprob mc_hitprob/gauss/mc_hitprob_gauss.c commons/gaussian_elim.h commons/vector.h commons/vector.c commons/mc_treedec.c commons/mc_treedec.h)
add_executable(gauss-mc-discsum mc_discsum/gauss/mc_discsum_gauss.c commons/gaussian_elim.h commons/vector.h commons/vector.c commons/mc_treedec.c commons/mc_treedec.h)

add_executable(sle commons/vector.h commons/vector.c sle/sle_main.c sle/sle_treedec.c sle/sle_treedec.h sle/derive_independent_eqs.h sle/gaussian_elimination.h)