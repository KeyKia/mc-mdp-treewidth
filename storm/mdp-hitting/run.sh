#!/usr/bin/env bash

path=/home/amir/Desktop/rmc-treewidth-code/

for folder in `ls ${path}benchmarks/dacapobenchmark/outputs/`
do
	if [[ ${folder} == raw_outputs || ${folder} == output.txt || ${folder} == transform.py || ${folder} == test.txt ]]
    then
        continue
    fi
	echo "folder is $folder"

	mkdir mdps-$folder
	python ${path}mdp/mdp_utils/mdpprob_reformat.py ${path}benchmarks/dacapobenchmark/outputs/raw_outputs/${folder}.txt mdps-$folder
    runtime1=0.0
    runtime2=0.0
    for file in `ls mdps-${folder}/`
    do
		echo "file is ${folder}/${file}"

        python transform.py mdps-${folder}/${file}
	target=`cat property.txt`
        start=`date +%s`
	
	storm --prism model.txt --prop "Pmax=? [F x=$target]" > ans.txt

	sleep 2

        end=`date +%s`
        dif1=`echo ${end} - ${start} | bc`
        dif2=`python calc_time.py`

        runtime1=`echo ${runtime1} + ${dif1} | bc`
        runtime2=`echo ${runtime2} + ${dif2} | bc`
	echo "r1=${runtime1}"
	echo "r2=${runtime2}"
    done
    echo ${folder} ${runtime1} >> results1.txt
    echo ${folder} ${runtime2} >> results2.txt
done
