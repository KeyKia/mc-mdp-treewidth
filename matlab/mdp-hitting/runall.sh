g++ convert_to_matlab_system.cpp -std=c++11 -ocnv
for filename in ../../benchmarks/dacapobenchmark/outputs/raw_outputs/*.txt; do
	name=${filename##*/};
	name=${name%.txt};
	echo "processing $name";
	mkdir mdps-$name
	python ../../mdp/mdp_utils/mdpprob_reformat.py ../../benchmarks/dacapobenchmark/outputs/raw_outputs/$name.txt mdps-$name
	totaltime=0
	for txtfile in mdps-$name/*.txt; do
		echo textfile is $txtfile
		./cnv $txtfile >> mdps-$name/matlabcode.m 
	done
	matlab -nodisplay -nosplash -nodesktop -r "run('mdps-$name/matlabcode.m');exit;" 2>> mdps-$name/log-matlab.txt 
	tm=`paste -sd+ mdps-$name/log-matlab.txt | bc`
	echo $name $tm >> results.txt
done

