#!/usr/bin/env bash

path=/Users/ali/research/rmc-treewidth-code/
for folder in `ls ${path}benchmarks/dacapobenchmark/outputs/`
do
    runtime=0.0
    for file in `ls ${path}benchmarks/dacapobenchmark/outputs/${folder}`
    do
        python ${path}lpsolve/mc/mc_solver.py ${path}benchmarks/dacapobenchmark/outputs/${folder}/${file}
        start=`gdate +%s.%N`
        lp_solve < out.txt > /dev/null
        end=`gdate +%s.%N`
        dif=`echo ${end}- ${start} | bc`
        runtime=`echo ${runtime} + ${dif} | bc`
    done
    echo ${folder} ${runtime}
done