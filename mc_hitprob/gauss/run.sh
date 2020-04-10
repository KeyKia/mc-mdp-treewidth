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
    folder=${folder%".txt"}
	echo "${green}Running Gaussian Elimination for computing Markov Chain Hitting Probabilities on benchmark --------> $folder ${reset}"
	percent=`echo "100 * ${cnt} / ${tot}" | bc`

	mkdir mcs-$folder

	python3 ${path}utils/mcprob_reformat.py ${path}benchmarks/dacapobenchmark/outputs/raw_outputs/$folder.txt mcs-$folder
	totaltime=0
	for file in `ls mcs-$folder`
	do
        echo " [${percent}%] ${green} Gaussian Elimination: ${yellow} file ---> ${file} ${reset}"
		./${path}build/gauss-mc-hitprob mcs-$folder/$file > mcs-$folder/$file.log
		v=`cat mcs-$folder/$file.log`
		echo $v + $totaltime | bc > temp.tmp
		totaltime=`cat temp.tmp`
	done
	echo $folder $totaltime >> results.txt
	cnt=`echo ${cnt} + 1 | bc`
done

