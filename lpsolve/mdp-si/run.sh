#!/usr/bin/env bash

path=/Users/ali/research/rmc-treewidth-code/
for folder in `ls ${path}benchmarks/dacapobenchmark/outputs/`
do
    if [[ ${folder} == raw_outputs || ${folder} == output.txt || ${folder} == transform.py || ${folder} == test.txt ]]
    then
        continue
    fi


    runtime=0.0
    for file in `ls ${path}benchmarks/dacapobenchmark/outputs/${folder}`
    do
        dif=`python ${path}lpsolve/mdp-si/mdp_solver.py ${path}benchmarks/dacapobenchmark/outputs/${folder}/${file}`
        runtime=`echo ${runtime} + ${dif} | bc`
    done
    echo ${folder} ${runtime}
done