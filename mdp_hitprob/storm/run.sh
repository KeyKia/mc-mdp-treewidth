#!/usr/bin/env bash

path=../../

red=`tput setaf 1`
green=`tput setaf 2`
yellow=`tput setaf 3`
blue=`tput setaf 4`
reset=`tput sgr0`

cnt=1
tot=`ls ${path}benchmarks/dacapobenchmark/outputs/raw_outputs | wc -l`

for folder in `ls ${path}benchmarks/dacapobenchmark/outputs/raw_outputs`
do
	if [[ ${folder} == raw_outputs || ${folder} == output.txt || ${folder} == transform.py || ${folder} == test.txt ]]
    then
        continue
    fi

    folder=${folder%".txt"}
	echo "${green}Running Storm for computing Markov Decision Process Hitting Probabilities on benchmark --------> $folder ${reset}"
	percent=`echo "100 * ${cnt} / ${tot}" | bc`

	mkdir mdps-$folder
	python3 ${path}utils/mdpprob_reformat.py ${path}benchmarks/dacapobenchmark/outputs/raw_outputs/${folder}.txt mdps-$folder
    runtime=0.0
    for file in `ls mdps-${folder}/`
    do
		echo " [${percent}%] ${green} Storm: ${yellow} file ---> ${file} ${reset}"

        python3 transform.py mdps-${folder}/${file}
	    target=`cat property.txt`
        start=`date +%s`
	
	    ${path}/storm/build/bin/storm --prism model.txt --prop "Pmax=? [F x=$target]" > ans.txt

	    sleep 2

        end=`date +%s`
        dif=`python3 calc_time.py`

        runtime=`echo ${runtime} + ${dif} | bc`

    done
    echo ${folder} ${runtime} >> results.txt
    cnt=`echo ${cnt} + 1 | bc`
done
