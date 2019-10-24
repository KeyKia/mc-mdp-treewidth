#!/usr/bin/env bash

path=/Users/goharshady/ist-git/rmc-treewidth-code/
for folder in `ls ${path}benchmarks/dacapobenchmark/outputs/`
do
	echo "folder is $folder"
	mkdir mdps-$folder
	python ${path}mdp/mdp_utils/mdpsum_reformat.py ${path}benchmarks/dacapobenchmark/outputs/raw_outputs/${folder}.txt mdps-$folder
    runtime=0.0
    for file in `ls mdps-${folder}/`
    do
		echo "file is ${folder}/${file}"
		
        python mdp_solver.py mdps-${folder}/${file}
        start=`gdate +%s.%N`
        lp_solve < out.txt > /dev/null
        end=`gdate +%s.%N`
        dif=`echo ${end}- ${start} | bc`
        runtime=`echo ${runtime} + ${dif} | bc`
    done
    echo ${folder} ${runtime} >> results.txt
done