g++ convert_to_matlab_system.cpp -std=c++11 -ocnv
for filename in ../../benchmarks/dacapobenchmark/outputs/raw_outputs/*.txt; do
	name=${filename##*/};
	name=${name%.txt};
	echo "processing $name";
	mkdir mcs-$name
	python ../../mc/mc_utils/mcprob_reformat.py ../../benchmarks/dacapobenchmark/outputs/raw_outputs/$name.txt mcs-$name
	totaltime=0
	for txtfile in mcs-$name/*.txt; do
		echo textfile is $txtfile
		./cnv $txtfile >> mcs-$name/matlabcode.m 
	done
	/Applications/MATLAB_R2019b.app/bin/matlab -nodisplay -nosplash -nodesktop -r "run('mcs-$name/matlabcode.m');exit;" 2>> mcs-$name/log-matlab.txt 
	tm=`paste -sd+ mcs-$name/log-matlab.txt | bc`
	echo $name $tm >> results.txt
done

